/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/25 20:00:06 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Factory.hpp>
#include <Operand.hpp>
#include <cstdio> 
#include <cstdlib> 
#include <algorithm>
#include <stdlib.h>
#include <string>


std::vector<std::string>		Factory::eOperandTypeString = 
	{"int8", "int16", "int32", "float", "double"};
std::vector<std::string>		Factory::eOpCodeString = 
	{"push", "pop", "dump", "assert", "add", "sub", "mul", "div", "mod", "print", "exit"};
const int				Factory::eOpCodeArg[] =
	{1, 0, 0, 1, 0, 0, 0, 0 ,0 ,0 ,0};


Factory::Factory(void)
{
	//just for the compiler to generate template classes
	Operand<int8_t, Int8>(0);
	Operand<int16_t, Int16>(0);
	Operand<int32_t, Int32>(0);
	Operand<float, Float>(0);
	Operand<double, Double>(0);
}


Factory::~Factory(void)
{}

static inline int			indexOf(std::vector<std::string> & v, std::string s)
{
	return std::find(v.begin(), v.end(), s) - v.begin();
}

static inline std::string	mstr(std::smatch & m, int index)
{
	return std::string(m[index].first, m[index].second);
}

void			 Factory::readOperand(std::string s, int const nb_arg) const
{
	eOperandType				type;
	static const std::regex 	reg("^(\\w+)\\s*\\(\\s*(.+?)\\s*\\)\\s*(.*)");
	std::smatch					m;

	if (!std::regex_match(s, m, reg))
		throw AvmException(s + " is not a valid operand" );

	type = static_cast<eOperandType>(indexOf(eOperandTypeString, mstr(m, 1)));

	if (type == TypeError)
		throw AvmException(mstr(m ,1) + " is note a valide type");
	createOperand(type, mstr(m, 2));
	if (nb_arg > 1)
		readOperand(mstr(m, 3), nb_arg - 1);
}

IOperand const * Factory::createOperand(eOperandType type, std::string const & value ) const
{
	static t_create_func create_func[] =
	{
		&Factory::createInt8,
		&Factory::createInt16,
		&Factory::createInt32,
		&Factory::createFloat,
		&Factory::createDouble,
	};
	return (this->*(create_func[type]))(value);
}

Avm::eOpcode			Factory::match_line(std::istream & s) const
{
	std::string 			line;
	static std::regex const reg("^\\s*(\\w*)\\s*([^;]*)(;?;?)");
	std::smatch				m;

	getline(s, line);
	std::regex_match(line, m, reg);
	if (m[1].first == m[1].second && m[2].first != m[2].second)
		throw AvmException("Syntaxe error : " + line);
	if (m[1].first != m[1].second)
		return readOpcode(m);
	if (s.eof() || (m[3].first - m[3].second) == 2)
		throw EndOfInputFile();
	return match_line(s);
}


Avm::eOpcode		Factory::readOpcode(std::smatch & m) const
{
	Avm::eOpcode		res;
	int					nb_arg;

	res = static_cast<Avm::eOpcode>(indexOf(eOpCodeString, mstr(m, 1)));

	if (res == Avm::CodeError)
		throw AvmException(mstr(m, 1) + " is note a valide opcode");
	nb_arg = Factory::eOpCodeArg[res];
	if (nb_arg > 0)
		readOperand(mstr(m, 2), nb_arg);
	return res;
}


void			Factory::assemble_file(std::istream & s, Avm & avm, std::ofstream * ofs) const
{
	avm.assemble_mode(true);
	try
	{
		while (true)
			avm.write_instruction(match_line(s));
	}
	catch(EndOfInputFile &e)
	{}
	if (ofs)
		avm.saveBinary(*ofs);
	avm.assemble_mode(false);
}

static long		stoi_range_checked(std::string const & value, long range)
{
	long		res = std::stol(value);

	if (abs(res) > range)
		throw AvmException("integer out of range " + res);
	return res;

}

IOperand const * Factory::createInt8( std::string const & value ) const
{
	return new Operand<int8_t, Int8>(stoi_range_checked(value, 0x7f));
}

IOperand const * Factory::createInt16( std::string const & value ) const
{
	return new Operand<int16_t, Int16>(stoi_range_checked(value, 0x7fff));
}

IOperand const * Factory::createInt32( std::string const & value ) const
{
	return new Operand<int32_t, Int32>(stoi_range_checked(value, 0x7fffff));
}

IOperand const * Factory::createFloat( std::string const & value ) const
{
	return new Operand<float, Float>(std::stof(value));
}

IOperand const * Factory::createDouble( std::string const & value ) const
{
	return new Operand<double, Double>(std::stod(value));
}
