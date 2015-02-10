/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 19:32:53 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Factory.hpp>
#include <Operand.hpp>
#include <cstdio> 
#include <cstdlib> 

Factory::Factory(void)
{}

Factory::Factory(Factory const & src)
{
	*this = src;	
}

Factory::~Factory(void)
{}


IOperand const ** Factory::readOperand( std::istream const & s ) const
{
	std::string					value;
	std::string					*ref;
	Factory::eOpCode			res = 0;
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
	switch(type)
	{
		case Int8 :
			return createInt8(value);
		case Int16 :
			return createInt16(value);
		case Int32 :
			return createInt32(value);
		case Float :
			return createFloat(value);
		case Double :
			return createDouble(value);
		default :
			return NULL;
	}
}

Factory::eOpCode	Factory::readOpcode(std::istream const & s )
{
	std::string			op;
	std::string			*ref;
	Factory::eOpCode	res = 0;

	s >> op;
	ref = Factory::eOpCodeString;
	while (ref[res])
	{
		if (ref[res].compare(op))
			return res;
		res++;
	}
	throw AvmSyntaxError(op + " is note a valide opcode");
}

Instruction const * Factory::createInstruction(std::istream const & s ) const
{
	IOperand *arg;

	switch(readOpcode(s))
	{
		case Push :
			return new Instr<Push,1>(&(arg = readOperand()));
		case Pop :
			return new Instr<Pop,0>();
		case Dump :
			return new Instr<Dump,0>();
		case Assert :
			return new Instr<Assert,1>(&(arg = readOperand()));
		case Add :
			return new Instr<Add,0>();
		case Sub :
			return new Instr<Sub,0>();
		case Mul :
			return new Instr<Mul,0>();
		case Div :
			return new Instr<Div,0>();
		case Mod :
			return new Instr<Mod,0>();
		case Print :
			return new Instr<Print,0>();
		case Exit :
			return new Instr<Exit,0>();
	}
}

IOperand const * Factory::createInt8( std::string const & value ) const
{
	return new Operand<int8_t, Int8>(Int8, boost::lexical_cast<int8_t>(value));
}

IOperand const * Factory::createInt16( std::string const & value ) const
{
	return new Operand<int16_t, Int16>(Int8, boost::lexical_cast<int16_t>(value));
}

IOperand const * Factory::createInt32( std::string const & value ) const
{
	return new Operand<int32_t, Int32>(Int8, boost::lexical_cast<int32_t>(value));
}

IOperand const * Factory::createFloat( std::string const & value ) const
{
	return new Operand<float, Float>(Int8, boost::lexical_cast<float>(value));
}

IOperand const * Factory::createDouble( std::string const & value ) const
{
	return new Operand<double, Double>(Int8, boost::lexical_cast<double>(value));
}
