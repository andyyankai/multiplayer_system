
#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <time.h> 
#include "websocket.h"
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include "GameLogic.h"
#include <queue>
#include <algorithm>


using namespace std;

//used to save usernames of connected players
std::map<int,std::string> usernames;
webSocket server;
GameLogic game;



int counter;
int counterLimit = 500;
bool gameStarted = false;


//store the info for the player paddles
float px;
float py;
float px2;
float py2;
float px3;
float py3;
float px4;
float py4;

//after both are true run update and send back messages
bool p1;
bool p2;

int playerInGame = -1;
int playerInGame2 = -1;

int playerCount = 0;

float playerNumber = 1;


void sendPing()
{
	if (server.getClientIDs().size() > 0)
	{
		int counter = 0;
		for (auto& care: server.getClients())
		{
			std::string ping = "ping,";
			ping += care->getTimeStamp();
			//std::cout << "The ping I'm sending: " << ping << std::endl;
			server.wsSend(counter,ping);
			counter++;
		}
	}
}

void sendMessages()
{
	
}

//Parse the string helper
std::vector<std::string> &parseString(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::string item;
    std::stringstream ss(s);

    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}
//Parse string
std::vector<std::string> parseString(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    parseString(s, delim, elems);
    return elems;
}

void startGame()
{
	vector<int> clientIDs = server.getClientIDs();
	px = 10;
	py = 200;
	px2 = 880;
	py2 = 200;
	px3 = 400;
	py3 = 10;
	px4 = 400;
	py4 = 880;

	std::string toSend;
	std::string second;
	toSend = "game-state,";
	toSend += game.update(px, py, px2, py2, px3, py3, px4, py4);
	second = toSend;


	toSend += ",";
	toSend += std::to_string(10);
	toSend += ",";
	toSend += std::to_string(game.padY);

	second += ",";
	second += std::to_string(880);
	second += ",";
	second += std::to_string(game.padY2);

	p1 = false;
	p2 = false;
	gameStarted = true;
	std::cout << "I'm sending my messages now! " << toSend << std::endl;
	std::string opponentName = "opponent," + usernames.find(playerInGame)->second;
	std::string opponentName2 = "opponent," + usernames.find(playerInGame2)->second;
	std::cout << "Player ones opponent: " << opponentName2 << std::endl;
	std::cout << "Player two opponent: " << opponentName << std::endl;
	sendPing();

	server.wsSend(clientIDs[playerInGame], opponentName2);
	server.wsSend(clientIDs[playerInGame2], opponentName);



	server.wsSend(clientIDs[playerInGame], toSend);
	server.wsSend(clientIDs[playerInGame2], second);
}

void checkForMatches()
{
	if (!gameStarted)
	{
		bool nextPlayer = false;
		if (usernames.size() > 1)
		{
			for (auto& check : usernames)
			{
				if (!nextPlayer)
				{
					playerInGame = check.first;
					nextPlayer = true;
				}
				else
				{
					playerInGame2 = check.first;
					break;
				}

			}
		}

		std::string toSend = "player-number,playerone";
		std::string toSend2 = "player-number,playertwo";


		server.wsSend(playerInGame, toSend);
		server.wsSend(playerInGame2, toSend2);
		startGame();
	}
}


void openHandler(int clientID)
{
	if (playerNumber > 3) {
	        std::cout << clientID << "Connection refused, 4 players are full!" << std::endl;
	        server.wsSend(clientID, "connection refused, 4 players are already playing;");
		server.wsClose(clientID);
		return;
	}
	else
	{
	std::string toSend = "player-number,";

	if(playerNumber == 1)
	{
		toSend += "playerone";
	}
	else if(playerNumber == 2)
	{
		toSend += "playertwo";
	}
	else if(playerNumber == 3)
	{
		toSend += "playerthree";
	}
	else
	{
		toSend += "playerfour";
	}

	std::cout << "You're player: " << playerNumber << std::endl;
	playerNumber++;
	if(playerInGame == -1)
		playerInGame = clientID;
	else if(playerInGame2 == -1)
		playerInGame2 = clientID;

    server.wsSend(clientID, toSend);
	}
}


