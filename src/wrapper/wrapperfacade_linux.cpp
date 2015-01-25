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

#include "wrapperfacade_linux.h"
#include <iostream>

namespace FeheConnection
{

	//client
	
	int Wrapperfacade_linux::connect(string ip, int port)
	{
		int socket;
		socket = ::socket(AF_INET, SOCK_STREAM, 0);
	
		if ( socket == -1){
			return -1; //todo: korrekte fehlerbehandlung?
		}
	
		_address.sin_family = AF_INET ;
		_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
		_address.sin_port = :: htons(port);
	
		int len = sizeof (_address);
		int flag = :: connect ( socket , ( struct sockaddr *) & _address , len );
		if ( flag == -1){
			return -1;//todo: korrekte fehlerbehandlung?
		}
		
		return socket;
	}
	
	
	//server
	
	int Wrapperfacade_linux::start(int port, int max_requests)
	{
		int socket;
	
		socket = :: socket ( AF_INET , SOCK_STREAM , 0);
		if ( socket == -1){
			return -1; //todo: Fehlerbehandlung korrekt? (hoehere Schichten)
		}
	
		int i = 1;
		::setsockopt(socket , SOL_SOCKET , SO_REUSEADDR , &i , sizeof ( i ));
		// bind server port
		_address . sin_family = AF_INET ;
		_address . sin_addr . s_addr = INADDR_ANY ;
		_address . sin_port = :: htons ( port );
	
		int r = :: bind ( socket , ( struct sockaddr *) & _address , sizeof ( _address ));
		if( r == -1){
			return -1; //todo: Fehlerbehandlung korrekt? (hoehere Schichten)
		}
	
		::listen(socket, max_requests); // max_request = Max. Verbindungsanfragen an den Server
		_addrlen = sizeof(struct sockaddr_in);
	
		return socket;
	}
	
	
	int Wrapperfacade_linux::accept(int socket){
		// waiting for incoming requests
		//cout << " waiting for incoming requests ... " << endl ;
		int conn = :: accept(socket , (struct sockaddr *) & _address , & _addrlen);
		if ( conn == -1){
			return -1; //todo: wir muessen pruefen ob der fehlerfall korrekt abgedeckt ist. Theorie reicht, denn das hier Fehler auftreten ist sehr unwahrscheinlich und wie will man welche provozieren?
		}
		return conn ;
	}
	
	
	//both
	
	
	int Wrapperfacade_linux::send(int socket, char* data, int length){
		return ::write(socket , data , length);
	}
	
	char* Wrapperfacade_linux::recv(int socket, protocol* p){
		int size_header = p->getHeaderSize();
	
		char* header = new char[size_header];
		int read_header = ::read(socket, header, size_header);
		
		if(read_header == size_header){
	
			int size_content = p->getContentSize(header); //Wie gross ist der Content
			int read_content = 0; //Wieviel haben wir gelesen
			char* content = new char[size_header + size_content]; //Content-Daten
			read_content = ::read(socket, content+size_header, size_content); //Schreibe Daten in content rein. Lasse Platz fuer den Header
			if(read_content == size_content ){
				//Alright lets build final message
				memcpy(content, header, size_header); //Lade header an Anfang von Nutzdaten
				delete header; //Wir brauchen den Header nicht mehr
				return content; //Jetzt haben wir ein Paket mit Header und Nutzdaten
			}
			else if(read_content <= 0){
				delete[] content;
				return p->encodeDisconnected(); //opponent is disconnected
			}
			else{
				//todo: falsche groesse empfangen. Hendrik was machen wir hiermit?
				delete[] content;
				delete[] header;
				return p->encodeWrongSize();
			}
		}
		else if(read_header <= 0){
			delete[] header;
			return p->encodeDisconnected();
		}
		else{
			//todo: Wir haben hier irgendwelche RAW-Daten empfangen, was nicht von unserer library stammt. Hendrik was machen wir damit?
			delete[] header;
			return p->encodeRawData();
		}
		//return 0; //todo: Hendrik was machen wir im default case?
	}
	
	void Wrapperfacade_linux::disconnect(int socket){
		::close(socket);
	}
	
	Wrapperfacade_linux::~Wrapperfacade_linux(){}
	
	Wrapperfacade_linux::Wrapperfacade_linux(){}
}

#endif /* LINUX */

