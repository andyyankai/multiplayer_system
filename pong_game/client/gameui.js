
		var Server;
		var gameState = false;
		var upArrow = 38;
		var leftArrow = 37;
		var downArrow = 40;
		var rightArrow = 39;
		var debug = false;
		var offset = 0;
		function log( text ) {
			$log = $('#log');
			//Add text to log
			$log.append(($log.val()?"\n":'')+text);
			//Autoscroll
			$log[0].scrollTop = $log[0].scrollHeight - $log[0].clientHeight;
		}

		function send( text ) {
			Server.send( 'message', text );
		}

        function connect(){

        	if (gameState == true)
        	{
        		document.getElementById("cntBtn").disabled = false;
			    document.getElementById("cntBtn").innerHTML = "Connect"
			    window.scrollTo(0, 0);
			    Server.disconnect();
			    gameState = false;
        	}
        	else
        	{

            log('Connecting...<br>');
			Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);

			$(document).keydown(function(event) {
			    if (event.which == upArrow || event.which == downArrow) {
			        var d = new Date();
			        send(d.getTime() + "," + event.which);
			        offset += event.which == upArrow ? -15 : 15;

			    }
			    else if(event.which==leftArrow||event.which==rightArrow)
			    {
			        var d2 = new Date();
			        send(d2.getTime() + "," + event.which);
			        offset2 += event.which == leftArrow ? -15 : 15;
			    }
			});

			//Let the user know we're connected
			Server.bind('open', function() {
                document.getElementById("cntBtn").disabled = true;
                document.getElementById("cntBtn").innerHTML = "Disconnect"
                log("Connected.<br>");
                send(document.getElementById('gameid').value);
			});

			//OH NOES! Disconnection occurred.
			Server.bind('close', function( data ) {
                document.getElementById("cntBtn").disabled = false;
                document.getElementById("cntBtn").innerHTML = "Connect"
				log( "Disconnected.<br>" );
			});

			//Log any messages sent from server
			Server.bind('message', function( payload ) {
			    if (payload =="connection refused, 4 players are already playing;<br>") {
			        log(payload);
			        log("<br>");
			    }
			    else if (payload.indexOf("Move confirmed") > -1) {
			        var res = payload.split(",");
			        offset += res[1] == upArrow ? 15 : -15;
			        offset2 += res[1] == leftArrow ? 15 : -15;

			    }
			    else if (payload.indexOf("New Round") > -1) {
			        offset = 0;
			        offset2 = 0;
			    }
			    else if (gameState) {
			        if (debug) {
			            log(payload);
			        }
			        var res = payload.split(",");

			        ball.update(res[8], res[9]);

			        if (res[18] == 0) {
			            player.update(parseInt(res[0]), parseInt(res[1]), offset);
			            player2.update(parseInt(res[2]), parseInt(res[3]), 0);
			            player3.update(parseInt(res[4]), parseInt(res[5]),0);
			            player4.update(parseInt(res[6]), parseInt(res[7]),0);

			        }
			        if (res[18] == 1) {
			            player.update(parseInt(res[0]), parseInt(res[1]), 0);
			            player2.update(parseInt(res[2]), parseInt(res[3]), offset);
			            player3.update(parseInt(res[4]), parseInt(res[5]), 0);
			            player4.update(parseInt(res[6]), parseInt(res[7]), 0);

			        }
			        if (res[18] ==2) {
			            player.update(parseInt(res[0]), parseInt(res[1]), 0);
			            player2.update(parseInt(res[2]), parseInt(res[3]), 0);
			            player3.update(parseInt(res[4]), parseInt(res[5]), offset2);
			            player4.update(parseInt(res[6]), parseInt(res[7]), 0);

			        }
			        if (res[18] == 3) {
			            player.update(parseInt(res[0]), parseInt(res[1]), 0);
			            player2.update(parseInt(res[2]), parseInt(res[3]), 0);
			            player3.update(parseInt(res[4]), parseInt(res[5]), 0);
			            player4.update(parseInt(res[6]), parseInt(res[7]), offset2);

			        }
			        

			        score.update(res[14], res[15], res[16], res[17], res[10], res[11], res[12], res[13]); //score(playername,score)
			        // ping.update();

			    }
			    else {
			        log(payload);
			        log("<br>")
			        gameState = true;
			    }
			});

			Server.connect();
			}
        }

