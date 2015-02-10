/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:37:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 17:53:54 by mguinin          ###   ########.fr       */
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
	virtual void				dump( void ) const = 0;
	virtual IOperand *			pop( void ) const = 0;
	virtual IOperand *			push(IOperand *prev) const = 0;
	virtual void				empty_stack( void ) = 0;
	virtual ~IOperand( void ) {}

protected:
	template<typename TYPE>
	virtual operator TYPE() const = 0;


};