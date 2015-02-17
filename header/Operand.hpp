/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:48:16 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/16 12:16:10 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP


#include <iostream>
#include <IOperand.hpp>
#include <Factory.hpp>
#include <iostream>

#define CAST(T) \
	virtual operator T() const 			\
	{									\
		return static_cast<T>(_value);	\
	}

#define OPERATOR(X)
	virtual IOperand const * operator X ( IOperand const & rhs ) const	\
	{																	\
		return new Operand<TYPE, ETYPE>(_value X rhs);					\
	}																	\
}

#define OPERATOR_DIV(X)
	virtual IOperand const * operator X ( IOperand const & rhs ) const	\
	{
		if (rhs == static_cast<TYPE>(0))								\
			throw AvmException("second operand of X is 0");				\
		return new Operand<TYPE, ETYPE>(_value X rhs);					\
	}

template<typename TYPE, eOperandType ETYPE>
class Operand : public IOperand
{
public:
	Operand<TYPE, ETYPE>(void);
	Operand<TYPE, ETYPE>(Operand<TYPE, ETYPE> const & src);

	Operand<TYPE, ETYPE>(TYPE value) :
		_value(value)
	{}

	virtual ~Operand<TYPE, ETYPE>(void)
	{}

	virtual IOperand * const upgrade
	(
		IOperand & op, 
		IOperand * const (IOperand::*calc)(IOperand const &)
	) const
	{
		return Operand<TYPE, ETYPE>(static_cast<TYPE>(op)).*calc(*this);
	}

	Operand<TYPE, ETYPE> &		operator=(Operand<TYPE, ETYPE> const & rhs);

	virtual int 			getPrecision( void ) const
	{
		return (sizeof(TYPE));
	}

	virtual eOperandType 	getType( void ) const
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
		return rhs->getType() == ETYPE && _value == static_cast<TYPE>(rhs);
	}

	virtual std::string const & toString( void ) const
	{
		std::stringstream	res;

		res << _value;
		return (res.str());
	}

	virtual int			const 	opSize( void ) const
	{
		return sizeof(Operand);
	}

	void *				Operand::operator new(std::size_t size)
	{
		return Avm::reserveStack(int size);
	}

	virtual ~IOperand( void ) {}

private:

	TYPE const				_value;
};

#endif // OPERAND_HPP