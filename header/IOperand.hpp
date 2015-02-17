/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:37:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/16 12:22:51 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I_OPERAND_HPP
#define I_OPERAND_HPP


enum eOperandType {Int8, Int16, Int32, Float, Double};

class IOperand {
public:

	virtual int 			getPrecision( void ) const = 0; // Precision of the type of the instance
	virtual eOperandType 	getType( void ) const = 0; // Type of the instance

	virtual IOperand const * operator+( IOperand const & rhs ) const = 0; // Sum
	virtual IOperand const * operator-( IOperand const & rhs ) const = 0; // Difference
	virtual IOperand const * operator*( IOperand const & rhs ) const = 0; // Product
	virtual IOperand const * operator/( IOperand const & rhs ) const = 0; // Quotient
	virtual IOperand const * operator%( IOperand const & rhs ) const = 0; // Modulo
	virtual bool			 operator==( IOperand const & rhs ) const = 0;
	virtual IOperand * const upgrade() const = 0;

	virtual std::string const & toString( void ) const = 0; // String representation of the instance
	virtual int			const 	opSize( void ) const = 0;
	virtual ~IOperand( void ) {}

	virtual operator int8_t() const;
	virtual operator int16_t() const;
	virtual operator int32_t() const;
	virtual operator float() const;
	virtual operator double() const;

};

#endif // I_OPERAND_HPP