var context;
var HEIGHT = 600;
var WIDTH = 800;
var canvas;
//players---------------------------
var player = {
    width: 20, height: 100,
    x: null, y: null,
    update: function (x, y, offset) {
        this.x = x; this.y = y; this.y += offset;
        this.y = Math.max(Math.min(this.y, 600 - this.height), 0);
    },
    draw: function () { context.fillRect(this.x, this.y, this.width, this.height); }
};
var player2 = {
    width: 20, height: 100,
    x: null, y: null,
    update: function (x, y, offset) {
        this.x = x; this.y = y; this.y += offset;
        this.y = Math.max(Math.min(this.y, 600 - this.height), 0);
    },
    draw: function () { context.fillRect(this.x, this.y, this.width, this.height); }
};
var player3 = {
    width: 100, height: 20,
    x: null, y: null,
    update: function (x, y, offset) {
        this.x = x; this.y = y; this.x += offset;
        this.x = Math.max(Math.min(this.x, 800 - this.width), 0);
    },
    draw: function () { context.fillRect(this.x, this.y, this.width, this.height); }
};
var player4 = {
    width: 100, height: 20,
    x: null, y: null,
    update: function (x, y, offset) {
        this.x = x; this.y = y; this.x += offset;
        this.x = Math.max(Math.min(this.x, 800 - this.width), 0);
    },
    draw: function () { context.fillRect(this.x, this.y, this.width, this.height); }
};

var score = {
    playerID: "", pscore: 0,
    playerID2: "", pscore2: 0,
    playerID3: "", pscore3: 0,
    playerID4: "", pscore4: 0,
    update: function (pid, pid2, pid3, pid4, ps, ps2, ps3, ps4) {
        this.playerID = pid;
        this.playerID2 = pid2;
        this.playerID3 = pid3;
        this.playerID4 = pid4;
        this.pscore = ps;
        this.pscore2 = ps2;
        this.pscore3 = ps3;
        this.pscore4 = ps4;
    },
    draw: function () {
        context.font = "24px ariel";
        context.textAlign = 'center';


        context.fillText(this.playerID + "'s score: " + this.pscore +
		" " + this.playerID2 + "'s score: " + this.pscore2 +
		" " + this.playerID3 + "'s score: " + this.pscore3 +
		" " + this.playerID4 + "'s score: " + this.pscore4, WIDTH / 2, 200);

    }
};
var ball = {
    x: null, y: null, radius: 20,
    update: function (x, y) {
        this.x = x;
        this.y = y;
    },
    draw: function () {
        context.beginPath();
        context.arc(this.x, this.y, this.radius, 0, 2 * Math.PI);
        context.fillStyle = "white";
        context.fill();
        context.stroke();
    }
};

function main() {
    canvas = document.createElement("canvas");
    canvas.width = WIDTH;
    canvas.height = HEIGHT;
    context = canvas.getContext("2d");
    $('#game_canvas').append(canvas);

    player.x = player.width;
    player.y = (HEIGHT - player.height) / 2;

    player2.x = WIDTH - player2.width * 2;
    player2.y = (HEIGHT - player2.height) / 2;

    player3.x = (WIDTH - player3.width) / 2;
    player3.y = player3.height;

    player4.x = (WIDTH - player4.width) / 2;
    player4.y = HEIGHT - player4.height * 2;

    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    var loop = function () {
        draw();
        window.requestAnimationFrame(loop, canvas);
    };
    window.requestAnimationFrame(loop, canvas);
}

function draw() {
    context.fillRect(0, 0, WIDTH, HEIGHT);

    context.save();
    context.fillStyle = "white";
    ball.draw();
    player.draw();
    player2.draw();
    player3.draw();
    player4.draw();
    score.draw();
    context.fillText(FancyWebSocket.ping, 100, 50);


    context.restore();
}
main();

