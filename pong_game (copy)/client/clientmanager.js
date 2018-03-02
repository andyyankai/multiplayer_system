
function OnOpen(MSG) {
	// JSON.stringify to show response message in formatted output
	// Uncomment alert messages to use for debugging purposes
	//alert('Connection Opened\n' + JSON.stringify(MSG, null, 4));
};

function OnClose(MSG) {
	// JSON.stringify to show response message in formatted output
	// Uncomment alert messages to use for debugging purposes
	//alert('Connection Closed\n' + JSON.stringify(MSG, null, 4));
};

// This function will run whenever the server sends a message to the client
// Fill out the values of ClientManager as needed. 
function OnMessage(MSG, ClientManager) {
	console.log("message event");

    var values = MSG.data.split(",");
    var temp = values[0];
    //alert(temp.toString());
    //alert('Values:' + values.toString());

	// Values array assumed to have ['message', ballX, ballY, paddleX, paddleY, score]
	// Could do if statements down here on different messages in values[0]
	// based on what the server sends. So you could have an 'end-game' message, etc.
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
		// Possibly send a message to display that the connection was denied
		// Uncomment alert messages to use for debugging purposes
		//alert('A game is already in progress. Your request to start another game has been denied.');
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
		this.hoursNow = date.getHours();
		this.minutesNow = date.getMinutes();
		this.secondsNow = date.getSeconds();
		this.millisecondsNow = date.getMilliseconds();

		ClientManager.send(["ping", hoursNow, minutesNow, secondsNow, millisecondsNow]);
	}
	else if (temp == "ping-return")
	{
		var hoursSent = values[1];
		var minutesSent = values[2];
		var secondsSent = values[3];
		var millisecondsSent = values[4];

		var hours = hoursSent - hoursNow;
		var minutes = minutesSent - minutesNow;
		var seconds = secondsSent - secondsNow;

		var total = 0;

		var milliseconds = millisecondsSent - millisecondsNow;

		if (milliseconds >= 0)
			total = millisecondsSent - millisecondsNow;
		else
			total = 1000 + (millisecondsSent - millisecondsNow);

		ClientManager.ping = "Ping: " + total + " ms";
	}

	this.message_received = true;
};

function OnError(MSG) {
	// Uncomment alert messages to use for debugging purposes
	//alert('Connection Errored\n' + JSON.stringify(MSG, null, 4));
};

// The goal of WaitForConnection is to make sure that the websocket
// is done establishing its connection before any messages are sent 
// over socket.
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

// This is connect player in a constructor
// Instantiate like clientManager = ClientManager(IP, port, username)
var ClientManager = function (IP, Port, Protocols, Username) {
    //alert('Trying to connect...');
    this.IP = IP;
    this.Port = Port;

	//this.Connection = new WebSocket(IP, Protocols);
    //this.Connection = new WebSocket('ws://' + IP + ':' + port + '/data/websocket', Protocols);
	this.Connection = new WebSocket('ws://' + IP + ':' + Port);

	//alert('Created the websocket');

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

// This function takes a callback and will execute
// it when a message is received from the server
ClientManager.prototype.wait = function (callback) {
    alert('I am waiting in the ClientManager Wait function');
	self = this;
	setTimeout(
		function() {
			// Check if a message was received
			if (this.message_received === true) {
				if (callback !== undefined) {
					callback();
				}
				// If it was execute the callback then set the value back to false
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
	// Grab connection in NEW object so it can be used
    // in the WaitForConnection callback
    //alert('I am in the ClientManager send');
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

// This is example usage of the ClientManager. Things like port and username are NOT
// currently used and will need to be used when these things have meaning
//clientManager = new ClientManager('ws://localhost:8080/data/websocket', 80, 'echo-protocol', "KittensWithNoMittens");

// Send ping to server or something. This is an example from the tutorial...
// sending ping to this server actually doesn't do anything.

// After this send is issued the test server will return an array of values and they will be parsed in
// OnMessage. Will need some sort of waiting system in place to determine when the message is received
//clientManager.send('s');

// This function below will wait on the current thread until a message is received from the server. This function
// is what will be used, likely, when starting a new game. So you'd send an 's' and wait for the server to respond
// with whatever data you need for new game. The data provided here is just example data.

// This will block the thread of execution if used before a send or something. It will also block the thread
// if the server never responds so strategies will have to be come up with to fix that.
//clientManager.wait(function() {
	////alert('message received and processed new clientManager values are: ' + clientManager.paddleX + ',' + clientManager.paddleY + ',' + clientManager.score);
//});
