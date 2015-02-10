/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Avm.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/10 13:19:51 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/10 17:55:51 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#define OPINT unsigned char; 

class Avm {
public:
	Avm(void);
	Avm(Avm const & src);
	virtual ~Avm(void);

	Avm &		operator=(Avm const & rhs);

	void		run();
	void		exit();
	
private:
	IOperand	*_stack;
	Instruction	*_first;
};

std::ostream &	operator<<(std::ostream & stream, Avm const & s);