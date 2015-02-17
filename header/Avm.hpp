/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/17 15:22:19 by mguinin          ###   ########.fr       */
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

	enum eOpcode {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit};	

	Avm &		operator=(Avm const & rhs);

	void					run();
	void					instruction_loop(void);
	inline static void		*reserveStack(int const size);
	inline void				assemble_mode(bool);
	void					pop(void);
	void					push(void);
	void					dump(void);
	void					assert(void);
	void					print(void);
	int const &				get_line(void);
	inline void				write_instruction(eOpcode opcode);

	template<IOperand const * (IOperand::*FUNC)(IOperand const &)>
	void 					binary_op()
	{
		IOperand		&a;
		IOperand		&b;

		pop();
		a = *_stack;
		pop();
		b = *_stack;
		if (a.getType() < b.getType())
			b.upgrade(a, FUNC);
		else
			(a.*FUNC)(b);
	}

	void					saveBinary(std::ofstream & ofs) const;
	void					loadBinary(std::ifstream & ifs);
	
private:

	inline IOperand * byte_shift(IOperand * ref, int shift) const;

	IOperand	*_stack;
	IOperand	*_stack_start;
	IOperand	*_stack_end;

	IOperand	*_data_segment;

	eOpcode		*_instruction;
	
	int			_line;
	static Avm *	_assemble_mode_avm;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);

#endif // AVM_HPP