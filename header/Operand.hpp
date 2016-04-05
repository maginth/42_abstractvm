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
#include <string>
#include <IOperand.hpp>
#include <Avm.hpp>
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

inline double mod(double const a, double const b)
{
	return fmod(a, b);
}

inline float mod(float const a, float const b)
{
	return fmod(a, b);
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
		(void)(size);
		return Avm::reserveStack(B_SIZE);
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

	virtual IOperand const * operator / ( IOperand const & rhs ) const
	{
		if (static_cast<const TYPE>(rhs) == static_cast<TYPE>(0))
			throw AvmException("second operand of / is 0");
		return new Operand<TYPE, ETYPE>(_value / static_cast<const TYPE>(rhs));
	}

	virtual IOperand const * operator % ( IOperand const & rhs ) const
	{
		if (static_cast<const TYPE>(rhs) == static_cast<TYPE>(0))
			throw AvmException("second operand of %% is 0");
		return new Operand<TYPE, ETYPE>(mod(_value, static_cast<const TYPE>(rhs)));
	}

	virtual bool			 operator==( IOperand const & rhs ) const
	{
		return rhs.getPrecision() == ETYPE && _value == static_cast<TYPE>(rhs);
	}

	virtual const void * getValue( void ) const
	{
		return &_value;
	}

	virtual std::string const toString( void ) const
	{
		return (Avm::eOperandString[ETYPE] + '(' + std::to_string(_value) + ')');
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