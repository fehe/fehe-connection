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

#ifndef FEHE_CLIENT_H_
#define FEHE_CLIENT_H_

#include <string>
#include <stdio.h>

#ifdef __linux
	#include <netinet/in.h>
	#include <thread>
#elif defined(_WIN32) || defined(_WIN64)
	//#include <windows.h>
	#include <process.h>
#endif

#include "fehe_event_listener.h"
#include "protocol/protocol.h"
#include "wrapper/wrapperfacade.h"
#include "wrapper/wrapperfacade_linux.h"
#include "wrapper/wrapperfacade_windows.h"
#include "fehe_sender.h"
#include "fehe_dllmain.h"

using namespace std;

namespace FeheConnection
{

	class fehe_client
	{	
	public:
		DllExport fehe_client(fehe_event_listener* listener);
	
		//connect and send
		DllExport int sendCharArray(char* data, int length);
		DllExport int sendIntArray(int* data, int length);
		DllExport int sendFloatArray(float* data, int length);
		DllExport int sendStringArray(string* data, int length);
		DllExport int sendDoubleArray(double* data, int length);
		DllExport int sendBoolArray(bool* data, int length);
		DllExport int sendLongArray(long long* data, int length);
		DllExport int sendShortArray(short* data, int length);
		DllExport int connect(string ip, int port);
		DllExport void disconnect();
	
		//fallback
		DllExport void setFallBackServer(string ip, int port);
		DllExport int connectToFallBackServer();
	
		//destructor
		DllExport ~fehe_client();


	private:
		DllExport void recv();
		
		fehe_sender* _sender;
		
		Wrapperfacade* _wrapper_client;
		string _ip;
		int _port;
	
		//fallback
		string _fallbackIp;
		int _fallbackPort;
		bool _isFallback;
	
		bool _isReceive;
		
	#if defined(_WIN32) || defined(_WIN64)
		
		struct thread_parameter_client
		{
			fehe_client* client;
		};
	
		static DWORD WINAPI win_thread_recv(LPVOID params);
		HANDLE _receiveThread;
	#elif __linux
		thread* _receiveThread; //Thread der die ganze zeit accept abfragt;
	#endif
	
		fehe_event_listener* _listener;
		int _socket;
		protocol* _protocol;
	
	};
}





#endif /* FEHE_CLIENT_H_ */
