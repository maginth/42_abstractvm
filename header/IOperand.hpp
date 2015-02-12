/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:37:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/12 19:39:10 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	virtual IOperand const * upgrade(IOperand::*cast) const = 0;

	virtual std::string const & toString( void ) const = 0; // String representation of the instance
	virtual int			const 	opSize( void ) const = 0;
	virtual ~IOperand( void ) {}

	virtual operator int8_t() const;
	virtual operator int16_t() const;
	virtual operator int32_t() const;
	virtual operator float() const;
	virtual operator double() const;

};