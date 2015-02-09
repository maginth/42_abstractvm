/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:43:15 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/09 18:00:23 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

enum eOperandType = {Int8, Int16, Int32, Float, Double};
string		eOperandTypeString[] = {"Int8", "Int16", "Int32", "Float", "Double"};

class Factory {
public:
	Factory(void);
	virtual ~Factory(void);

	IOperand const * createOperand( std::string const & value ) const;

	IOperand const * createOperand( eOperandType type, std::string const & value ) const;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;
	
private:
	Factory(Factory const & src);
	Factory &		operator=(Factory const & rhs);
	
};



