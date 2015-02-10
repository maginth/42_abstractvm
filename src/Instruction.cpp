/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Instruction.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 14:16:01 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 17:57:24 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Instruction.hpp>



void Instr<Push, 1>::exec(IOperand	** stack) const
{
	*stack = operand->push(*stack);
}

void Instr<Pop, 0>::exec(IOperand	** stack) const
{
	*stack->pop();
}

void Instr<Dump, 0>::exec(IOperand	** stack) const
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

void Instr<Add, 0>::exec(IOperand	** stack) const
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = *stack->pop();
	*stack = b->pop()->push(a + b);
}

void Instr<Sub, 0>::exec(IOperand	** stack) const
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = *stack->pop();
	*stack = b->pop()->push(a - b);
}

void Instr<Mul, 0>::exec(IOperand	** stack) const
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = *stack->pop();
	*stack = b->pop()->push(a * b);
}

void Instr<Div, 0>::exec(IOperand	** stack) const
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = *stack->pop();
	*stack = b->pop()->push(a / b);
}

void Instr<Mod, 0>::exec(IOperand	** stack) const
{
	IOperand		*a;
	IOperand		*b;

	a = *stack;
	b = *stack->pop();
	*stack = b->pop()->push(a % b);
}

void Instr<Print, 0>::exec(IOperand ** stack) const
{
	std::cout << **stack << std::endl;
}

void Instr<Exit, 0>::exec(IOperand	** stack) const
{
	*stack->empty_stack();
	*stack = NULL;
}