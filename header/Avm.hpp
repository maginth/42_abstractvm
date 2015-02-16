/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/16 11:22:29 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_HPP
#define AVM_HPP


#include <iostream>
#include <Factory.hpp>
#define DATA_MAX_SIZE 10000000
typedef unsigned char byte;


class Avm {
public:
	Avm(void);
	Avm(Avm const & src);
	virtual ~Avm(void);

	enum eOpCode {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit};	

	Avm &		operator=(Avm const & rhs);

	void					run();
	void					instruction_loop(void);
	inline static void		reserveStack(int const size);
	inline void				assemble_mode(bool);
	void					pop(void);
	void					push(void);
	void					dump(void);
	void					assert(void);
	void					print(void);

	template<IOperand * (IOperand::*FUNC)(IOperand &)>
	void 					binary_op();

	void					saveBinary(std::ofstream & ofs) const;
	void					loadBinary(std::ifstream & ifs);
	
private:
	IOperand	*_stack;
	IOperand	*_stack_start;
	IOperand	*_stack_end;

	IOperand	*_data_segment;

	eOpCode		*_instruction;
	
	int			_line;
	static Avm *	_assemble_mode_avm;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);

#endif // AVM_HPP