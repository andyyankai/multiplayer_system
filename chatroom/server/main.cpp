#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"

using namespace std;

webSocket server;


/* called when a client connects */
void openHandler(int clientID) {
	ostringstream os;
	os << "Stranger " << clientID << " has joined.";
	map<int, int> clientIDs = server.get_map();

	map<int, int>::iterator cp;
	for (cp = clientIDs.begin(); cp != clientIDs.end(); cp++) {
		if (cp->first != clientID)
			if (cp->second == clientIDs[clientID])
				server.wsSend(cp->first, os.str());
	}
	server.wsSend(clientID, "Welcome!");
}


/* called when a client disconnects */
void closeHandler(int clientID) {
	ostringstream os;
	os << "Stranger " << clientID << " has leaved.";
	map<int, int> clientIDs = server.get_map();
	map<int, int>::iterator cp;
	for (cp = clientIDs.begin(); cp != clientIDs.end(); cp++) {
		if (cp->first != clientID)
			if (cp->second == clientIDs[clientID])
				server.wsSend(cp->first, os.str());
	}
	
}





/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	
	ostringstream os;
	os << "Stranger " << clientID << " says: " << message;
	
	map<int, int> clientIDs = server.get_map();

	map<int, int>::iterator cp;
	for (cp = clientIDs.begin(); cp != clientIDs.end(); cp++) {
		if (cp->first != clientID)
			if (cp->second == clientIDs[clientID])
				server.wsSend(cp->first, os.str());
	}
}




/* called once per select() loop */


int main(int argc, char *argv[]) {
	//int port;

	//cout << "Please set server port: ";
	//cin >> port;
	int port1 = 4025;
	int port2 = 4026;
	int port3 = 4027;
	int port4 = 4028;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	//server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port1, port2, port3, port4);


	return 1;
}
