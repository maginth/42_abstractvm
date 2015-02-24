/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/24 18:36:04 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Factory.hpp>
#include <Operand.hpp>
#include <cstdio> 
#include <cstdlib> 
#include <regex>
#include <algorithm>
#include <stdlib.h>
#include <string>


std::vector<std::string>		Factory::eOperandTypeString = 
	{"Int8", "Int16", "Int32", "Float", "Double"};
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


IOperand const * Factory::readOperand( std::ifstream & s ) const
{
	std::string					whole_type;
	eOperandType				res;
	static const std::regex 	e("^(\\d+)\\((\\d+)\\)$");
	std::smatch					match;

	s >> whole_type;
	if (!std::regex_match(whole_type, match, e))
		throw AvmException(whole_type + " is not a valid operand" );

	res = static_cast<eOperandType>(
			std::find(eOperandTypeString.begin(),
					eOperandTypeString.end(),
					std::string(match[1].first, match[1].second))
			- eOperandTypeString.begin());

	std::string type(match[2].first, match[2].second);
	if (res != TypeError)
		return createOperand(res, type);
	throw AvmException(type + " is note a valide type");
}

IOperand const * Factory::createOperand( eOperandType type, std::string const & value ) const
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

std::string			Factory::skipComment(std::ifstream & s) const
{
	std::string 	res;

	do {
		s >> res;
	std::cout << res << std::endl;
		if (res[0] == ' ' || res[0] == ';')
		{
			getline(s, res);
	std::cout << res << std::endl;
			continue;
		}
	} while (!s.eof() && res[0] == ';' && res[1] != ';');
	if (s.eof() || (res[0] == ';' && res[1] == ';'))
		throw EndOfInputFile();
	return res;
}


Avm::eOpcode		Factory::readOpcode(std::ifstream & s) const
{
	std::string			op;
	Avm::eOpcode		res;
	int					args;

	op = skipComment(s);

	res = static_cast<Avm::eOpcode>(
			find(eOpCodeString.begin(), eOpCodeString.end(), op)
			 - eOpCodeString.begin());

	std::cout << res << std::endl;
	if (res != Avm::CodeError)
	{
		args = Factory::eOpCodeArg[res];
		while (args--)
			readOperand(s);
		getline(s, op);
		return res;
	}

	throw AvmException(op + " is note a valide opcode");
}


void			Factory::assemble_file(std::ifstream & s, Avm & avm, std::ofstream & ofs) const
{
	avm.assemble_mode(true);
	try
	{
		while (true)
			avm.write_instruction(readOpcode(s));
	}
	catch(EndOfInputFile &e)
	{}
	avm.saveBinary(ofs);
	avm.assemble_mode(false);
}

static long	stoi_range_checked(std::string const & value, long range)
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
