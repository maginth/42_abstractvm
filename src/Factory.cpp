/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 16:46:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/09 17:54:11 by mguinin          ###   ########.fr       */
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

IOperand const * Factory::createOperand( eOperandType type, std::string const & value ) const
{
	switch(type)
	{
		case Int8 :
			createInt8(value);
			break;
		case Int16 :
			createInt16(value);
			break;
		case Int32 :
			createInt32(value);
			break;
		case Float :
			createFloat(value);
			break;
		case Double :
			createDouble(value);
			break;
		default :
			//error
			break;
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
