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

#include "fehe_sender.h"

namespace FeheConnection
{

	int fehe_sender::sendCharArray(int socket, char* data, int length){
		int packet_size;
		char* packet = _protocol->encodeCharPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	
	int fehe_sender::sendIntArray(int socket, int* data, int length){
		int packet_size;
		char* packet = _protocol->encodeIntPointerPacket(data, length * sizeof(int), &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	
	int fehe_sender::sendStringArray(int socket, string* data, int length){
		int packet_size;
		char* packet = _protocol->encodeStringPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	
	int fehe_sender::sendFloatArray(int socket, float* data, int length){
		int packet_size;
		char* packet = _protocol->encodeFloatPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	
	int fehe_sender::sendDoubleArray(int socket, double* data, int length){
		int packet_size;
		char* packet = _protocol->encodeDoublePointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	int fehe_sender::sendBoolArray(int socket, bool* data, int length){
		int packet_size;
		char* packet = _protocol->encodeBoolPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	int fehe_sender::sendLongArray(int socket, long long* data, int length){
		int packet_size;
		char* packet = _protocol->encodeLongPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;;
	}
	int fehe_sender::sendShortArray(int socket, short* data, int length){
		int packet_size;
		char* packet = _protocol->encodeShortPointerPacket(data, length, &packet_size);
		int written = _wrapper->send(socket, packet, packet_size);
		delete[] packet;
		return written;
	}
	
	fehe_sender::fehe_sender(protocol* p, Wrapperfacade* wrapper){
		_protocol = p;
		_wrapper = wrapper;
	}
	
	fehe_sender::~fehe_sender(){}
}




