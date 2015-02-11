/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instruction.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 14:16:01 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 12:48:47 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Instruction.hpp>



void Instr<Push, 1>::exec(IOperand	** stack) const
{
	*stack = operand->push(*stack);
}

void Instr<Pop>::exec(IOperand	** stack) const
{
	*stack->pop();
}

void Instr<Dump>::exec(IOperand	** stack) const
{
	*stack->dump();
}

void Instr<Assert, 1>::exec(IOperand	** stack) const
{
	if (**stack != *operand)
	{
		//errror;
	}
}

template<IOperand * (IOperand::*FUNC)(IOperand &)>
inline void stack_operation(IOperand	** stack)
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = a->pop_self();
	*stack = (a->*FUNC(b))->push_self(b->pop_self());
	delete a;
	delete b;
}

void Instr<Add>::exec(IOperand	** stack) const
{
	stack_operation<&IOperand::operator+>(stack);
}

void Instr<Sub>::exec(IOperand	** stack) const
{
	stack_operation<&IOperand::operator->(stack);
}

void Instr<Mul>::exec(IOperand	** stack) const
{
	stack_operation<&IOperand::operator*>(stack);
}

void Instr<Div>::exec(IOperand	** stack) const
{
	stack_operation<&IOperand::operator/>(stack);
}

void Instr<Mod>::exec(IOperand	** stack) const
{
	stack_operation<&IOperand::operator%>(stack);
}

void Instr<Print>::exec(IOperand ** stack) const
{
	std::cout << **stack << std::endl;
}

void Instr<Exit>::exec(IOperand	** stack) const
{
	*stack->empty_stack();
	*stack = NULL;
}