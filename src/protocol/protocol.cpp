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

#include "protocol.h"

#define HEADER_SIZE_LENGTH 3
#define HEADER_SIZE_CONTENT_DES 1
#define VERSION 0x01

namespace FeheConnection
{

	int protocol::getHeaderSize(){
		return HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES; //Laenge + Contentdes = Headerlaenge
	}
	
	int protocol::getContentSize(char* header){
		int result = 0;
	
		char* copy = new char[HEADER_SIZE_LENGTH];
		memcpy(copy, header, HEADER_SIZE_LENGTH);
	
		result = (copy[HEADER_SIZE_LENGTH -1]<<0) | (copy[HEADER_SIZE_LENGTH - 2 ]<<8) | (copy[HEADER_SIZE_LENGTH - 3 ]<<16);
		delete[] copy;
	
		return result;
	}
	
	int protocol::compareSize(char des){
		int opponent;
		int we = sizeof(int);
		int is64 = (des & 0x40) >> 6;
	
		if(is64)
			opponent = 8;
		else
			opponent = 4;
	
		if(we == opponent)
			return 0;
		if(we > opponent){
			return 1;
		}
		if(we < opponent){
			return 2;
		}
		return 0;
	}
	
	bool protocol::decodeMessage(char* message, int socket, fehe_event_listener* listener,  bool* acknowledge){
		bool dummy;
		return decodeMessage(message, socket, listener, acknowledge, &dummy);
	}
	
	bool protocol::decodeMessage(char* message, int socket, fehe_event_listener* listener, bool* acknowledge, bool* isReceive){
		int content_size = getContentSize(message);
	
		(*acknowledge) = false;
		(*isReceive) = true;
	
			char* content = message + getHeaderSize();
			unsigned char content_des = *(message + HEADER_SIZE_LENGTH);
			bool system_message = (content_des & 128 ) >> 7;
	
			if(system_message){
				//System message
				switch(content_des){
				case 129:{
					(*isReceive) = false;
					if(_expectDisconnect)
						listener->onOpponentDisconnect(socket);
					else
						listener->onOpponentDisconnectUnexspected(socket);
					delete message;
	
					return false;
				}
				case 130:{ //client wants to end connection
					_expectDisconnect = true;
					return true;
				}
				case 131:{ //server wants to disconnect
					_expectDisconnect = true;
					int packet_size;
					char* answer = encodeServerDisconnectAcknowledge(&packet_size);
					_wrapper->send(socket, answer, packet_size);
					delete[] answer;
					return true;
				}
				case 132:{ //client acknowledge to server end. here to set acknowledge to true
					(*acknowledge) = true;
					_wrapper->disconnect(socket);
					return false;
				}
				case 133:{ //version request
					int version = (*content);
					int packet_size;
					char* packet = encodeVersionReply(&packet_size, version);
					_wrapper->send(socket, packet, packet_size);
					return true;
				}
				case 134:{ //version reply received
					char state = *content;
					if(state == 1){
						//alright we have nothing to do. version check was successfull, so client and server are on the same...
						return true;
					}
					else{
						//the version is different. lets output it to user...
						char version = *(content + 1);
						_wrapper->disconnect(socket);
						listener->onVersionErrorResponse(VERSION, version);
						return false;
					}
				}
				}
			}
			else{
				//User message
				switch(content_des){
					case 31:{//char Pointer
						char* return_content = new char[content_size];
						memcpy(return_content, content, content_size); //In case content is after the header, and we do not need the header anymore. If the user deletes the char* pointer it would be complicated
						listener->onReceiveCharArray(socket, return_content, content_size);
						break;
					}
					case 32:{ //int pointer
						int* return_content = new int[content_size / sizeof(int)];
						memcpy(return_content, content, content_size);
						listener->onReceiveIntArray(socket, return_content, content_size / sizeof(int));
	
						break;
					}
					case 33:{ //string pointer
						char* asChar = new char[content_size];
						vector<string> allStrings;
						memcpy(asChar, content, content_size);
						int first = 0, index = 0;
						while(index < content_size){
							if(asChar[index] == '\0'){
								string s(asChar + first, index - first);
								allStrings.push_back(s);
								if(index < content_size - 1)
									first = index + 1;
							}
							index++;
						}
	
						delete asChar;
						string* result = new string[allStrings.size()];
						for(unsigned int x = 0; x < allStrings.size(); x++){
							result[x] = allStrings.at(x);
						}
						listener->onReceiveStringArray(socket, result, allStrings.size());
						break;
					}
					case 34:{ //float pointer
						float* return_content = new float[content_size / sizeof(float)];
						memcpy(return_content, content, content_size);
						listener->onReceiveFloatArray(socket, return_content, content_size / sizeof(float));
	
						break;
					}
					case 35:{ //double pointer
						double* return_content = new double[content_size / sizeof(double)];
						memcpy(return_content, content, content_size);
						listener->onReceiveDoubleArray(socket, return_content, content_size / sizeof(double));
	
						break;
					}
					case 36:{ //bool pointer
						bool* return_content = new bool[content_size / sizeof(bool)];
						memcpy(return_content, content, content_size);
						listener->onReceiveBoolArray(socket, return_content, content_size / sizeof(bool));
	
						break;
					}
					case 37:{ //long pointer
						long long* return_content = new long long[content_size / sizeof(long long)];
						memcpy(return_content, content, content_size);
						listener->onReceiveLongArray(socket, return_content, content_size / sizeof(long long));
	
						break;
					}
					case 38:{ //short pointer
						short* return_content = new short[content_size / sizeof(short)];
						memcpy(return_content, content, content_size);
						listener->onReceiveShortArray(socket, return_content, content_size / sizeof(short));
	
						break;
					}
				}
				delete[] message;
				return true;
			}
			return true;
	}
	
	
	char* protocol::encodeDisconnected(){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 129; //10000001 says that opponent disconnected
		return data;
	}
	
