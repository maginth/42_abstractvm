/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:48:16 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 14:08:24 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <IOperand.hpp>
#include <Factory.hpp>

#include <iostream>

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

	virtual IOperand const upgrade(IOperand & op) const
	{
		return Operand<TYPE, ETYPE>(static_cast<TYPE>(op));
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

	virtual IOperand const * operator+( IOperand const & rhs ) const
	{
		if (ETYPE < rhs->getType())
			return rhs.upgrade(*this) + rhs;
		return new Operand<TYPE, ETYPE>(_value + rhs);
	}

	virtual IOperand const * operator-( IOperand const & rhs ) const
	{
		if (ETYPE < rhs->getType())
			return rhs.upgrade(*this) - rhs;
		return new Operand<TYPE, ETYPE>(_value - rhs);
	}

	virtual IOperand const * operator*( IOperand const & rhs ) const
	{
		if (ETYPE < rhs->getType())
			return rhs.upgrade(*this) * rhs;
		return new Operand<TYPE, ETYPE>(_value * rhs);
	}

	virtual IOperand const * operator/( IOperand const & rhs ) const
	{
		if (ETYPE < rhs->getType())
			return rhs.upgrade(*this) / rhs;
		return new Operand<TYPE, ETYPE>(_value / rhs);
	}

	virtual IOperand const * operator%( IOperand const & rhs ) const
	{
		if (ETYPE < rhs->getType())
			return rhs.upgrade(*this) % rhs;
		return new Operand<TYPE, ETYPE>(_value % rhs);
	}

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

protected:
	template<typename TYPE2>
	virtual operator TYPE2() const
	{
		return static_cast<TYPE2>(_value);
	}

private:

	TYPE const				_value;
	IOperand *				_prev;
	
};


template<typename TYPE, eOperandType ETYPE>
std::ostream &	operator<<(std::ostream & stream, Operand<TYPE, ETYPE> const & s)
{
	std::ostream << s.toString() << std::endl;
}