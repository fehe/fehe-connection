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

#include "fehe_client.h"
#include <iostream>

using namespace std;

namespace FeheConnection
{

	fehe_client::fehe_client(fehe_event_listener* listener)
	{
	#ifdef __linux
		_wrapper_client = new Wrapperfacade_linux();
	#elif defined(_WIN32) || defined(_WIN64)
		_wrapper_client = new Wrapperfacade_windows();
	#endif
		_listener = listener;
		_protocol = new protocol(_wrapper_client);
		_sender = new fehe_sender(_protocol, _wrapper_client);
		_isFallback = false;
	}
	
	int fehe_client::connect(string ip, int port){
		_socket =  _wrapper_client->connect(ip, port);
		if(_socket >= 0)
		{
			this->_isReceive = true;
			this->_ip = ip;
			this->_port = port;
			
			#if defined(_WIN32) || defined(_WIN64)
				thread_parameter_client* tp = new thread_parameter_client();
				
				tp->client = this;
			
				_receiveThread = CreateThread(NULL, 0, &fehe_client::win_thread_recv, tp, 0, 0);
				//_receiveThread = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, , this, 0, 0)); 
			#elif __linux
				_receiveThread = new thread(&fehe_client::recv, this);
			#endif


			int packet_size;
			char* versionPacket = _protocol->encodeVersionRequest(&packet_size);
			_wrapper_client->send(_socket, versionPacket, packet_size);
			delete[] versionPacket;
		}
	
		return _socket;
	}
	
	
	void fehe_client::setFallBackServer(string ip, int port){
		_fallbackIp = ip;
		_fallbackPort = port;
	}
	
	int fehe_client::connectToFallBackServer(){
		if(_isReceive){
			//cout << "isReceive aktiv..." << endl;
			return -1;
		}
		else if(_isFallback){
			return -2; //-2 means we have no more fallback servers
		}
		else{
			//cout << "connect" << endl;
			_isFallback = true;
			_ip = _fallbackIp;
			_port = _fallbackPort;
			return connect(_ip, _port);
		}
	}
	
	#if defined(_WIN32) || defined(_WIN64)
	DWORD WINAPI fehe_client::win_thread_recv(LPVOID params)
	{
		thread_parameter_client *tp = (thread_parameter_client*)params;
		tp->client->recv();
		
		delete tp; // Wird vielleicht nicht aufgerufen!?
	}
	#endif
	
	void fehe_client::recv()
	{
		bool active;
		bool wantServerEnd = false;
		do
		{
			char* block = _wrapper_client->recv(_socket, _protocol);
			active = _protocol->decodeMessage(block, _socket, _listener, &wantServerEnd, &_isReceive);
		} while(active);
	
		_isReceive = false;
	}
	
	int fehe_client::sendCharArray(char* data, int length){
		return _sender->sendCharArray(_socket, data, length);
	}
	
	int fehe_client::sendIntArray(int* data, int length){
		return _sender->sendIntArray(_socket, data, length);
	}
	
	int fehe_client::sendStringArray(string* data, int length){
		return _sender->sendStringArray(_socket, data, length);
	}
	
	int fehe_client::sendFloatArray(float* data, int length){
		return _sender->sendFloatArray(_socket, data, length);
	}
	
	int fehe_client::sendDoubleArray(double* data, int length){
		return _sender->sendDoubleArray(_socket, data, length);
	}
	int fehe_client::sendBoolArray(bool* data, int length){
		return _sender->sendBoolArray(_socket, data, length);
	}
	int fehe_client::sendLongArray(long long* data, int length){
		return _sender->sendLongArray(_socket, data, length);
	}
	int fehe_client::sendShortArray(short* data, int length){
		return _sender->sendShortArray(_socket, data, length);
	}
	
	
	void fehe_client::disconnect(){
		int packet_size;
		char* packet = _protocol->encodeClientWillDisconnect(&packet_size);
		_wrapper_client->send(_socket, packet, packet_size);
		//Sleep(100);
		_isReceive = false;
		_wrapper_client->disconnect(_socket);
	}
	
	fehe_client::~fehe_client()
	{	
		//todo: check if connected, if yes disconnect
		delete _sender;
		delete _wrapper_client;
		delete _protocol;
	
	}
}


