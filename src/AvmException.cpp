/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AvmException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:46:39 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/21 15:15:43 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <AvmException.hpp>

std::string const default_msg = "AbstractVM exception";

AvmException::AvmException(void) : _msg(default_msg) {}

AvmException::AvmException(std::string const & msg) : _msg(msg) {}

const char * AvmException::what() const noexcept
{
	return _msg.c_str();
}

AvmException::~AvmException(void){}