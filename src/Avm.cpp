/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 12:58:22 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/12 21:20:02 by mguinin          ###   ########.fr       */
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
	Avm		&avm = *Avm::_data_mode_avm;

	res = avm._stack;
	avm._stack += size;
	if (avm._stack > avm._stack_end)
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


void	Avm::data_mode(bool data_mode)
{
	int	size;

	if (data_mode)
	{
		size = DATA_MAX_SIZE;
		Avm::_data_mod_avm = this;
	}
	else
	{
		_data_segment = static_cast<IOperand *>
			(realloc(_stack_start, (int)(_stack - _stack_start)));
		size = _line * sizeof(Operand<double, Double>);
		Avm::_data_mode_avm = NULL;
	}
	_line = 0;
	_stack_start =
	_stack = static_cast<IOperand *>(new byte[size]);
	_stack_end = static_cast<IOperand *>(static_cast<byte *>_stack + size);
}

void	Avm::write_instruction(eOpcode opcode)
{
	_instruction[_line++] = opcode;
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


void		Avm::instruction_loop()
{
	static		(Avm::*instr_list)( void )[]
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

	while(_instruction != Exit)
	{
		instr_list[_instruction++];
		_line++;
	}
}


void		Avm::run()
{
	_line = 0;

}
