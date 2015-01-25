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

#ifndef WRAPPERFACADE_WRAPPERFACADE_H_
#define WRAPPERFACADE_WRAPPERFACADE_H_

#include <string>
//#include "protocol/protocol.h"

namespace FeheConnection
{
	class protocol;
	
	using namespace std;
	
	class Wrapperfacade{
	public:
		//client
		virtual int connect(string ip, int port) = 0; //returns socket
	
		//server
		virtual int start(int port, int max_requests) = 0; //returns socket
		virtual int accept(int socket) = 0; //returns socket
	
		//both
		virtual int send(int socket, char* data, int length) = 0; //todo: Wir sollten als Rueckgabewert zurueckgeben, ob das sendne erfolgreich war, z.B. die Anzahl der geschriebenen Bytes und im Fehlerfall -1...
		virtual char* recv(int socket, protocol* p) = 0;
		virtual void disconnect(int socket) = 0;
	
		//destructor
		virtual ~Wrapperfacade();
	};
}


#endif /* WRAPPERFACADE_WRAPPERFACADE_H_ */
