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

#ifndef FEHE_SERVER_H_
#define FEHE_SERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <vector>

#ifdef __linux
	#include <netinet/in.h>
	#include <thread>
#elif defined(_WIN32) || defined(_WIN64)
	//#include <windows.h>
	#include <process.h>
#endif

#include "protocol/protocol.h"
#include "wrapper/wrapperfacade.h"
#include "wrapper/wrapperfacade_linux.h"
#include "wrapper/wrapperfacade_windows.h"
#include "fehe_event_listener.h"
#include "fehe_sender.h"
#include "fehe_dllmain.h"

using namespace std;

/*
 * Wichtige Anmerkung:
 * Die Methode endServer() muss auf jeden Fall aufgerufen werden bevor der server mittels delete geloescht wird. Dies hat einen einfachen Grund:
 * Das System ist ausfallsicher, d.h. geht die Verbindung zu dem Server verloren versucht der client einen fallback-Server zu erreichen.
 * Wird der Server jedoch regulaer heruntergefahren so muss eine Meldung an alle clients ausgegeben werden. Um sicher zu gehen, dass diese die empfangen haben
 * gibt senden diese einen acknowledge. Anschliessend trennt der Server die Verbindung. Endserver initiert diese Requests.
 * Sind alle Clients sauber getrennt, so wird die entsprechende Methode des Listeners aufgerufen. Dort kann der Server sicher geloescht werden
 */

namespace FeheConnection
{
	class fehe_server
	{
	public:
		DllExport fehe_server(fehe_event_listener* listener, int port);
		DllExport void send(int socket, char* data, int length);
		DllExport void endServer();
		DllExport ~fehe_server();
		DllExport fehe_sender* getSender();

	
	private:
		DllExport void accept_loop();
		DllExport void receive_loop(int socket,  protocol* p);
		
	#if defined(_WIN32) || defined(_WIN64)
		
		struct thread_parameter_server
		{
			fehe_server* server;
			protocol* p;
			int socket;
		}; 
		
		static DWORD WINAPI win_thread_accept(LPVOID params);
		static DWORD WINAPI win_thread_recv(LPVOID params);
		
		HANDLE _accept_thread;
	#elif __linux
		thread* _accept_thread; //Thread der die ganze zeit accept abfragt;
	#endif
		
		fehe_sender* _sender;
		Wrapperfacade* _wrapper_server; //Unsere Wrapperfacade
		int _server_socket;
		vector<int> _sockets; //Alle Sockets des Servers	
		bool _isAccept; //Solange diese Variable true ist wird accepted laufen
		fehe_event_listener* _listener;
		protocol* _protocol;
	};
}



#endif /* FEHE_SERVER_H_ */
