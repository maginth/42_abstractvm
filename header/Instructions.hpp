/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instructions.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:23:32 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 18:30:14 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class Instructions
{
public:
	void		exec(IOperand	* stack) const = 0;
};

template<Factory::eOpCode OPCODE, int OP_NB>
class Instr : Instructions {
public:
	Instr<OPCODE, OP_NB>(void);
	Instr<OPCODE, OP_NB>(IOperand **operand)
	{
		for (int i = 0; i < OP_NB; i++)
			_operand[i] = operand[i];
	}

	virtual ~Instr<OPCODE, OP_NB>(void) {}

	Instructions &		operator=(Instructions const & rhs);
	
private:
	Instr<OPCODE, OP_NB>(Instructions const & src);

	IOperand *		_operand[OP_NB];
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