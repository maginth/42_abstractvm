/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 12:58:22 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 14:29:40 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Avm.hpp>

Avm::Avm(void)
{}

Avm::~Avm(void)
{}

inline void	* Avm::reserveStack(int const size)
{
	byte	*res;

	byte = _stack;
	_stack += size;
	if (_stack > _stack_end)
		throw AvmStackOverflow();
	return res;
}

void	Avm::pop()
{
	*_stack -= _stack->opSize();
	if (_stack < _stack_start)
		throw AvmPopOnEmptyStack();
}

void	Avm::push()
{
	*_stack = *_data_segment;
	reinterpret_cast<byte *>(_data_segment) -= _data_segment->opSize();
}

void		Avm::run()
{
	while(_instruction != Exit)
	{
		_line++;
	}
}

template<IOperand * (IOperand::*FUNC)(IOperand &)>
void binary_op()
{
	IOperand		&a;
	IOperand		&b;

	pop();
	a = *_stack;
	pop();
	b = *_stack;
	a.*FUNC(b);
	delete a;
	delete b;
}


static		(Avm::*stack_operation)( void )[]
{
	&Avm::push,
	&Avm::pop,
	&Avm::dump,
	&Avm::Assert,
	&Avm::binary_op<&IOperand::operator+>,
	&Avm::binary_op<&IOperand::operator->,
	&Avm::binary_op<&IOperand::operator*>,
	&Avm::binary_op<&IOperand::operator/>,
	&Avm::binary_op<&IOperand::operator%>
}