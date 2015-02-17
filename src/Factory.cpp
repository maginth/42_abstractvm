/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/17 16:37:10 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Factory.hpp>
#include <Operand.hpp>
#include <cstdio> 
#include <cstdlib> 
#include <boost/lexical_cast.hpp>


std::string const		Factory::eOperandTypeString[] = 
	{"Int8", "Int16", "Int32", "Float", "Double", NULL};
std::string const		Factory::eOpCodeString[] = 
	{"push", "pop", "dump", "assert", "add", "sub", "mul", "div", "mod", "print", "exit", NULL};
const int				Factory::eOpCodeArg[] =
	{1, 0, 0, 1, 0, 0, 0, 0 ,0 ,0 ,0};
t_create_func Factory::create_func[] =
	{
		&Factory::createInt8,
		&Factory::createInt16,
		&Factory::createInt32,
		&Factory::createFloat,
		&Factory::createDouble,
	};

Factory::Factory(void)
{}

Factory::Factory(Factory const & src)
{
	*this = src;	
}

Factory::~Factory(void)
{}


IOperand const * Factory::readOperand( std::ifstream const & s ) const
{
	std::string					value;
	std::string					*ref;
	Avm::eOpcode				res = 0;
	static const boost::regex 	e("^(\\d+)\\((\\d+)\\)$");
	boost::smatch				match;

	s >> value;
	boost::regex_search(value, match, e);
	if (!match[0])
		throw AvmSyntaxError(value + " is not a valid operand" )
	ref = Factory::eOperationString;
	while (ref[res])
	{
		if (ref[res].compare(match[1].first))
			return createOperand(res, match[1].second);
		res++;
	}
	throw AvmSyntaxError(match[1].first + " is note a valide type");
}

IOperand const * Factory::createOperand( Factory::eOperandType type, std::string const & value ) const
{
	this->*(Factory::create_func[type])(value);
}

std::string			Factory::skipComment(std::ifstream const & s) const
{
	std::string			res;

	do {
		s >> res;
	} while (res[0] == ';' && res[1] != ';');
	if (s.eof() || (res[0] == ';' && res[1] == ';'))
		throw EndOfInputFile();
	return res;
}

Avm::eOpCode		Factory::readOpcode(std::ifstream const & s ) const
{
	std::string			op;
	std::string			*ref;
	Avm::eOpcode		res = 0;
	int					args;

	s >> op;
	ref = Factory::eOpCodeString;
	while (ref[res])
	{
		if (ref[res].compare(op))
		{
			args = Factory::eOpCodeArg[res];
			while (args--)
				readOperand();
			return res;
		}
		res++;
	}
	throw AvmSyntaxError(op + " is note a valide opcode");
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
