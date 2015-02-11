/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:43:15 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 12:57:19 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>


class Factory {
public:
	enum eOperandType = {Int8, Int16, Int32, Float, Double};
	static unsigned double eOperandMax[] 
		= {0xff, 0xffff, 0xffffffff, 0xffffffff, 0xffffffffffffffff};
	static string		eOperandTypeString[] = 
		{"Int8", "Int16", "Int32", "Float", "Double", NULL};


	static IOperand const * (Factory::create_func const)(std::string const & value)[] =
	{
		&Factory::createInt8,
		&Factory::createInt16,
		&Factory::createInt32,
		&Factory::createFloat,
		&Factory::createDouble,
	};

	Factory(void);
	virtual ~Factory(void);

	IOperand const * createOperand( std::string const & value ) const;

	Instruction const * createInstruction( std::string const & value ) const;

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



