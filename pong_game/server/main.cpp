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
	int playerID2;
	int playerID3;
	int playerID4;
	std::string playername;
	std::string playername2;
	std::string playername3;
	std::string playername4;
};

struct game_message {
	unsigned int player;
	unsigned long long time;
	unsigned long long timeSent;
	unsigned int keyCode;
	unsigned int delay;
};

players user;
std::queue<game_message> msg;
bool artificial_latency = false;
bool fixed_latency = true;
bool increased_latency = false;


/* called when a client connects */
void openHandler(int clientID) {

	vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 4) {
		std::cout << clientID << "Connection refused, 4 players are full!" << std::endl;
		server.wsSend(clientID, "connection refused, 4 players are already playing;");
		server.wsClose(clientID);
	}
	else {

		std::cout << clientID << " has joined the game";
		if (clientIDs.size() == 4) {
			user.playerID = clientIDs[0];
			user.playerID2 = clientIDs[1];
			user.playerID3 = clientIDs[2];
			user.playerID4 = clientIDs[3];
		}
	}

}

/* called when a client disconnects */
void closeHandler(int clientID) {

	std::cout << "a player " << clientID << " has leaved.";

	vector<int> clientIDs = server.getClientIDs();

	if (clientIDs.size() < 4) {
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
	ostringstream os;
	os << "another player named" + message + "hasjoined!";
	vector<int> clientIDs = server.getClientIDs();
	server.wsSend(clientID, "You named: " + message + "has joined!");
	for (int i = 0; i < clientIDs.size(); i++) {
		if (clientIDs[i] != clientID)
		{
			server.wsSend(clientIDs[i], os.str());
		}
	}



	if (gameState == true)
	{
		std::vector<string> split_msg = split(message, ',');
		unsigned int player;
		if (clientID == user.playerID) {
			player = 0;
		}
		else if (clientID == user.playerID2) {
			player = 1;
		}
		else if (clientID == user.playerID3) {
			player = 2;
		}
		else if (clientID == user.playerID4) {
			player = 3;
		}
		game_message move;
		move.player = player;
		move.timeSent = std::stoull(split_msg[0]);
		move.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		move.keyCode = std::stoi(split_msg[1]);
		if (artificial_latency==true) {
			move.delay = (rand() % 3 + 1) * 100;
		}
		if (fixed_latency==true) {
			move.delay = 40; 
		}
		if (increased_latency==true) {
			int max = 300;
			int min = 100;
			move.delay = 100;
		}
		
		msg.push(move);
	}
	if (gameState == false) {
		if (clientID == 0) {
			user.playername = message;
		}
		else if (clientID == 1) {
			user.playername2 = message;
		}
		else if (clientID == 2) {
			user.playername3 = message;
		}
		else if (clientID == 3) {
			user.playername4 = message;
		}



		if (clientIDs.size() == 4) {
			game = new Pong(800, 600);
			gameState = true;
		}
	}

}

/* called once per select() loop */
void periodicHandler() {
	vector<int> clientIDs = server.getClientIDs();
	if (gameState == true && clientIDs.size() == 4) {
		if (!msg.empty()) {
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			if (time >= msg.front().time + msg.front().delay) {
				std::ostringstream confirmMsg;
				confirmMsg << "Move confirmed," << msg.front().keyCode;

				game->movePlayer(msg.front().player, msg.front().keyCode, time - msg.front().timeSent);
				if (msg.front().player == 0) {
					server.wsSend(user.playerID, confirmMsg.str());
				}
				if (msg.front().player == 1) {
					server.wsSend(user.playerID2, confirmMsg.str());
				}
				if (msg.front().player == 2) {
					server.wsSend(user.playerID3, confirmMsg.str());
				}
				if (msg.front().player == 3) {
					server.wsSend(user.playerID4, confirmMsg.str());

				}
				msg.pop();
			}
			if (increased_latency==true) {
				if (msg.front().delay < 300) { msg.front().delay += 20; }
			}
		}


		bool new_game = game->update();
		if (new_game == true) {
			server.wsSend(user.playerID, "New Round");
			server.wsSend(user.playerID2, "New Round");
			server.wsSend(user.playerID3, "New Round");
			server.wsSend(user.playerID4, "New Round");
		}


		std::ostringstream pos = game->trackmovement();
		std::ostringstream p2os = game->trackmovement();
		std::ostringstream p3os = game->trackmovement();
		std::ostringstream p4os = game->trackmovement();

		pos << "," << user.playername << "," << user.playername2 << "," << user.playername3 << "," << user.playername4 << ",0";
		p2os << "," << user.playername << "," << user.playername2 << "," << user.playername3 << "," << user.playername4 << ",1";
		p3os << "," << user.playername << "," << user.playername2 << "," << user.playername3 << "," << user.playername4 << ",2";
		p4os << "," << user.playername << "," << user.playername2 << "," << user.playername3 << "," << user.playername4 << ",3";

		server.wsSend(user.playerID, pos.str());
		server.wsSend(user.playerID2, p2os.str());
		server.wsSend(user.playerID3, p3os.str());
		server.wsSend(user.playerID4, p4os.str());



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
