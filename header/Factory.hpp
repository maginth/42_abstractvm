/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:43:15 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 19:37:52 by mguinin          ###   ########.fr       */
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

	enum eOpCode = {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit};
	static string		eOpCodeString[] = 
		{"push", "pop", "dump", "assert", "add", "sub", "mul", "div", "mod", "print", "exit", NULL};

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



