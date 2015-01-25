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

#include "fehe_server.h"
#include <iostream>

using namespace std;

namespace FeheConnection
{

	fehe_server::fehe_server(fehe_event_listener* listener, int port)
	{
	#ifdef __linux
		_wrapper_server = new Wrapperfacade_linux();
	#elif defined(_WIN32) || defined(_WIN64)
		_wrapper_server = new Wrapperfacade_windows();
	#endif
		_server_socket = _wrapper_server->start(port, 1000);
		_isAccept = true;
		
		#if defined(_WIN32) || defined(_WIN64)
			thread_parameter_server* tp = new thread_parameter_server();
			tp->server = this;
			
			_accept_thread = CreateThread(NULL, 0, &fehe_server::win_thread_accept, tp, 0, 0);
		#elif __linux
			_accept_thread = new thread(&fehe_server::accept_loop, this);
		#endif
		
		_protocol = new protocol(_wrapper_server);
		_sender = new fehe_sender(_protocol, _wrapper_server);
		_listener = listener;
	}
	
	#if defined(_WIN32) || defined(_WIN64)
	DWORD WINAPI fehe_server::win_thread_accept(LPVOID params)
	{
		thread_parameter_server *tp = (thread_parameter_server*)params;
		tp->server->accept_loop();
		
		delete tp; // Wird vielleicht nicht aufgerufen!?
	}
	#endif
	
	void fehe_server::accept_loop()
	{
		while(_isAccept)
		{
			int socket = _wrapper_server->accept(_server_socket);
			if(socket > 0)
			{
				_sockets.push_back(socket);
				protocol* p = new protocol(_wrapper_server);
	
				#if defined(_WIN32) || defined(_WIN64)
					thread_parameter_server *tp = new thread_parameter_server();
				
					tp->p = p;
					tp->server = this;
					tp->socket = socket;
					
					CreateThread(NULL, 0, &fehe_server::win_thread_recv, tp, 0, 0);
	
				#elif __linux
					new thread(&fehe_server::receive_loop, this, socket, p);
				#endif
				
				if(_listener != 0)
					_listener->onClientConnect(socket);
			}
		}
	}
	
	fehe_sender* fehe_server::getSender()
	{
		return _sender;
	}
	
	#if defined(_WIN32) || defined(_WIN64)
	DWORD WINAPI fehe_server::win_thread_recv(LPVOID params)
	{
		protocol* p;
		int socket;
		
		thread_parameter_server *tp = (thread_parameter_server*)params;
		
		p = tp->p;
		socket = tp->socket;
		
		tp->server->receive_loop(socket, p);
		
		delete tp; // Wird vielleicht nicht aufgerufen!?
	}
	#endif
	
	void fehe_server::receive_loop(const int socket, protocol* p){
		bool active;
		bool isAcknowledge = false;
		do{
			char* block = _wrapper_server->recv(socket, p);
			active = p->decodeMessage(block, socket, _listener, &isAcknowledge);
		}while(active && _isAccept); // solange client nicht getrennt und server aktiv
	
		for(unsigned int count = 0; count < _sockets.size(); count++)
			if(_sockets.at(count) == socket){
				_sockets.erase(_sockets.begin() + count); //delete the according socket from our list
				break;
			}
	
		if(isAcknowledge){ //when we have shutdown the server, we already had deleted the protocol
			if(_sockets.size() == 0){ //when we are the last client we can safety shutdown the server
				_listener->safetyServerEnd();
			}
		}
	
		delete p; //genutztes protokoll wieder freigeben
	}
	
	void fehe_server::send(int socket, char* data, int length){
		int packet_size;
		char* packet = _protocol->encodeCharPointerPacket(data, length, &packet_size);
		_wrapper_server->send(socket, packet, packet_size);
	}
	
	
	
	void fehe_server::endServer(){
		//todo: to be clear we would have to end the accept thread, but this could be heavy dangerous. Hendrik hast du eine Idee?
		_isAccept = false;
		int packet_size;
		if(_sockets.size() == 0){
			//no need to send end message
			_listener->safetyServerEnd();
		}
		else{
			//sende an jeden client den disconnect request. Wir muessen auf einen acknowledge warten
			char* endMessage = _protocol->encodeServerWillDisconnect(&packet_size);
			for(unsigned int x = 0; x < _sockets.size(); x++){
				_wrapper_server->send(_sockets.at(x), endMessage, packet_size);
			}
			delete[] endMessage;
		}
	
	
	}
	
	fehe_server::~fehe_server(){
		if(_sockets.size() > 0){
			//todo: server is not shutdown. We have to inform the user
		}
		else{
			_wrapper_server->disconnect(_server_socket);
			delete _wrapper_server;
			delete _protocol;
			delete _sender;
		}
	}
}

