/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigBlock.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerperez <jerperez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:29:48 by jerperez          #+#    #+#             */
/*   Updated: 2024/09/19 16:05:55 by jerperez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "ConfigBlock.hpp"
#include "config_error_constants.h"
#include "Directive.hpp"

int	ConfigBlock::_addDirective_unique(\
	const std::string &name,\
	const Directive::args_t &parameters)
{
	if (parameters.empty())
		return CF_ERRDNOPARAM;
	this->_directive_parameters[name] = parameters;
	return 0;
}

int	ConfigBlock::_addDirective_io(\
	const std::string &name,\
	Directive::args_t parameters)
{
	if (parameters.empty())
		return CF_ERRDNOPARAM;
	const std::string output = parameters.back();

	parameters.pop_back();
	if (parameters.empty())
		return CF_ERRDNOPARAM;
	const Directive::args_t::const_iterator	it_end = parameters.end();

	for (Directive::args_t::const_iterator it = parameters.begin(); \
		it_end != it; \
		++it)
			this->_io_directive_parameters[name][*it] = output;
	return 0;
}

int		ConfigBlock::_addDirective(Directive* directive)
{
	Directive::args_t	args_copy = directive->getArgs();

	if (args_copy.empty())
		return (0);
	std::string			name = args_copy.front();

	if (name.empty())
		return CF_ERRDNONAME;
	else if (!this->_knownDirective(name))
		return CF_ERRDUNKNOWN;
	args_copy.pop_front();
	if ("cgi" == name || "error_page" == name)
		return this->_addDirective_io(name, args_copy);
	else
		return this->_addDirective_unique(name, args_copy);
	return 0;
}
