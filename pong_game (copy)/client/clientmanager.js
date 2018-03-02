
function OnOpen(MSG) {

};

function OnClose(MSG) {

};

function OnMessage(MSG, ClientManager) {
	console.log("message event");

    var values = MSG.data.split(",");
    var temp = values[0];

	if (temp.toString() == "start-game")
	{
		log('Connected.<br>');
	}

	else if (temp.toString() == "game-state")
	{
	    ClientManager.ballX = values[1];
	    ClientManager.ballY = values[2];
	    ClientManager.score1 = values[3];
	    ClientManager.score2 = values[4];
	    ClientManager.opponentPx = values[5];
	    ClientManager.opponentPy = values[6];
	}

	else if (temp.toString() == "Deny")
	{
		
	}

	else if (temp == "username")
	{
		ClientManager.username = values[1];
	}

	else if (temp == "player-number")
	{
		ClientManager.playerNumber = values[1];

		if (values[1] == "playerone")
			ClientManager.playerOne = true;
		else
			ClientManager.playerOne = false;
	}
	else if (temp == "opponent")
	{
		ClientManager.opponentUsername = values[1];
	}
	else if (temp == "ping")
	{
		var date = new Date();
		this.currHour = date.getHours();
		this.currMin = date.getMinutes();
		this.currSec = date.getSeconds();
		this.currMs = date.getMilliseconds();

		ClientManager.send(["ping", currHour, currMin, currSec, currMs]);
	}
	else if (temp == "ping-return")
	{
		var hoursSent = values[1];
		var minutesSent = values[2];
		var secondsSent = values[3];
		var millisecondsSent = values[4];

		var hours = hoursSent - currHour;
		var minutes = minutesSent - currMin;
		var seconds = secondsSent - currSec;

		var total = 0;

		var milliseconds = millisecondsSent - currMs;

		if (milliseconds >= 0)
			total = millisecondsSent - currMs;
		else
			total = 1000 + (millisecondsSent - currMs);

		ClientManager.ping = "Ping: " + total + " ms";
	}

	this.message_received = true;
};

function OnError(MSG) {

};

function WaitForConnection(socket, callback) {
    setTimeout(
        function() {
            if (socket.readyState === 1) {
                if(callback !== undefined) {
                    callback();
                }
                return;
            } 
            else {
                WaitForConnection(socket,callback);
            }
        }, 
    5);
};


var ClientManager = function (IP, Port, Protocols, Username) {
    // log('Trying to connect...');
    this.IP = IP;
    this.Port = Port;

	this.Connection = new WebSocket('ws://' + IP + ':' + Port);

	// log('Created the websocket');

	this.Start_Game = 'start-game';
	this.Deny_Access = 'Deny';

	this.Connection.onopen = function(evt) {
		OnOpen(evt);
	};

	this.Connection.onclose = function(evt) {
		OnClose(evt);
	};

	self = this;
	this.Connection.onmessage = function(evt) {
		OnMessage(evt, self);
	};

	this.Connection.onerror = function(evt) {
		OnError(evt);
	};

	this.ballX = 450;
	this.ballY = 200;
	this.paddleX = 880;
	this.paddleY = 200;
	this.score1 = 0;
	this.score2 = 0;
	this.username = Username;
	this.opponentUsername;
	this.playerNumber;
	this.playerOne;
	this.opponentPx = 10;
	this.opponentPy = 200;
	this.ping = "Ping: 0 ms";

	this.hoursNow = 0;
	this.minutesNow = 0;
	this.secondsNow = 0;
	this.millisecondsNow = 0;
	this.total = 0;

	this.message_received = false;
}

ClientManager.prototype.wait = function (callback) {
    alert('I am waiting in the ClientManager Wait function');
	self = this;
	setTimeout(
		function() {
			if (this.message_received === true) {
				if (callback !== undefined) {
					callback();
				}
				this.message_received = false;
				return;
			}
			else {
				self.wait(callback);
			}
		},
	5);
}

ClientManager.prototype.send = function(MSG) {

	Connection = this.Connection;

	WaitForConnection(this.Connection, function() {
		Connection.send(MSG);
	});
}

ClientManager.prototype.requestNewGame = function () {
	this.send(this.Start_Game);
}

ClientManager.prototype.closeConnection = function() {
	this.Connection.close();
}

