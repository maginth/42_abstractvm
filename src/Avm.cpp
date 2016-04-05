/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 12:58:22 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/25 19:28:58 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Avm.hpp>
#include <Operand.hpp>
#include <fstream>

std::vector<std::string>		Avm::eOperandString = 
	{"int8", "int16", "int32", "float", "double"};

inline IOperand * byte_shift(IOperand * ref, int shift)
{
	return reinterpret_cast<IOperand *>(reinterpret_cast<byte *>(ref) + shift);
}

Avm::Avm(void) : _stack_start(NULL), _line(0) {}

Avm * Avm::_assemble_mode_avm = NULL;

Avm::~Avm(void)
{
	if (_stack_start)
		free(_stack_start);
}

void	* Avm::reserveStack(int const size)
{
	Avm		&avm = *Avm::_assemble_mode_avm;
	void	*res;

	res = avm._stack;
	if (avm._stack >= avm._stack_end)
		throw AvmException("Avm stack overflow");
	avm._stack = byte_shift(avm._stack, size);
	return res;
}

void	Avm::pop()
{
	if (_stack <= _stack_start)
		throw AvmException("Pop on empty stack");
	_stack = byte_shift(_stack, -B_SIZE);
}

void	Avm::push()
{
	if (_stack >= _stack_end)
		throw AvmException("Avm stack overflow");
	memcpy(_stack, _data_segment, B_SIZE);
	_stack = byte_shift(_stack, B_SIZE);
	_data_segment = byte_shift(_data_segment, B_SIZE);
}

void	Avm::print()
{
	IOperand	*op;
	int			type;
	static std::string		tlist[]{"int8", "int16", "int32", "float", "double"};

	op = byte_shift(_stack, -B_SIZE);
	type = op->getPrecision();
	if (type != Int8)
		throw AvmException("print error: "
			+ tlist[type]
			+ " should be int8");
	std::cout << *reinterpret_cast<const char *>(op->getValue());
}

void	Avm::dump()
{
	IOperand	*i;

	i = _stack;
	while (i > _stack_start)
	{
		i = byte_shift(i, -B_SIZE);
		std::cout << i->toString() << std::endl;
	}
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
		throw AvmException("Assert fail " + a->toString() + " != " + b->toString());
	pop();
	
}

/*
** Memory Representation of different segment :
** _stack [_stack_size] :: _data_segment[_data_size] :: _instruction[instr_size]
*/

void	Avm::assemble_mode(bool assemble_mode)
{
	int	stack_size;
	int	data_size;
	int	instr_size;

	if (assemble_mode)
	{
		stack_size = DATA_MAX_SIZE;
		data_size = 0;
		instr_size = DATA_MAX_SIZE;
		if (_stack_start)
			free(_stack_start);
		_stack_start = reinterpret_cast<IOperand *>(new byte[stack_size + instr_size]);
		Avm::_assemble_mode_avm = this;
	}
	else
	{
		stack_size = _line * sizeof(Operand<double, Double>);
		data_size = reinterpret_cast<byte *>(_stack) - reinterpret_cast<byte *>(_stack_start);
		instr_size = _line * sizeof(_instruction);
		memmove(byte_shift(_stack_start, stack_size), _stack_start, data_size);
		memmove(byte_shift(_stack_start, stack_size + data_size), _instruction, instr_size);
		_stack_start = reinterpret_cast<IOperand *>(realloc(_stack_start, stack_size + data_size + instr_size));
	}
	_line = 0;
	_data_segment = _stack_end = byte_shift(_stack_start, stack_size);
	_instruction = reinterpret_cast<Avm::eOpcode*>(byte_shift(_data_segment, data_size));
	_stack = _stack_start;
}

void	Avm::saveBinary(std::ofstream & ofs) const
{
	int	data_size;

	data_size = reinterpret_cast<const char *>(_stack) - reinterpret_cast<const char *>(_stack_start);
	ofs << _line;
	ofs << data_size;
	ofs.write(reinterpret_cast<const char *>(_stack_start), data_size);
	ofs.write(reinterpret_cast<const char *>(_instruction), _line * sizeof(*_instruction));
}

void	Avm::loadBinary(std::ifstream & ifs)
{
	int	stack_size;
	int	data_size;
	int	instr_size;

	ifs >> _line;
	ifs >> data_size;
	stack_size = _line * B_SIZE;
	instr_size = _line * sizeof(*_instruction);
	if (_stack_start)
		delete _stack_start;
	_stack = _stack_start = reinterpret_cast<IOperand *>(new byte[stack_size + data_size + instr_size]);
	_data_segment = _stack_end = byte_shift(_stack_start, stack_size);
	_instruction = reinterpret_cast<Avm::eOpcode*>(byte_shift(_data_segment, data_size));
	ifs.read(reinterpret_cast<char *>(_data_segment), data_size);
	ifs.read(reinterpret_cast<char *>(_instruction), instr_size);
}

int const &		Avm::get_line()
{
	return _line;
}


void		Avm::run()
{
	static		void (Avm::*instr_list[])()
	{
		&Avm::push,
		&Avm::pop,
		&Avm::dump,
		&Avm::assert,
		&Avm::binary_op<&IOperand::operator + >,
		&Avm::binary_op<&IOperand::operator - >,
		&Avm::binary_op<&IOperand::operator * >,
		&Avm::binary_op<&IOperand::operator / >,
		&Avm::binary_op<&IOperand::operator % >,
		&Avm::print,
		NULL
	};

	Avm::_assemble_mode_avm = this;
	while(*_instruction < Exit)
	{
		(this->*(instr_list[*(_instruction++)]))();
		_line++;
	}
	if (*_instruction == CodeError)
		throw AvmException("program terminated without exit call");
	if (*_instruction != Exit)
		throw AvmException("corrupt file : invalide binary operation");
}
