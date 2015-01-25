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
#include <unistd.h>

#include "fehe_connection/fehe_server.h"
#include "fehe_connection/fehe_event_listener.h"

using namespace std;
using namespace FeheConnection;

fehe_server* server;
int connected = 0;
int empfangen = 10;

int _socket = 0;

class event_listener : public fehe_event_listener{
public:

	//both
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



	//server
	virtual void onClientConnect(int socket);
	virtual void safetyServerEnd();

	//client, we dont need it here
	virtual void onVersionErrorResponse(int ownVersion, int opponentversion){};

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
void event_listener::onReceiveLongArray(int socket, long long* data, int length){
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
	cout << "Client disconnected" << endl;
	connected--;
}

void event_listener::onOpponentDisconnectUnexspected(int socket){
	cout << "Client disconnect unexspected." << endl;
	connected--;
}

void event_listener::onClientConnect(int socket){
	cout << "New client connected. Socket: " << socket << endl;
	_socket = socket;
	connected++;
}
void event_listener::safetyServerEnd(){
	cout << "All Clients disconnected. Server shut down..." << endl;
	delete server;
	cout << "Server is offline..." << endl;
}

void waitKey(){
#ifdef __linux
	getchar();
#elif _WIN32
	getchar(); //todo: take right function for windows
#endif
}


int main(int argc, char* argv[]) {
	event_listener listener;
	
	
	cout << "Server is starting" << endl;

	if(argc > 1)
		server = new fehe_server(&listener, atoi(argv[1]));
	else{
		cout << "No port specified. Server endet..." << endl;
		return -1;
	}


	//server = new fehe_server(&listener, 13000);
	cout << "Server started ..." << endl;


	double* d = new double[3];
	d[0] = 0.123456789;
	d[1] = 2.145878;
	d[2] = 17.589658475213333758;

	bool* b = new bool[3];
	b[0] = true;
	b[1] = false;
	b[2] = true;

	long long* l = new long long[3];
	l[0] = 800000000; 
	l[1] = 1600000000; 
	l[2] = 2400000000;

	short* s = new short[3];
	s[0] = 16000;
	s[1] = 20000;
	s[2] = 30000;



	cout << "Waiting for client..." << endl;
	while(!_socket){} //simple method to wait for a client to connect
	cout << endl;

	cout << "Press enter to send a double array" << endl;
	waitKey();
	if(server->getSender()->sendDoubleArray(_socket, d, 3) < 0){
		cout << "Error on sending...:(" << endl;
	}

	cout << "Press enter to send a bool array" << endl;
	waitKey();
	if(server->getSender()->sendBoolArray(_socket, b, 3) < 0){	
		cout << "Error on sending...:(" << endl;
	}

	cout << "Press enter to send a long array" << endl;
	waitKey();
	if(server->getSender()->sendLongArray(_socket, l, 3) < 0){
		cout << "Error on sending...:(" << endl;
	}

	cout << "Press enter to send a short array" << endl;
	waitKey();
	if(server->getSender()->sendShortArray(_socket, s, 3) < 0){
		cout << "Error on sending...:(" << endl;
	}



	cout << "Press enter to shutdown the server" << endl;
	waitKey();
	cout << "Server down" << endl;
	server->endServer();


	cout << "Close program..." << endl;
	waitKey();

	delete[] d;
	delete[] b;
	delete[] l;
	delete[] s;
	return 0;
}
