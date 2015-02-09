/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:48:16 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/09 16:48:34 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <IOperand.hpp>
#include <Factory.hpp>

#include <iostream>

template<typename TYPE, eOperandType ETYPE>
class Operand : public IOperand, public CastValue
{
public:
	Operand<TYPE, ETYPE>(void);
	Operand<TYPE, ETYPE>(Operand<TYPE, ETYPE> const & src);

	Operand<TYPE, ETYPE>(TYPE value) :
		_value(value),
	 	_type(ETYPE)
	{}

	virtual ~Operand<TYPE, ETYPE>(void)
	{}

	Operand<TYPE, ETYPE> &		operator=(Operand<TYPE, ETYPE> const & rhs);

	virtual int 			getPrecision( void ) const
	{
		return (sizeof(TYPE));
	}

	virtual eOperandType 	getType( void ) const
	{
		return ETYPE;
	}

	virtual IOperand const * operator+( IOperand const & rhs ) const
	{
		if (_type < rhs->getType())
			return rhs + *this;
		return new Operand<TYPE, ETYPE>(_value + rhs);
	}

	virtual IOperand const * operator-( IOperand const & rhs ) const
	{
		if (_type < rhs->getType())
			return rhs - *this;
		return new Operand<TYPE, ETYPE>(_value - rhs);
	}

	virtual IOperand const * operator*( IOperand const & rhs ) const
	{
		if (_type < rhs->getType())
			return rhs * *this;
		return new Operand<TYPE, ETYPE>(_value * rhs);
	}

	virtual IOperand const * operator/( IOperand const & rhs ) const
	{
		if (_type < rhs->getType())
			return rhs / *this;
		return new Operand<TYPE, ETYPE>(_value / rhs);
	}

	virtual IOperand const * operator%( IOperand const & rhs ) const
	{
		if (_type < rhs->getType())
			return rhs % *this;
		return new Operand<TYPE, ETYPE>(_value % rhs);
	}

	virtual std::string const & toString( void ) const
	{
		std::stringstream	res;

		res << _value;
		return (res.str());
	}

	virtual ~IOperand( void ) {}

protected:
	template<typename TYPE2>
	virtual operator TYPE2() const
	{
		return static_cast<TYPE2>(_value);
	}

private:

	TYPE const		_value;
	ETYPE const		_type;
	
};


template<typename TYPE, eOperandType ETYPE>
std::ostream &	operator<<(std::ostream & stream, Operand<TYPE, ETYPE> const & s)
{
	std::cout << s.toString() << std::endl;
}