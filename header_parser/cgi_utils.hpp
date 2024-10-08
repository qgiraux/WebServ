/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:17:29 by nrea              #+#    #+#             */
/*   Updated: 2024/10/08 11:30:19 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_UTILS_HPP
#define CGI_UTILS_HPP

#include <map>
#include <iostream>
#include <string>
#include <vector>


char **MapToEnv(std::map<std::string,std::string> const & map);
void freeEnv(char **env);
std::string	detect_cgi(std::string const &uri);
void stripPathInfo(std::string &uri, std::string &path_info, std::string cgi);
std::string cgi_convert(std::string header_attribute);
#endif