	char* protocol::encodeClientWillDisconnect(int* packet_size){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 130; //10000010 says that client will disconnect
		(*packet_size) = getHeaderSize();
		return data;
	}
	
	char* protocol::encodeServerWillDisconnect(int* packet_size){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 131; //10000011 says that server wants to disconnect
		(*packet_size) = getHeaderSize();
		return data;
	}
	
	char* protocol::encodeServerDisconnectAcknowledge(int* packet_size){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 132; //10000100 says that client replys disconnect acknowledge
		(*packet_size) = getHeaderSize();
		return data;
	}
	
	
	char* protocol::encodeVersionRequest(int* packet_size){
		char* packet = createFieldWithHeader(1, 133, packet_size);
		packet[(*packet_size) - 1] = VERSION;
		return packet;
	}
	
	char* protocol::encodeVersionReply(int* packet_size, int opponentVersion){
	//	cout << "called" << endl;
		char* packet;
		if(VERSION == opponentVersion){
			packet = createFieldWithHeader(2, 134, packet_size);
			packet[(*packet_size)  - 1] = VERSION;
			packet[(*packet_size)  - 2] = 1;
			//cout << "Version is ok " << endl;
		}
		else{
			//cout << "Version is no ok " << endl;
			//cout << opponentVersion << endl;
			_expectDisconnect = true;
			packet = createFieldWithHeader(2, 134, packet_size);
			packet[(*packet_size)  - 1] = VERSION;
			packet[(*packet_size)  - 2] = 0;
		}
		return packet;
	}
	
	char* protocol::encodeRawData(){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 135; //RAW Data
		//(*packet_size) = getHeaderSize();
		return data;
	}

	char* protocol::encodeWrongSize(){
		char* data = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES];
		for(int x = 0; x < HEADER_SIZE_LENGTH; x++)
			data[x] = 0; //Disconnected has no payload
		data[HEADER_SIZE_LENGTH] = 136; //Wrong Size
		//(*packet_size) = getHeaderSize();
		return data;
	}

	char* protocol::createFieldWithHeader(int length, char type, int* packet_size){
		char* block = new char[HEADER_SIZE_LENGTH + HEADER_SIZE_CONTENT_DES + length];
	
		for(int count = HEADER_SIZE_LENGTH - 1; count >= 0; count--){
			block[count] = length & (0xFF << (8*(HEADER_SIZE_LENGTH - 1 -count)));
		}
		block[HEADER_SIZE_LENGTH] = type;
	
		*packet_size = length + getHeaderSize();
		return block;
	}
	
	
	char* protocol::encodeCharPointerPacket(char* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length, 31, packet_size); //31 is for char pointer
		memcpy(packet+ getHeaderSize(), data, length);//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeIntPointerPacket(int* data, int length, int* packet_size){
	
		char* packet = createFieldWithHeader(length, 32, packet_size); //32 is for int*
		memcpy(packet + getHeaderSize(), data, length);//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeStringPointerPacket(string* data, int length, int* packet_size){
		int bytes = 0;
		for(int x = 0; x < length; x++){
			bytes += data[x].length() + 1;
		}
		char* asChar = new char[bytes];
		int write = 0;
		for(int x = 0; x < length; x++){
			strncpy(asChar + write, data[x].c_str(), data[x].length() + 1);
			write += data[x].length() + 1;
		}
	
		char* packet = createFieldWithHeader(write, 33, packet_size); //33 is for string pointer
		memcpy(packet + getHeaderSize(), asChar, write);//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeFloatPointerPacket(float* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length * sizeof(float), 34, packet_size); //34 is for float*
		memcpy(packet + getHeaderSize(), data, length * sizeof(float));//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeDoublePointerPacket(double* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length * sizeof(double), 35, packet_size); //35 is for double*
		memcpy(packet + getHeaderSize(), data, length * sizeof(double));//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeBoolPointerPacket(bool* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length * sizeof(bool), 36, packet_size); //36 is for bool*
		memcpy(packet + getHeaderSize(), data, length * sizeof(bool));//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeLongPointerPacket(long long* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length * sizeof(long long), 37, packet_size); //37 is for long*
		memcpy(packet + getHeaderSize(), data, length * sizeof(long long));//Haenge Daten an
		return packet;
	}
	
	char* protocol::encodeShortPointerPacket(short* data, int length, int* packet_size){
		char* packet = createFieldWithHeader(length * sizeof(short), 38, packet_size); //38 is for short*
		memcpy(packet + getHeaderSize(), data, length * sizeof(short));//Haenge Daten an
		return packet;
	}
	
	protocol::protocol(Wrapperfacade* wrapper){
		_expectDisconnect = false;
		_wrapper = wrapper;
	}
}




