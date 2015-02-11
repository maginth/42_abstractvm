/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 12:55:59 by mguinin          ###   ########.fr       */
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
	this->*(Factory::create_func[type])(value);
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
			return new Instr<Push,1>(readOperand());
		case Pop :
			return new Instr<Pop>();
		case Dump :
			return new Instr<Dump>();
		case Assert :
			return new Instr<Assert,1>(readOperand());
		case Add :
			return new Instr<Add>();
		case Sub :
			return new Instr<Sub>();
		case Mul :
			return new Instr<Mul>();
		case Div :
			return new Instr<Div>();
		case Mod :
			return new Instr<Mod>();
		case Print :
			return new Instr<Print>();
		case Exit :
			return new Instr<Exit>();
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
