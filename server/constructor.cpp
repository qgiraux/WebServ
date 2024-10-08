/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qgiraux <qgiraux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:49:28 by qgiraux           #+#    #+#             */
/*   Updated: 2024/10/10 14:53:43 by qgiraux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


Server::Server(std::list<ConfigServer> servers) : servers(servers)
{
    list_size = servers.size();
    tmpfindex = 0;
    std::vector<int> vect(list_size);
    std::vector<struct sockaddr_in> vect2(list_size);
    server_fd = vect;
    address = vect2;
    nfds = 0;
    maxBodySize = BUFFER_SIZE;
    set_mimeList();
    set_errorList();

}