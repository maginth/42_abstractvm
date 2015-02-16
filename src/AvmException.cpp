/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AvmException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguinin <mguinin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/13 11:46:39 by mguinin           #+#    #+#             */
/*   Updated: 2015/02/13 11:53:02 by mguinin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <AvmException.hpp>

AvmException::AvmException(void) _msg("AbstractVM exception") {}

AvmException(std::string const & msg) : _msg(msg) {}

const char * AvmException::what() const noexcept
{
	return &_msg;
}