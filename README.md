# fehe-connection
An easy to use, server-client framework using sockets.

This is an easy to use library for communicating over the network via sockets. It is written in C++. Actually the library works fine on windows and linux. We have attachted two simple sample programs, which describes the usage of the library. 

This library does NOT need any additional dependencies.

Note: If there were any errors on compiling, installing, using or if you found bugs, please contact us and tell us about your error, so we can support you and improve the experience. You can reach us on our email-addresses: 
felix.froehling1@gmail.com
hendriku31@gmail.com

###How to install the library?
####Compiling and installing on linux:

This will install the library (libFeheConnection.so) to /usr/lib and the include filed to /usr/include/fehe_connection. This is hardcoded, so if you want different locations you have to edit this in the makefile.
  1. Download and unzip library.
  2. Change to "src" directory. (cd fehe_connection/src)
  3. compile by running "make" (make)
  4. install by running "sudo make install" (sudo make install)
  5. This will output some commands, but you do NOT need to run them(but maybe if there were errors)
  6. If you want to verify the installation, check if libFeheConnection.so appears under /usr/lib and all header files under /usr/include/fehe_connection are present (including two subdirectories)
  7. Note that you can tell us about any issues, we will try to help.


####Compiling and installing on windows:

Unfortunately we have actually no makefiles or scripts or for windows. We tested compiling and executing on windows on the IDE DevC++, but we haven't created a DLL at the moment. This feature will coming soon after we resolved the last errors within this. You can try to compile, but note that this will not install any DLLs to you system out of the box.
For compilation you will have to use c++11 compile flag, otherwise your compiler will complain.

###How to install the test programms:
####Compiling and running on linux:

This will compile a test programm for Server and Client. It will just work if you correctly set up you library like the steps above. This test programs documents the basic functionality of the library. You will see it is very easy to use.

You have to change to directory "test" and simply type "make".

After successfull compiling you can start a server by typing ./Server_Test 12000, where 12000 specifies the used port.

Launching a client will work with ./Client_Test 127.0.0.1 12000. This will connect to the server on 127.0.0.1 (localhost) on port 12000. Obviously you can connect from another computer to the server, you have just to check your firewall settings not to block any requests.

####Compiling and running on windows:
You can compile this test program if you have a working version of the library available on windows. This can be done with every IDE, for example DevC++. Please note that you have to specify the concret DLL to the linker and the location of the header files to the compiler. Also you will have to use C++11 standard for compiling. Note that you have to pass port number as start parameter to the server and an ip and a port number to client on start up. For that you can run the created executable from command-line with parameters or configuring your IDE to pass these parameters. See linux-section for this.




###Quick using guide:

With working library you need to:
```c++
#include <fehe_connection/fehe_listener.h>
#include <fehe_connection/fehe_server.h> //for server
#include <fehe_connection/fehe_client.h> // for client

//set namespace
using namespace FeheConnection;

//create listener
class listener: public fehe_event_listener{
//see test_programs for content
};

//implementing functions
listener::onReceiveCharArray(int socket, char* data, int length){
//char array empfangen
...
}

...

//for creating server
fehe_server* server = new fehe_server(port);

//for creating client
fehe_client* client = new fehe_client(ip, port);

//senden von server
server->getSender()->sendCharArray(socket, char* data, length);

//senden von client:
client->sendCharArray(char* data, length)


//disconnect client
client->disconnect();

//end server
server->endServer(); //NOTE: If you have any clients connected call this method before delete server. Call delete                                   server if event safetyServerEnd() occurs. We will explain this reason in detailed                                     documentation coming soon.
```

Obviously this is no complete code, this are just example commands. For detailed using please have a look at our attached examples.

Compile and link against the library on linux: g++-o foo -lFeheConnection -std=c++11 foo.cpp




###Coming soon:
In the future we want to:
- Add MAC-OS support
- Improve windows support
- Write an detailed documentation for using the library
- Add some features:
  - Server possibility to just disconnect one single client
  - Encryption
  - time synchronisation (real time)
  - simplification of some processes
  - Maybe copy-restore functions



