/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_error.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:13:31 by nrea              #+#    #+#             */
/*   Updated: 2024/10/16 13:58:05 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headerParser.hpp"

/* returns a header_infos set to the appropriate error*/
header_infos response_error(std::string error_code, ConfigServer  * config,int locationIndex)
{
	header_infos response;
	response.toDo = ERROR;
	response.returnCode = std::atoi(error_code.c_str());//error_code;
	response.contentType = "text/html";
	response.ressourcePath =  config->getDirectiveOutput(locationIndex, "error_page", error_code);
	if (response.ressourcePath.size())
		response.bodySize = getFileSize(response.ressourcePath.c_str());
	else
		response.bodySize = 0;
	response.configServer = config;
	response.locationIndex = locationIndex;
	response.chunked = false;

	if ( error_code == HTTP_STATUS_BAD_REQUEST || error_code == HTTP_STATUS_INTERNAL_SERVER_ERROR ||
	error_code == HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED || error_code == HTTP_STATUS_PAYLOAD_TOO_LARGE )
		response.keepAlive = false;
	else
		response.keepAlive = true;
	return response;
}
