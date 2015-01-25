/* *************************************************************************
 *
 * Copyright (C) 2015 by fehe (F. Fröhling, H. Heimers)
 *
 * Contact: felix.froehling1@gmail.com
 *			hendriku31@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 ************************************************************************* */

#ifdef __linux
#ifndef WRAPPERFACADE_WRAPPERFACADE_LINUX_H_
#define WRAPPERFACADE_WRAPPERFACADE_LINUX_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "wrapperfacade.h"
#include "../protocol/protocol.h"

namespace FeheConnection
{

	class Wrapperfacade_linux : public Wrapperfacade{
	public:
		//client
		virtual int connect(string ip, int port); //returns socket
	
		//server
		virtual int start(int port, int max_requests); //returns socket
		virtual int accept(int socket); //returns socket
	
		//both
		virtual int send(int socket, char* data, int length);
		virtual char* recv(int socket, protocol* p);
		virtual void disconnect(int socket);
	
		//constructor
		Wrapperfacade_linux();
	
		//destructor
		virtual ~Wrapperfacade_linux();
	
	
	private:
		sockaddr_in _address;
		socklen_t _addrlen;
	
	};
}


#endif /* WRAPPERFACADE_WRAPPERFACADE_CLIENT_LINUX_H_ */
#endif /* LINUX */
