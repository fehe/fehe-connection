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

#ifndef PROTOCOL_PROTOCOL_H_
#define PROTOCOL_PROTOCOL_H_

#include "../wrapper/wrapperfacade.h"
#include "../fehe_event_listener.h"
//#include "fehe_server.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

namespace FeheConnection
{

	class fehe_server;
	//class Wrapperfacade;
	
	
	using namespace std;
	
	/*
	 * Protocol:
	 * Every packet header has 4 Byte:
	 * 3 Byte size, to tell the socket system how much content is to read
	 * 1 Byte content-type:
	 * 		- Bit 7 of Content-Type indicates whether the message is a system message or a message send by the user
	 * 			If it is 1 it is a system message
	 * 			If it is 0 it is a user message
	 * 		- The other 7 Bits indicates the type of the message:
	 * 			- If bit 7 is 1 (or content-type >= 128)
	 * 				128: not used
	 * 				129: opponent disconnected
	 * 				130: client will disconnect clearly
	 * 				131: server informs that it will end
	 * 				132: client reply acknowledge to server ending. Server has to wait for every connected client to receive acknowledge. Otherwise clients could try to reach the backup server
	 * 				133: version request
	 * 				134: version reply
	 * 				135: RAW-Data
	 * 				136: Wrong Size
	 * 			- If bit 7 is 0 (or content-type >= 0 but < 128
	 * 				0: not used
	 * 				1: char
	 * 				2: int
	 * 				3: string
	 * 				4: float
	 * 				5: double
	 * 				6: bool
	 * 				7: long
	 * 				8: short
	 *
	 * 				21: unsigned char
	 * 				22: unsigned int
	 * 				23: unsigned long
	 * 				24: unsigned short
	 *
	 * 				30: not used
	 * 				31: char*
	 * 				32: int*
	 * 				33:	string*
	 * 				34: float*
	 * 				35: double*
	 * 				36: bool*
	 * 				37: long*
	 * 				38: short*
	 *
	 * 				51: unsigned char*
	 * 				52: unsigned int*
	 * 				53: unsigned long*
	 * 				54: unsigned short*
	 *
	 *
	 *	Please note that transmit pointers does NOT works like copy & restore. It is just thought for tramsitting arrays of data-types.
	 *	todo: Maybe we should convert the transmitted data to network data format to avoid issues due 32 or 64 bit systems, e.G. 4/8 byte integers
	 *
	 */
	
	class protocol{
	public:
		int getHeaderSize();
		int getContentSize(char* header); //header with size from headersize
		//bool decodeMessageOnServer(char* message, int socket, fehe_server_event_listener* listener); //returns if the connection keeps alive
		//bool decodeMessageOnClient(char* message, fehe_client_event_listener* listener); //returns if the connection keeps alive
		bool decodeMessage(char* message, int socket, fehe_event_listener* listener,  bool* acknowledge);
		bool decodeMessage(char* message, int socket, fehe_event_listener* listener,  bool* acknowledge, bool* isReceive); //for the server
		//bool decodeMessage(char* message, int socket, fehe_event_listener* listener, bool* acknowledge, bool* isClientReceive); // for the server
		//bool decodeMessage(char* message, int socket, fehe_event_listener* listener)
	
	
		//system
		char* encodeDisconnected();
		char* encodeClientWillDisconnect(int* packet_size);
		char* encodeServerWillDisconnect(int* packet_size);
		char* encodeServerDisconnectAcknowledge(int* packet_size);
		char* encodeVersionRequest(int* packet_size); //one byte payload with version number
		char* encodeVersionReply(int* packet_size, int oponnentVersion); //one / two byte payload. first with state: 1 for ok, 0 for not ok. if not ok a second byte with version number is applied
		char* encodeRawData();
		char* encodeWrongSize();
	
		//user
		char* encodeCharPointerPacket(char* data, int length, int* packet_size);
		char* encodeIntPointerPacket(int* data, int length, int* packet_size);
		char* encodeStringPointerPacket(string* data, int length, int* packet_size);
		char* encodeFloatPointerPacket(float* data, int length, int* packet_size);
		char* encodeDoublePointerPacket(double* data, int length, int* packet_size);
		char* encodeBoolPointerPacket(bool* data, int length, int* packet_size);
		char* encodeLongPointerPacket(long long* data, int length, int* packet_size);
		char* encodeShortPointerPacket(short* data, int length, int* packet_size);
	
		protocol(Wrapperfacade* wrapper);
	private:
		Wrapperfacade* _wrapper;
		bool _expectDisconnect;
		char* createFieldWithHeader(int size, char type, int* packet_size);
		int compareSize(char des); // returns 0 if source and we are equal. returns 1 if source is lower. returns 2 if source is greater
	};
}



#endif /* PROTOCOL_PROTOCOL_H_ */
