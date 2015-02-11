/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instructions.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:23:32 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 11:13:49 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class Instructions
{
public:
	void		exec(IOperand	* stack) const = 0;
};

template<Factory::eOpCode OPCODE, int OP_NB = 0>
class Instr : Instr<OPCODE, OP_NB - 1> {
public:
	Instr<OPCODE, OP_NB>(void);

	template<class IOP, class... IOP_LISTE>
	Instr<OPCODE, OP_NB>(IOP operand, IOP_LISTE op_next) :
		Instr<OPCODE, OP_NB - 1>(op_next...),
		_operand(operand)
	{}

	virtual ~Instr<OPCODE, OP_NB>(void) {}

	Instructions &		operator=(Instructions const & rhs);
	
private:
	Instr<OPCODE, OP_NB>(Instructions const & src);

	IOperand		_operand;
};

template<Factory::eOpCode OPCODE>
class Instr : Instructions {
public:
	Instr<OPCODE, 0>(void) {}
	virtual ~Instr<OPCODE, 0>(void) {}


private:
	Instr<OPCODE, 0>(Instructions const & src);
	Instructions &		operator=(Instructions const & rhs);
};

std::ostream &	operator<<(std::ostream & stream, Instructions const & s);