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

#if defined(_WIN32) || defined(_WIN64)

#include "wrapperfacade_windows.h"
#include <iostream>
#include <errno.h>

namespace FeheConnection
{

	//client
	int Wrapperfacade_windows::connect(string ip, int port)
	{
		init_sockets();
		_sock = (int)::socket(AF_INET, SOCK_STREAM, 0);
	
		if(_sock == INVALID_SOCKET)
		{
			printf("Socket Initialisierung fehlgeschlagen");
			return -1;
		}
	
		memset(&_addr, 0, sizeof(SOCKADDR_IN));
	
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(port);
		_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	
		sock_code = ::connect(_sock, (SOCKADDR*)&_addr, sizeof(SOCKADDR));
	
		if(sock_code == SOCKET_ERROR)
		{
			printf("Socket Verbindung fehlgeschlagen");
			return -1;
		}
	
		return _sock;
	}
	
	//server
	int Wrapperfacade_windows::start(int port, int max_requests)
	{
		init_sockets();
		_sock = (int)::socket(AF_INET, SOCK_STREAM, 0);
	
		if (_sock == INVALID_SOCKET)
		{
			printf("Socket Initialisierung fehlgeschlagen");
			return -1;
		}
	
		int i = 1;
		::setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &i, sizeof(int));
		
		memset(&_addr, 0, sizeof(SOCKADDR_IN));
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(port);
		_addr.sin_addr.s_addr = ADDR_ANY;
		
		_addrlen = sizeof(struct sockaddr_in);
	
		sock_code = ::bind(_sock, (SOCKADDR*)&_addr, sizeof(SOCKADDR_IN));
	
		if (sock_code == SOCKET_ERROR)
		{
			printf("Fehler: bind, Fehlercode: %d\n", WSAGetLastError());
			return -1;
		}
	
		sock_code = ::listen(_sock, max_requests); // max_request = Max. Verbindungsanfragen an den Server
	
		if (sock_code == SOCKET_ERROR)
		{
			printf("Fehler: listen, Fehlercode: %d\n", WSAGetLastError());
			return -1; //todo: Hendrik, ist es Absicht hier 1 zurueckzugeben? nicht -1?
		}
		
		return _sock;
	}
	
	
	
	int Wrapperfacade_windows::accept(int socket)
	{
		int conn = ::accept(socket, (SOCKADDR*)&_addr, &_addrlen);
	
		if (conn == INVALID_SOCKET)
		{
			printf("Fehler: accept, Fehlercode: %d\n", WSAGetLastError());
			return -1;
		}
	
		return conn;
	}
	
	//both
	
	int Wrapperfacade_windows::send(int socket, char* data, int length)
	{
		return ::send((SOCKET)socket, data, length, 0);
	}
	
	char* Wrapperfacade_windows::recv(int socket, protocol* p)
	{
		int size_header = p->getHeaderSize();
		char* header = new char[size_header];
	
		sock_code = ::recv(socket, header, size_header, 0);
		
		if (sock_code == size_header)
	    {
			int size_content = p->getContentSize(header); //Wie gross ist der Content
			char* content = new char[size_header + size_content]; //Content-Daten
	
			sock_code = ::recv(socket, content+size_header, size_content, 0);
	
			if (sock_code == size_content )
			{
				//Alright lets build final message
				memcpy(content, header, size_header); //Lade header an Anfang von Nutzdaten
	
				delete[] header; //Wir brauchen den Header nicht mehr
				return content; //Jetzt haben wir ein Paket mit Header und Nutzdaten
			}
			else if (sock_code <= 0)
			{
				delete[] header;
				delete[] content;
				return p->encodeDisconnected();
			}
			else
			{
				//Wrong size ??
				//Protocol Handler will handle this
				delete[] header;
				delete[] content;
				return p->p->encodeWrongSize();
			}
	    }
		else if (sock_code <= 0)
		{
			delete[] header;
			return p->encodeDisconnected();
		}
		else
		{
			delete[] header;
			return p->encodeRawData();
			//irgendwas anderes geladen
		}
	}
	
	void Wrapperfacade_windows::disconnect(int socket)
	{
		closesocket((SOCKET)socket);
	}
	
	int Wrapperfacade_windows::init_sockets()
	{
		WSADATA wsa;
		sock_code = ::WSAStartup(MAKEWORD(2,0), &wsa);
	
		if (sock_code != NO_ERROR)
		{
			printf("WSAStartup fehlgeschlagen");
			return -1;
		}
	}
	
	// Destructor
	Wrapperfacade_windows::~Wrapperfacade_windows() {}
	
	// Konstructor
	Wrapperfacade_windows::Wrapperfacade_windows() {}
}

#endif

