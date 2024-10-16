/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_get.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrea <nrea@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:27:16 by nrea              #+#    #+#             */
/*   Updated: 2024/10/11 10:45:16 by nrea             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headerParser.hpp"


/*===================================================================================================*/

header_infos handleFileErrror
(int error, header_infos &response, ConfigServer  * config,int locationIndex)
{
	if (ENOENT == error)
	{
		{
			std::ostringstream oss;
			oss <<"[handle_get]	The ressource "<<response.ressourcePath <<" does not exist";
			webservLogger.log(LVL_DEBUG, oss);
		}
		return response_error(HTTP_STATUS_NOT_FOUND, config, locationIndex);
	}
	else if (EACCES == error)
	{
		{
			std::ostringstream oss;
			oss <<"[handle_get]	The ressource "<<response.ressourcePath <<" is forbidden";
			webservLogger.log(LVL_DEBUG, oss);
		}
		return  response_error(HTTP_STATUS_FORBIDDEN, config, locationIndex);
	}

	{
		std::ostringstream oss;
		oss <<"[handle_get]	An internal error has occured";
		webservLogger.log(LVL_DEBUG, oss);
	}
	return  response_error(HTTP_STATUS_INTERNAL_SERVER_ERROR, config, locationIndex);
}



header_infos Server::serve_regular_file(header_infos &response,
ConfigServer  * config,int locationIndex,std::map<std::string, std::string> header_attributes)
{
	struct stat stat_buf;
	int ret;

	ret = stat(response.ressourcePath.c_str(),  &stat_buf);
	if (ret != 0)
		return handleFileErrror(errno,response,config, locationIndex);
	//on checke si le fichier est regulier et accessible en lecture
	//sinon on retourne erreur 403
	if (!S_ISREG(stat_buf.st_mode) || access(response.ressourcePath.c_str(), R_OK))
	{
		{
			std::ostringstream oss;
			oss <<"[serve_regular_file]	"<<response.ressourcePath <<" is not a readable regular file";
			webservLogger.log(LVL_DEBUG, oss);
		}
		return response_error(HTTP_STATUS_FORBIDDEN, config, locationIndex);
	}

	//ON Ddetermine sa taille
	response.bodySize = getFileSize(response.ressourcePath);

	//Si la taille est trop grande on passe en chunked; //TODO
	// --------------------------------------

	if (response.bodySize > CHUNK_SIZE)
		response.chunked = true;
	else
		response.chunked = false;

	response.returnCode = 200;
	response.locationIndex = locationIndex;
	response.configServer = config;

	// response pour initialiser ces champs vides
	response.interpreterPath = "";
	response.fd_ressource = 0;
	response.cgi_pid = 0;

	// on DETERMINE LE  MIME TYPE DE LA RESSOURCE ==> a deplacer
	response.contentType = get_mime_type(response.ressourcePath);

	//On verifie que le content-type est autorise
	if (!matchAcceptContentTypes(response.contentType, header_attributes["Accept"]))
		return response_error(HTTP_STATUS_NOT_ACCEPTABLE, config, locationIndex);
	if (header_attributes.find("Connection") != header_attributes.end() &&
	header_attributes["Connection"] == "close")
		response.keepAlive = false;
	else
		response.keepAlive = true;

	return response;
}



header_infos Server::handle_get(header_infos &response,
 ConfigServer  * config,int locationIndex,std::map<std::string, std::string> &header_attributes)
{
	struct stat stat_buf;
	int ret;

// On cherche a detecter un eventuel cgi
	std::string cgi = detect_cgi(header_attributes["URI"]);
	if (cgi != "") // une extension cgi a ete detecte das l'uri
	{
		{
			std::ostringstream oss;
			oss <<"[handle_get]	cgi file detected";
			webservLogger.log(LVL_DEBUG, oss);
		}
		return handle_get_cgi(response,cgi,config, locationIndex, header_attributes);
	}

	response.toDo = GET;
// test access
	ret = stat(response.ressourcePath.c_str(),  &stat_buf);
	if (ret != 0)
		return handleFileErrror(errno,response,config, locationIndex);
//Si la ressource est un repertoire
	if (S_ISDIR(stat_buf.st_mode))
		return handle_dir(response, config, locationIndex, header_attributes);
	return serve_regular_file(response, config, locationIndex, header_attributes);
}

