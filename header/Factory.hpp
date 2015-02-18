/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:43:15 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/18 10:45:53 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <iostream>
#include <fstream>
#include <AvmException.hpp>
#include <IOperand.hpp>
#include <Operand.hpp>
#include <Avm.hpp>

class Factory;
typedef IOperand const * (Factory::*t_create_func)(std::string const & value) const;

class Factory {
public:
	static std::string const		eOperandTypeString[];
	static std::string const		eOpCodeString[];
	static const int				eOpCodeArg[];


	static t_create_func create_func[];

	Factory(void);
	virtual ~Factory(void);

	IOperand const * createOperand( std::string const & value ) const;

	IOperand const * createOperand( eOperandType type, std::string const & value ) const;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;


	void			assemble_file(std::ifstream & s, Avm & avm, std::ofstream & ofs) const;
	
private:
	Factory(Factory const & src);
	Factory &		operator=(Factory const & rhs);
	
};

#endif // FACTORY_HPP


