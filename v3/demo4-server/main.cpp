#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <vector>
#include <queue>
#include <chrono>
#include "pong.h"
#include <cstdlib>

using namespace std;
using namespace std::chrono;

webSocket server;
Pong*game;
bool gameState = false;
bool use_latency = true;	//currently cannot use this
int latencyType1 = 0; // 0 == fixed, 1 = incremental, 2 = random	client->server
int latencyType2 = 0; // 0 == fixed, 1 = incremental, 2 = random	server->client
					  //remember to add to periodic handler
#define INTERVAL_MS 10
int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;



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



players user;




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



//==============message, inBuffer===================
struct game_message {
	unsigned int player;
	unsigned long long time;
	unsigned long long timeSent;
	unsigned int keyCode;

};
//buffer



//vector<pair<int, pair <int, long long> >> inBuffer;

vector< pair<pair<int, int>, pair<int, long long>> > inBuffer;
//pair (ID, pair(opcode,expected time))


int latencyTime = 50;
long long rlatency = 200;
long long  slatency = 200;

void getLatency(int type) {
	int min = 50;
	int max = 2000;


	//fixed
	if (type == 0) {
		rlatency = 200;
		slatency = 200;
	}
	//incremental
	if (type == 1) {
		if (latencyTime <= max) {
			latencyTime += 50;
			rlatency = latencyTime;
			slatency = latencyTime;
		}
	}
	//random
	else if (type == 2) {
		latencyTime = (rand() % (max - min + 1)) + min;
		rlatency = latencyTime;
		slatency = latencyTime;
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
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

		unsigned long long t1 = std::stoull(split_msg[0]);
		unsigned int keyCode = std::stoi(split_msg[1]);
		auto now = std::chrono::system_clock::now();
		auto diff = now.time_since_epoch();
		auto sd = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

		pair <int, int> playerID_and_keycode;
		playerID_and_keycode.first = player;
		playerID_and_keycode.second = keyCode;
		std::cout << "playerID_and_keycode: " << playerID_and_keycode.first << "    " << playerID_and_keycode.second << endl;
		pair <int, long long> t1_and_expectedTime;
		t1_and_expectedTime.first = t1;
		getLatency(latencyType1);
		t1_and_expectedTime.second = sd + rlatency;
		std::cout << "t1_and_expectedTime: " << t1_and_expectedTime.first << "    " << t1_and_expectedTime.second << endl;			//correct, type long long

		pair<pair<int, int>, pair<int, long long>> send_info;
		send_info.first = playerID_and_keycode;
		send_info.second = t1_and_expectedTime;
		inBuffer.push_back(send_info);
		std::cout << "Test index0:  playerID:" << inBuffer[0].first.first << "op-code:" << inBuffer[0].first.second << "t1:" << inBuffer[0].second.first << "expected:" << inBuffer[0].second.second << endl;



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

		std::ostringstream os;
		os << "player named: " << message << "has joined!";
		server.wsSend(clientID, os.str());
		vector<int> clientIDs = server.getClientIDs();
		if (clientIDs.size() == 4) {
			game = new Pong(800, 600);
			gameState = true;
		}
	}

}
vector< pair<string, long long> >  outBuffer;
void periodicHandler() {
	vector<int> clientIDs = server.getClientIDs();
	static time_t next = clock() + interval_clocks;
	if (gameState == true && clientIDs.size() == 4) {
		if (!inBuffer.empty()) {
			auto Pnow = std::chrono::system_clock::now();
			auto Pdiff = Pnow.time_since_epoch();
			auto Psd = std::chrono::duration_cast<std::chrono::milliseconds>(Pdiff).count();
			vector<int> delete_list;
			for (int i = 0; i < inBuffer.size(); i++) {
				if (inBuffer[i].second.second <= Psd) {
					std::cout << "current time Psd" << Psd << "> expected time " << inBuffer[i].second.second << ", can process!";
					std::cout << "size: " << inBuffer.size() << endl;
					game->movePlayer(inBuffer[i].first.first, inBuffer[i].first.second);
					delete_list.push_back(i);
				}
			}
			for (int i = delete_list.size() - 1; i >= 0; i--) {
				inBuffer.erase(inBuffer.begin() + i);
			}
		}

		time_t current = clock();
		if (current >= next) {

			std::cout << "start determine." << endl;
			auto Pnow = std::chrono::system_clock::now();
			auto Pdiff = Pnow.time_since_epoch();
			auto Psd = std::chrono::duration_cast<std::chrono::milliseconds>(Pdiff).count();//t4
			std::cout << "outBuffer[i].time:   " << Psd << "end pos." << endl;
			bool new_game = game->update();
			if (new_game == true) {
				std::cout << "New Round: " << endl;
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
			
			getLatency(latencyType2);
			outBuffer.push_back(pair<string, long long>(pos.str(), Psd + slatency));


			vector<int> deletes;

			for (int i = 0; i < outBuffer.size(); i++) {
				if (outBuffer[i].second <= Psd) {
					deletes.push_back(i);
					for (int j = 0; j < clientIDs.size(); j++) {
						;
					}
				}
			}
			server.wsSend(user.playerID, pos.str());
			server.wsSend(user.playerID2, p2os.str());
			server.wsSend(user.playerID3, p3os.str());
			server.wsSend(user.playerID4, p4os.str());
			for (int i = deletes.size() - 1; i >= 0; i--) {
				outBuffer.erase(outBuffer.begin() + deletes[i]);
			}
			next = clock() + interval_clocks;

		}
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
