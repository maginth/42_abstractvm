/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Factory.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 14:43:15 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/25 19:49:05 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <AvmException.hpp>
#include <IOperand.hpp>
#include <Operand.hpp>
#include <Avm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

class Factory;
typedef IOperand const * (Factory::*t_create_func)(std::string const & value) const;

class Factory {
public:
	static std::vector<std::string>		eOperandTypeString;
	static std::vector<std::string>		eOpCodeString;
	static const int							eOpCodeArg[];

	Factory(void);
	virtual ~Factory(void);

	void			assemble_file(std::istream & s, Avm & avm, std::ofstream * ofs) const;
	
private:

	IOperand const * createOperand( std::string const & value ) const;

	IOperand const * createOperand( eOperandType type, std::string const & value ) const;

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;

	void			readOperand( std::string s, int const nb_arg ) const;
	Avm::eOpcode	match_line( std::istream & s ) const;
	Avm::eOpcode	readOpcode( std::smatch & m ) const;

	Factory(Factory const & src);
	Factory &		operator=(Factory const & rhs);
	
};

#endif // FACTORY_HPP


