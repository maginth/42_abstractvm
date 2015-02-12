/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/12 21:17:58 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#define DATA_MAX_SIZE 10000000
typedef unsigned char byte;


class Avm {
public:
	Avm(void);
	Avm(Avm const & src);
	virtual ~Avm(void);

	enum eOpCode = {Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit};	

	Avm &		operator=(Avm const & rhs);

	void					run();
	void					instruction_loop();
	inline static void		reserveStack(int const size);
	inline void				data_mode(bool);
	
private:
	IOperand	*_stack;
	IOperand	*_stack_start;
	IOperand	*_stack_end;

	IOperand	*_data_segment;

	eOpCode		*_instruction;
	
	int			_line;
	static Avm *	_data_mode_avm;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);