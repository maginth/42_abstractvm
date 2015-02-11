/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/11 14:30:43 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#define STACK_SIZE 
typedef unsigned char byte;

#define OPINT unsigned char; 

class Avm {
public:
	Avm(void);
	Avm(Avm const & src);
	virtual ~Avm(void);

	enum eOpCode = {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit};
	static string const		eOpCodeString[] = 
		{"push", "pop", "dump", "assert", "add", "sub", "mul", "div", "mod", "print", "exit", NULL};

	Avm &		operator=(Avm const & rhs);

	void			run();
	inline void		reserveStack(int const size);
	
private:
	IOperand	*_stack;
	IOperand	*_stack_start;
	IOperand	*_stack_end;

	IOperand	*_data_segment;

	eOpCode		*_instruction;
	
	int			_line;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);