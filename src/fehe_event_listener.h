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
 
#ifndef FEHE_EVENT_LISTENER_H_
#define FEHE_EVENT_LISTENER_H_

#include <string>
#include "fehe_dllmain.h"

using namespace std;

namespace FeheConnection
{

	class DllExport fehe_event_listener{
	public:
	
		//receive
		virtual DllExport void onReceiveCharArray(int socket, char* data, int length) = 0;
		virtual DllExport void onReceiveIntArray(int socket, int* data, int length) = 0;
		virtual DllExport void onReceiveStringArray(int socket, string* data, int length) = 0;
		virtual DllExport void onReceiveFloatArray(int socket, float* data, int length) = 0;
		virtual DllExport void onReceiveDoubleArray(int socket, double* data, int length) = 0;
		virtual DllExport void onReceiveBoolArray(int socket, bool* data, int length) = 0;
		virtual DllExport void onReceiveLongArray(int socket, long long* data, int length) = 0;
		virtual DllExport void onReceiveShortArray(int socket, short* data, int length) = 0;
	
		//events
		virtual DllExport void onOpponentDisconnect(int socket) = 0;
		virtual DllExport void onOpponentDisconnectUnexspected(int socket) = 0;
		virtual DllExport void onVersionErrorResponse(int ownVersion, int opponentversion) = 0; //will be called if version check was not successfull
	
		//server
		virtual DllExport void onClientConnect(int socket) = 0;
		virtual DllExport void safetyServerEnd() = 0;
	
		virtual DllExport ~fehe_event_listener(){};
	};
}



#endif /* FEHE_EVENT_LISTENER_H_ */
