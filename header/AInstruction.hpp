/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AInstruction.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/09 17:30:02 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/09 17:52:17 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class AInstruction {
public:
	AInstruction(void);
	AInstruction(AInstruction const & src);
	virtual ~AInstruction(void);

	AInstruction &		operator=(AInstruction const & rhs);
	
	void	exec( void ) const = 0;

private:
	std::string	const	_name;
	bool const			_need_value
};

std::ostream &	operator<<(std::ostream & stream, AInstruction const & s);