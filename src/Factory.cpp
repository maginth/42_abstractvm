/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/18 13:42:43 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Factory.hpp>
#include <Operand.hpp>
#include <cstdio> 
#include <cstdlib> 
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <algorithm>


std::vector<std::string const>		Factory::eOperandTypeString = 
	{"Int8", "Int16", "Int32", "Float", "Double"};
std::vector<std::string const>		Factory::eOpCodeString = 
	{"push", "pop", "dump", "assert", "add", "sub", "mul", "div", "mod", "print", "exit"};
const int				Factory::eOpCodeArg[] =
	{1, 0, 0, 1, 0, 0, 0, 0 ,0 ,0 ,0};


Factory::Factory(void)
{}

Factory::Factory(Factory const & src)
{
	*this = src;	
}

Factory::~Factory(void)
{}


IOperand const * Factory::readOperand( std::ifstream & s ) const
{
	std::string					whole_type;
	eOperandType				res;
	static const boost::regex 	e("^(\\d+)\\((\\d+)\\)$");
	boost::smatch				match;

	s >> whole_type;
	if (!boost::regex_match(whole_type, match, e))
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

IOperand const * Factory::createOperand( eOperandType type, std::string & value ) const
{
	static t_create_func create_func[] =
	{
		&Factory::createInt8,
		&Factory::createInt16,
		&Factory::createInt32,
		&Factory::createFloat,
		&Factory::createDouble,
	};
	this->*(Factory::create_func[type])(value);
}

std::string			Factory::skipComment(std::ifstream & s) const
{
	std::string			res;

	do {
		s >> res;
	} while (res[0] == ';' && res[1] != ';');
	if (s.eof() || (res[0] == ';' && res[1] == ';'))
		throw EndOfInputFile();
	return res;
}


Avm::eOpcode		Factory::readOpcode(std::ifstream & s ) const
{
	std::string			op;
	Avm::eOpcode		res;
	int					args;

	s >> op;
	res = static_cast<Avm::eOpcode>(
			find(eOpCodeString.begin(), eOpCodeString.end(), op)
			 - eOpCodeString.begin());
	if (res != CodeError)
	{
		args = Factory::eOpCodeArg[res];
		while (args--)
			readOperand();
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
	catch(EndOfInputFile &e);
	avm.saveBinary(ofs);
	avm.assemble_mode(false);
}

IOperand const * Factory::createInt8( std::string const & value ) const
{
	return new Operand<int8_t, Int8>(boost::lexical_cast<int8_t>(value));
}

IOperand const * Factory::createInt16( std::string const & value ) const
{
	return new Operand<int16_t, Int16>(boost::lexical_cast<int16_t>(value));
}

IOperand const * Factory::createInt32( std::string const & value ) const
{
	return new Operand<int32_t, Int32>(boost::lexical_cast<int32_t>(value));
}

IOperand const * Factory::createFloat( std::string const & value ) const
{
	return new Operand<float, Float>(boost::lexical_cast<float>(value));
}

IOperand const * Factory::createDouble( std::string const & value ) const
{
	return new Operand<double, Double>(boost::lexical_cast<double>(value));
}
