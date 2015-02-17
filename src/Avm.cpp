/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 12:58:22 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/16 11:51:43 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Avm.hpp>

Avm::Avm(void) : _stack_start(NULL) {}

Avm::~Avm(void)
{
	if (_stack_start)
		delete _stack_start;
}

inline void	* Avm::reserveStack(int const size)
{
	byte	*res;
	Avm		&avm = *Avm::_data_mode_avm;

	res = avm._stack;
	avm._stack += size;
	if (avm._stack > avm._stack_end)
		throw AvmException("Avm stack overflow");
	return res;
}

void	Avm::pop()
{
	static_cast<byte *>(_stack) -= _stack->opSize();
	if (_stack < _stack_start)
		throw AvmException("Pop on empty stack");
}

void	Avm::push()
{
	int	op_size;

	op_size = _data_segment->opSize();
	*_stack = *_data_segment;
	reinterpret_cast<byte *>(_data_segment) += op_size;
	reinterpret_cast<byte *>(_stack) 		+= op_size;
}

void	Avm::print()
{
	std::cout << _stack->toString() << std::endl;
}

void	Avm::dump()
{
	IOperand	*save;

	save = _stack;
	while (s > _stack_start)
	{
		pop();
		print();
	}
	_stack = save;
}

void	Avm::assert()
{
	IOperand	*a;
	IOperand	*b;

	a = _stack;
	pop();
	b = _stack;
	_stack = a;
	push();
	if (!(*a == *b))
		throw AvmException("Assert fail :" + a->toString() + " != " + b->toString());
	pop();
	
}


void	Avm::assemble_mode(bool assemble_mode)
{
	int	stack_size;
	int	data_size;
	int	line_size;

	if (assemble_mode)
	{
		stack_size = DATA_MAX_SIZE;
		data_size = 0;
		line_size = DATA_MAX_SIZE;
		if (_stack_start)
			delete _stack_start;
		_stack_start = new byte[stack_size + line_size];
		_instruction = static_cast<byte>(_stack_start) + stack_size;
		Avm::_assemble_mode_avm = this;
	}
	else
	{
		stack_size = _line * sizeof(Operand<double, Double>);
		data_size = _stack - _stack_start;
		line_size = _line * sizeof(Avm::eOpcode);
		memmove(_instruction, _stack + stack_size + data_size, line_size);
		_stack_start = realloc(_stack_start, stack_size + data_size + line_size);
		Avm::_assemble_mode_avm = NULL;
	}
	_line = 0;
	_data_segment = _stack_end = static_cast<byte *>(_stack_start) + stack_size;
	_instruction = static_cast<byte *>(_data_segment) + data_size;
	_stack = _stack_start;
}

void	Avm::write_instruction(Avm::eOpcode opcode)
{
	_instruction[_line++] = opcode;
}

void	Avm::saveBinary(ofstream & ofs) const
{
	int	data_size;

	data_size = _stack - _stack_start;
	ofs.write(_line, sizeof(int));
	ofs.write(data_size, sizeof(int));
	ofs.write(_instruction, _line * sizeof(Avm::eOpcode));
	ofs.write(_stack_start, data_size);
	ofs.close();
}

void	Avm::loadBinary(ifstream & ifs)
{
	int	stack_size;
	int	data_size;
	int	line_size;

	ifs.read(_line, sizeof(int));
	ifs.read(data_size, sizeof(int));
	stack_size = _line * sizeof(Operand<double, Double>);
	line_size = _line * sizeof(Avm::eOpcode);
	if (_stack_start)
		delete _stack_start;
	_stack = _stack_start = new byte[stack_size + data_size + line_size];
	_data_segment = _stack_end = static_cast<byte *>(_stack_start) + stack_size;
	_instruction = static_cast<byte *>(_data_segment) + data_size;
	ifs.read(_instruction, line_size);
	ifs.read(_data_segment, data_size);
	ifs.close();
}

template<IOperand * (IOperand::*FUNC)(IOperand &)>
void 	Avm::binary_op()
{
	IOperand		&a;
	IOperand		&b;

	pop();
	a = *_stack;
	pop();
	b = *_stack;
	if (a.getType < b.getType)
		b.upgrade(a,FUNC);
	else
		a.*FUNC(b);
}

void		Avm::run()
{
	static		(Avm::*instr_list)( void )[]
	{
		&Avm::push,
		&Avm::pop,
		&Avm::dump,
		&Avm::assert,
		&Avm::binary_op<&IOperand::operator+>,
		&Avm::binary_op<&IOperand::operator->,
		&Avm::binary_op<&IOperand::operator*>,
		&Avm::binary_op<&IOperand::operator/>,
		&Avm::binary_op<&IOperand::operator%>,
		&Avm::print,
		NULL
	}

	while(*_instruction < Exit)
	{
		instr_list[*(_instruction++)]();
		_line++;
	}
	if (_instruction != Exit)
		throw AvmException("corrupt file : invalide binary operation");
}
