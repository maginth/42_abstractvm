/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:48:16 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/25 18:23:13 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP


#include <iostream>
#include <IOperand.hpp>
#include <Avm.hpp>
#include <Factory.hpp>
#include <sstream>
#include <math.h>

#define CAST(T) \
	virtual operator T () const 			\
	{										\
		return static_cast< T >(_value);	\
	}

#define OPERATOR(X)	\
	virtual IOperand const * operator X ( IOperand const & rhs ) const		\
	{																		\
		return new Operand<TYPE, ETYPE>(_value X static_cast<const TYPE>(rhs));\
	}

template<typename T>
inline T mod(T const a, T const b)
{
	return a % b;
}

template<>
inline double mod(double const a, double const b)
{
	return fmod(a, b);
}

template<>
inline float mod(float const a, float const b)
{
	return fmod(a, b);
}

#define OPERATOR_DIV(X)	\
	virtual IOperand const * operator X ( IOperand const & rhs ) const		\
	{																		\
		if (static_cast<const TYPE>(rhs) == static_cast<TYPE>(0))			\
			throw AvmException("second operand of "#X" is 0");				\
		return new Operand<TYPE, ETYPE>(mod(_value, static_cast<const TYPE>(rhs)));\
	}

template<typename TYPE, eOperandType ETYPE>
class Operand : public IOperand
{
public:
	Operand<TYPE, ETYPE>(TYPE value) :
		_value(value)
	{}

	void *				operator new(std::size_t size)
	{
		return Avm::reserveStack(size);
	}

	virtual ~Operand<TYPE, ETYPE>(void)
	{}

	virtual IOperand const * upgrade(
		IOperand & op, 
		IOperand const * (IOperand::*calc)(IOperand const &) const
	) const
	{
		return (Operand<TYPE, ETYPE>(static_cast<TYPE>(op)).*calc)(*this);
	}


	virtual int 			getPrecision( void ) const
	{
		return ETYPE;
	}

	CAST(int8_t)
	CAST(int16_t)
	CAST(int32_t)
	CAST(float)
	CAST(double)

	OPERATOR(+)
	OPERATOR(-)
	OPERATOR(*)
	OPERATOR_DIV(/)
	OPERATOR_DIV(%)
	

	virtual bool			 operator==( IOperand const & rhs ) const
	{
		return rhs.getPrecision() == ETYPE && _value == static_cast<TYPE>(rhs);
	}

	virtual std::string const toString( void ) const
	{
		std::stringstream	res;

		res << _value;
		return (res.str());
	}

	virtual  int		 		opSize( void ) const
	{
		return sizeof(Operand);
	}


private:
	Operand<TYPE, ETYPE>(void);
	Operand<TYPE, ETYPE>(Operand<TYPE, ETYPE> const & src);
	Operand<TYPE, ETYPE> &		operator=(Operand<TYPE, ETYPE> const & rhs);

	TYPE const				_value;
};

#endif // OPERAND_HPP