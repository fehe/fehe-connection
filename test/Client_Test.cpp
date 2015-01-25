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

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "fehe_connection/fehe_client.h"

#include "fehe_connection/fehe_event_listener.h"

using namespace std;
using namespace FeheConnection;

fehe_client* client; //global client

bool isAlive;

class event_listener : public fehe_event_listener{
public:

	//server & client:

		//receive
		virtual void onReceiveCharArray(int socket, char* data, int length);
		virtual void onReceiveIntArray(int socket, int* data, int length);
		virtual void onReceiveStringArray(int socket, string* data, int length);
		virtual void onReceiveFloatArray(int socket, float* data, int length);
		virtual void onReceiveDoubleArray(int socket, double* data, int length);
		virtual void onReceiveBoolArray(int socket, bool* data, int length);
		virtual void onReceiveLongArray(int socket, long long* data, int length);
		virtual void onReceiveShortArray(int socket, short* data, int length);

		//event
		virtual void onOpponentDisconnect(int socket);
		virtual void onOpponentDisconnectUnexspected(int socket);

	//server, we dont need it here
	virtual void onClientConnect(int socket){};
	virtual void safetyServerEnd(){};

	//client
	virtual void onVersionErrorResponse(int ownVersion, int opponentversion);

	//destructor
	virtual ~event_listener(){};
};



void event_listener::onReceiveCharArray(int socket, char* data, int length){
	cout << "Received char array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}

void event_listener::onReceiveIntArray(int socket, int* data, int length){
	cout << "Received int array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}

void event_listener::onReceiveStringArray(int socket, string* data, int length){
	cout << "Received string array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}

void event_listener::onReceiveFloatArray(int socket, float* data, int length){
	cout << "Received float array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}

void event_listener::onReceiveDoubleArray(int socket, double* data, int length){
	cout << "Received double array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}
void event_listener::onReceiveBoolArray(int socket, bool* data, int length){
	cout << "Received bool array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}
void event_listener::onReceiveLongArray(int socket, long long *data, int length){
	cout << "Received long array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}
void event_listener::onReceiveShortArray(int socket, short* data, int length){
	cout << "Received short array..." << endl;
	for(int count = 0; count < length; count++){
		cout << data[count] << endl;
	}
	cout << endl;
	delete[] data;
}

void event_listener::onOpponentDisconnect(int socket){
	cout << "Server has ended..." << endl;
	isAlive = false;
}

void event_listener::onOpponentDisconnectUnexspected(int socket){ //When server has crashed or something else we have not exspected
	cout << "Server killed connection unexspected" << endl;
	cout << "Trying to reach fallback server..." << endl;

	int new_socket = client->connectToFallBackServer(); //connect to fallbackserver
	
	if(new_socket > 0){ //check if socket is valid
		cout << "Connected to fallback server" << endl;
		isAlive = true;
	}
	else if(new_socket == -2 ){
		cout << "Error: Library thinks, that we are connected to the main server :(" << endl;
		isAlive = false;
	}
	else{
		cout << "Could not reach fallback server..." << endl;
		isAlive = false;
	}
}

void event_listener::onVersionErrorResponse(int ownVersion, int opponentversion){
	cout << "The server is running a different version " << endl;
	cout << "Server version: " << opponentversion << endl;
	cout << "Our version: " << ownVersion << endl;
}

int main(int argc, char* argv[]) {
	//cout << sizeof(long long) << endl;
	event_listener listener;

	cout << "Client is starting..." << endl;
	client = new fehe_client(&listener); //create client with our listener

	client->setFallBackServer("127.0.0.1", 14000); //set fallback server. We will connect here if main server goes down. This is only optional

	cout << "Client has started" << endl;

//	int socket = client->connect("127.0.0.1", 13000); //connect to server on port 13000. That way you connect, but we will do this by commandline parameters.
	
	int socket;
	if(argc > 2)
		socket = client->connect(argv[1], atoi(argv[2]));
	else{
		cout << "Parameters are not correct. Please retry to give server ip and port" << endl;
		return -1;
	}
	
	if(socket >= 0){ //check if socket is valid
		cout << "Successfully connected to server..." << endl;
		isAlive = true;
	}
	else{
		cout << "Error on connect: " << strerror ( errno ) << endl; //We can ask for error number here
		delete client;
		return 0;
	}

	char* c = new char[4];
	c[0] = 'E';
	c[1] = 'n';
	c[2] = 't';
	c[3] = 'e';

	int* i = new int[3];
	i[0] = 42;
	i[1] = 1337;
	i[2] = 80;

	string* s = new string[3];
	s[0] = "Test1";
	s[1] = "Test2";
	s[2] = "Test3";

	float* f = new float[3];
	f[0] = 1.07;
	f[1] = 2.05;
	f[2] = 17.878;

	cout << "Please press enter to send a char array..." << endl;
	getchar();
	if(isAlive)
		client->sendCharArray(c, 4); //send char array
	else{
		cout << "We are not connected :(" << endl;
	}

	cout << "Please press enter to send an int array..." << endl;
	getchar();
	if(isAlive)
		client->sendIntArray(i, 3); //sende int array	
	else{
		cout << "We are not connected :(" << endl;
	}

	cout << "Please press enter to send a string array..." << endl;
	getchar();
	if(isAlive)
		client->sendStringArray(s, 3); //sende int array	
	else{
		cout << "We are not connected :(" << endl;
	}

	cout << "Please press enter to send a float array..." << endl;
	getchar();
	if(isAlive){
		client->sendFloatArray(f, 3); //sende int array
	}	
	else{
		cout << "We are not connected :(" << endl;
	}

	cout << "To disconnect and end the programm press enter..." << endl;
	getchar();

	cout << "Client is ending" << endl;
	client->disconnect();
	cout << "Client has ended." << endl;
	delete client;
	delete[] c;
	delete[] i;
	delete[] s;
	delete[] f;
	cout << "Program stopped." << endl;
	return 0;
}
