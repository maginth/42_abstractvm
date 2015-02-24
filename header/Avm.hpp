/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/24 16:34:27 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_HPP
#define AVM_HPP


#include <iostream>
#include <AvmException.hpp>
#include <IOperand.hpp>
#include <string.h>

#define DATA_MAX_SIZE 10000000
typedef unsigned char byte;

class Avm;
typedef void (Avm::*t_instr_func)(void);


class Avm {
public:
	Avm(void);
	Avm(Avm const & src);
	virtual ~Avm(void);

	enum eOpcode {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit, CodeError};	

	Avm &		operator=(Avm const & rhs);

	void					run();
	static void				*reserveStack(int const size);
	void					assemble_mode(bool);
	void					pop(void);
	void					push(void);
	void					dump(void);
	void					assert(void);
	void					print(void);
	int const &				get_line(void);
	void					write_instruction(eOpcode opcode);

	template<IOperand const * (IOperand::*FUNC)(IOperand const &) const>
	void 					binary_op()
	{
		pop();
		IOperand &a = *_stack;
		pop();
		IOperand &b = *_stack;
		if (a.getPrecision() < b.getPrecision())
			b.upgrade(a, FUNC);
		else
			(a.*FUNC)(b);
	}

	void					saveBinary(std::ofstream & ofs) const;
	void					loadBinary(std::ifstream & ifs);
	
private:

	IOperand * byte_shift(IOperand * ref, int shift) const;

	IOperand	*_stack;
	IOperand	*_stack_start;
	IOperand	*_stack_end;

	IOperand	*_data_segment;

	eOpcode		*_instruction;
	
	int			_line;
	static Avm *	_assemble_mode_avm;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);

inline IOperand * Avm::byte_shift(IOperand * ref, int shift) const
{
	return reinterpret_cast<IOperand *>(reinterpret_cast<byte *>(ref) + shift);
}

inline void	Avm::write_instruction(Avm::eOpcode opcode)
{
	_instruction[_line++] = opcode;
}

#endif // AVM_HPP