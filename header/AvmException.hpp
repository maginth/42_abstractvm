/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AvmException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:28:20 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/16 11:14:31 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_EXCEPTION_HPP
#define AVM_EXCEPTION_HPP

#include <iostream>

class AvmException : public std::exception {
public:
	AvmException(void);
	AvmException(AvmException const & src);
	AvmException(std::string const & msg);
	virtual ~AvmException(void);

	AvmException &		operator=(AvmException const & rhs);

	virtual const char * what() const noexcept;
	
private:
	std::string const & _msg;
};

class EndOfInputFile : public AvmException
{};

#endif // AVM_EXCEPTION_HPP