void closeHandler(int clientID)
{
	std::cout << "a player " << clientID << " has leaved.";
	
	playerCount--;
	if((clientID == playerInGame || clientID == playerInGame2) && gameStarted)
	{
		playerNumber = 1;
		if (playerInGame == clientID)
			playerInGame = -1;
		else if (playerInGame2 == clientID)
			playerInGame2 -1;
		gameStarted = false;
		game.Reinitialize();
	}
	std::map<int, string>::iterator it;
	it = usernames.find(clientID);
	usernames.erase(it);

	std::cout << "Player has DC'd!!" << std::endl;
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message)
{ 
	std::vector<std::string> v = parseString(message, ',');
	vector<int> clientIDs = server.getClientIDs();
	

	if ( v[0] == "start-game")
	{
		if(!gameStarted && (clientID == 0 || clientID == 1))
		{
			string toSend;
			//Call game logic to start
			if(clientID == playerInGame)
			{
				p1 = true;
				std::cout <<"Player one has connected!" << std::endl;
			}else if(clientID == playerInGame2)
			{
				p2 = true;
				std::cout <<"Player two has connected!" << std::endl;
			}

			if(p1 && p2)
			{
				startGame();
			}
		}
		else
			server.wsSend(clientIDs[clientID], "Deny");

	}else if(v[0] == "username")
	{
		usernames.insert(std::pair<int, string>(clientID, v[1]));
		playerCount++;
		if (usernames.size() > 1)
			checkForMatches();
	}
	else if (v[0] == "game-state")
	{
		if(gameStarted)
		{
			if(clientID == 0)
			{
				px = stof(v[1]);
				py = stof(v[2]);
				p1 = true;
				//std::cout<<"I've gotten player one's update! "<< std::endl;
			}else if(clientID == 1)
			{
					px2 = stof(v[1]);
					py2 = stof(v[2]);
				p2 = true;
				//std::cout<<"I've gotten player two's update! "<< std::endl;
			}

			if(p1 && p2)
			{
				std::string toSend;
				std::string playerTwo;

				toSend = "game-state,";
				toSend += game.update(px,py,px2,py2,px3,py3,px4,py4);
				playerTwo = toSend;

				toSend += ",";
				toSend += std::to_string(10);
				toSend += ",";
				toSend += std::to_string(game.padY);

				playerTwo += ",";
				playerTwo += std::to_string(880);
				playerTwo += ",";
				playerTwo += std::to_string(game.padY2);

				p1 = false;
				p2 = false;

				
				//That or we can condsider the fact that we wait on both players to input as the delay.
				server.wsSend(clientIDs[0], playerTwo);
				server.wsSend(clientIDs[1],toSend);
			}

			if (counter >= counterLimit)
			{
				//std::cout << "I'm sending my ping!" << std::endl;
				sendPing();
				counter = 0;
			}
			else
				counter++;
		}
	}

	// When the server sends a ping to the client, the client returns a ping with the time that the message was received.
	// We now want to calculate the time it took to receive a response.
	// We pass the time our message was received by the client to calculateLatency().
	else if (v[0] == "ping")
	{
		std::string time = "";
		time += v[1];
		time += ",";
		time += v[2];
		time += ",";
		time += v[3];
		time += ",";
		time += v[4];

		int latency = server.getClients()[clientID]->calculateLatency(time);
		if (clientID == playerInGame)
			game.player1Latency = latency;
		else
			game.player2Latency = latency;

		std::string ping = "ping-return,";
		ping += server.getClients()[clientID]->getTimeStamp();
		//std::cout << "The ping I'm sending: " << ping << std::endl;
		server.wsSend(clientID, ping);
	}
}
/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + 10;
    time_t current = time(NULL);
    if (current >= next){
        ostringstream os;
        string timestring = ctime(&current);
        timestring = timestring.substr(0, timestring.size() - 1);
        os << timestring;

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], os.str());

        next = time(NULL) + 10;
    }
}

int main(int argc, char *argv[])
{
    int port;

    cout << "Please set server port: ";
    cin >> port;

    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    //server.setPeriodicHandler(periodicHandler);
    server.startServer(port);
}
