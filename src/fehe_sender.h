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
 
#ifndef FEHE_SENDER_H_
#define FEHE_SENDER_H_

#include <string>

#include "protocol/protocol.h"
#include "wrapper/wrapperfacade.h"

//class protocol;
//class Wrapperfacade;

using namespace std;

namespace FeheConnection
{

	class fehe_sender{
	public:
		//server
		virtual int sendCharArray(int socket, char* data, int length);
		virtual int sendIntArray(int socket, int* data, int length);
		virtual int sendFloatArray(int socket, float* data, int length);
		virtual int sendStringArray(int socket, string* data, int length);
		virtual int sendDoubleArray(int socket, double* data, int length);
		virtual int sendBoolArray(int socket, bool* data, int length);
		virtual int sendLongArray(int socket, long long* data, int length);
		virtual int sendShortArray(int socket, short* data, int length);
	
		//constructor
		fehe_sender(protocol* p, Wrapperfacade* wrapper);
	
		//destructor
		virtual ~fehe_sender();
	
	private:
		//int _socket;
		protocol* _protocol;
		Wrapperfacade* _wrapper;
	};
}

#endif
