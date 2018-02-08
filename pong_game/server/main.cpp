#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
//#include <time.h>
#include "websocket.h"
#include <vector>
#include <queue>
#include <chrono>
#include "pong.h"
using namespace std;

webSocket server;
Pong*game;
bool gameState = false;

struct players {
	int playerID;
	std::string playername;
};

struct game_message {
	unsigned int player;
	unsigned long long time;
	unsigned long long timeSent;
	unsigned int keyCode;

};

players user;
std::queue<game_message> msg;



/* called when a client connects */
void openHandler(int clientID) {

	vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 1) {
		server.wsSend(clientID, "Connection closed because other player has disconnected.");
		server.wsClose(clientID);
	}
	else if (clientIDs.size() == 1) {

		std::cout << "A player with ID: " << clientID << " has conected.";
		user.playerID = clientIDs[0];
	}

}

/* called when a client disconnects */
void closeHandler(int clientID) {

	std::cout << "a player " << clientID << " has leaved.";

	vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() < 1) {
		gameState = false;
		delete game;
	}
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {

	if (gameState == true)
	{
		std::vector<string> split_msg = split(message, ',');
		unsigned int player = 0;
		game_message move;
		move.player = player;
		move.timeSent = std::stoull(split_msg[0]);
		move.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		move.keyCode = std::stoi(split_msg[1]);
		msg.push(move);
	}
	if(gameState==false)
	{		user.playername = message;
		std::ostringstream os;
		os << "player named: " << message << "has joined!";
		server.wsSend(clientID, os.str());
		vector<int> clientIDs = server.getClientIDs();
		if (clientIDs.size() == 1) {
			game = new Pong(800, 600);
			gameState = true;
		}
	}

}

/* called once per select() loop */
void periodicHandler() {
	vector<int> clientIDs = server.getClientIDs();
	if (gameState == true && clientIDs.size() == 1) {
		if (!msg.empty()) {
			std::ostringstream confirmMsg;
			confirmMsg << "Move confirmed," << msg.front().keyCode;

			game->movePlayer(msg.front().player, msg.front().keyCode);
			server.wsSend(user.playerID, confirmMsg.str());
			msg.pop();
		}
		
		
		bool new_game = game->update();
		if (new_game == true) {
			server.wsSend(user.playerID, "New Round");
		}


		std::ostringstream pos = game->trackmovement();
		pos << "," << user.playername << ",0";
		server.wsSend(user.playerID, pos.str());



	}
}

int main(int argc, char *argv[]) {
	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
