/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/11 12:58:22 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/18 13:44:37 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <Avm.hpp>
#include <Operand.hpp>

Avm::Avm(void) : _stack_start(NULL) {}

Avm::~Avm(void)
{
	if (_stack_start)
		delete _stack_start;
}

inline IOperand * Avm::byte_shift(IOperand * ref, int shift) const
{
	return reinterpret_cast<IOperand *>(reinterpret_cast<byte *>(ref) + shift);
}

inline void	* Avm::reserveStack(int const size)
{
	void	*res;
	Avm		&avm = *Avm::_assemble_mode_avm;

	res = avm._stack;
	avm._stack += size;
	if (avm._stack > avm._stack_end)
		throw AvmException("Avm stack overflow");
	return res;
}

void	Avm::pop()
{
	_stack = byte_shift(_stack, -_stack->opSize());
	if (_stack < _stack_start)
		throw AvmException("Pop on empty stack");
}

void	Avm::push()
{
	int	op_size;

	op_size = _data_segment->opSize();
	*_stack = *_data_segment;
	_data_segment = byte_shift(_data_segment, op_size);
	_stack = byte_shift(_stack, op_size);
}

void	Avm::print()
{
	std::cout << _stack->toString() << std::endl;
}

void	Avm::dump()
{
	IOperand	*save;

	save = _stack;
	while (_stack > _stack_start)
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
	int	instr_size;

	if (assemble_mode)
	{
		stack_size = DATA_MAX_SIZE;
		data_size = 0;
		instr_size = DATA_MAX_SIZE;
		if (_stack_start)
			delete _stack_start;
		_stack_start = reinterpret_cast<IOperand *>(new byte[stack_size + instr_size]);
		_instruction = reinterpret_cast<Avm::eOpcode*>(byte_shift(_stack_start, stack_size));
		Avm::_assemble_mode_avm = this;
	}
	else
	{
		stack_size = _line * sizeof(Operand<double, Double>);
		data_size = _stack - _stack_start;
		instr_size = _line * sizeof(_instruction);
		memmove(_instruction, _stack + stack_size + data_size, instr_size);
		_stack_start = reinterpret_cast<IOperand *>(realloc(_stack_start, stack_size + data_size + instr_size));
		Avm::_assemble_mode_avm = NULL;
	}
	_line = 0;
	_data_segment = _stack_end = byte_shift(_stack_start, stack_size);
	_instruction = reinterpret_cast<Avm::eOpcode*>(byte_shift(_data_segment, data_size));
	_stack = _stack_start;
}

inline void	Avm::write_instruction(Avm::eOpcode opcode)
{
	_instruction[_line++] = opcode;
}

void	Avm::saveBinary(std::ofstream & ofs) const
{
	int	data_size;

	data_size = _stack - _stack_start;
	ofs.write(reinterpret_cast<const char *>(&_line), sizeof(int));
	ofs.write(reinterpret_cast<const char *>(&data_size), sizeof(int));
	ofs.write(reinterpret_cast<const char *>(_instruction), _line * sizeof(_instruction));
	ofs.write(reinterpret_cast<const char *>(_stack_start), data_size);
	ofs.close();
}

void	Avm::loadBinary(std::ifstream & ifs)
{
	int	stack_size;
	int	data_size;
	int	instr_size;

	ifs.read(reinterpret_cast<char *>(&_line), sizeof(int));
	ifs.read(reinterpret_cast<char *>(&data_size), sizeof(int));
	stack_size = _line * sizeof(Operand<double, Double>);
	instr_size = _line * sizeof(_instruction);
	if (_stack_start)
		delete _stack_start;
	_stack = _stack_start = reinterpret_cast<IOperand *>(new byte[stack_size + data_size + instr_size]);
	_data_segment = _stack_end = byte_shift(_stack_start, stack_size);
	_instruction = reinterpret_cast<Avm::eOpcode*>(byte_shift(_data_segment, data_size));
	ifs.read(reinterpret_cast<char *>(_instruction), instr_size);
	ifs.read(reinterpret_cast<char *>(_data_segment), data_size);
	ifs.close();
}

int const &		Avm::get_line()
{
	return _line;
}


void		Avm::run()
{
	static		t_instr_func instr_list[]
	{
		&Avm::push,
		&Avm::pop,
		&Avm::dump,
		&Avm::assert,
		/*&Avm::binary_op<&IOperand::operator+ >,
		&Avm::binary_op<&IOperand::operator- >,
		&Avm::binary_op<&IOperand::operator* >,
		&Avm::binary_op<&IOperand::operator/ >,
		&Avm::binary_op<&IOperand::operator% >,*/
		&Avm::print,
		NULL
	};

	while(*_instruction < Exit)
	{
		(this->*(instr_list[*(_instruction++)]))();
		_line++;
	}
	if (*_instruction != Exit)
		throw AvmException("corrupt file : invalide binary operation");
}
