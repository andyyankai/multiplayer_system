function OnMessage(MSG, FancyWebSocket) {

    var values = MSG.data.split(",");
    var temp = values[0];
   
	if (temp.toString() == "game-state")
	{
	    ClientManager.ballX = values[1];
	    ClientManager.ballY = values[2];
	    ClientManager.score1 = values[3];
	    ClientManager.score2 = values[4];
	    ClientManager.opponentPx = values[5];
	    ClientManager.opponentPy = values[6];
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



var FancyWebSocket = function(url)
{
	var callbacks = {};
	var ws_url = url;
	var conn;

	this.bind = function(event_name, callback){
		callbacks[event_name] = callbacks[event_name] || [];
		callbacks[event_name].push(callback);
		return this;// chainable
	};

	this.send = function(event_name, event_data){
		this.conn.send( event_data );
		return this;
	};

	this.connect = function() {
		if ( typeof(MozWebSocket) == 'function' )
			this.conn = new MozWebSocket(url);
		else
			this.conn = new WebSocket(url);

		// dispatch to the right handlers
		this.conn.onmessage = function(evt){
			dispatch('message', evt.data);
		};

		this.conn.onclose = function(){dispatch('close',null)}
		this.conn.onopen = function(){dispatch('open',null)}
	};

	this.disconnect = function() {
		this.conn.close();
	};

	var dispatch = function(event_name, message){
		var chain = callbacks[event_name];
		if(typeof chain == 'undefined') return; // no callbacks for this event
		for(var i = 0; i < chain.length; i++){
			chain[i]( message )
		}
	}
	
	this.ping = "Ping: 0 ms";
	this.currHour = 0;
	this.currMin = 0;
	this.currSec = 0;
	this.currMs = 0;
	this.total = 0;
	
};

FancyWebSocket.prototype.send = function(MSG) {

	Connection = this.conn;

	Connection.send(MSG);
}

