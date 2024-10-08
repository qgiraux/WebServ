/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headerParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:44:25 by nrea              #+#    #+#             */
/*   Updated: 2024/10/10 10:45:01 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_PARSER_HPP
#define HEADER_PARSER_HPP

#include <list>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <exception>
#include "ConfigServer.hpp"
#include "Server.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include "http_errors.hpp"
#include <unistd.h>
#include "Logger.hpp"
#include "FileInfos.hpp"
#include "cgi_utils.hpp"

#define RST		"\033[0m"
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"


extern Logger webservLogger;


header_infos response_error(std::string error_code, ConfigServer  * config,int locationIndex);
header_infos response_autoindex(ConfigServer *config, int locationIndex, header_infos response);
header_infos response_redirect(std::string  &return_code, std::string  &redir_url, std::map<std::string, std::string> &header_attributes);

header_infos handle_get_cgi(header_infos & response, std::string cgi, ConfigServer *config,
int locationIndex, std::map<std::string,std::string> &header_attributes);
header_infos handle_post_cgi(header_infos & response, std::string cgi,
ConfigServer *config,int locationIndex, std::map<std::string,std::string> &header_attributes);
bool contains_only_numeric(std::string str);
long getFileSize(std::string filename);
std::string getFileExtension(std::string uri);
std::string getParentDir(std::string uri);

bool contains_forbbiden(std::string const &uri);
std::string  url_decode(std::string const &uri);
std::vector<std::string> splitString(const std::string& str, std::string delimiter);

header_infos handle_cgi(header_infos & response, std::string cgi, ConfigServer *config,int locationIndex, std::map<std::string,std::string> &header_attributes);

bool matchAcceptContentTypes(std::string file_content_type, std::string accepted_types);
bool matchAcceptServerContentTypes(std::string file_content_type, std::map<std::string,std::string> mime_list);

std::string str_todo(int todo);
#endif
