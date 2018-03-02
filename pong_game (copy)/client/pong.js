function log( text ) {
			$log = $('#log');
			//Add text to log
			$log.append(($log.val()?"\n":'')+text);
			//Autoscroll
			$log[0].scrollTop = $log[0].scrollHeight - $log[0].clientHeight;
		}

function Connect() {
    var IP = document.getElementById("ip").value;
    var PORT = document.getElementById("port").value;
    var USERNAME = document.getElementById("username").value;
    log('Connecting...<br>');
    clientManager = new ClientManager(IP, PORT, "", USERNAME);
    clientManager.send(['username', USERNAME]);
    clientManager.requestNewGame();

var animate = window.requestAnimationFrame || window.webkitRequestAnimationFrame || window.mozRequestAnimationFrame || function (callback) {
    window.setTimeout(callback, 1000 / 60)
};

// CREATE THE CANVAS AND ESTABLISH VARIABLES
var canvas = document.createElement("canvas");
var width = 900; 
var height = 400; 
canvas.width = width;
canvas.height = height;
var context = canvas.getContext('2d');
var player = new Player();
var player2 = new Player2();
var player3 = new Player3();
var player4 = new Player4();

var ball = new Ball(clientManager.ballX, clientManager.ballY);
var keysDown = {};
var score1 = clientManager.score1;
var score2 = clientManager.score2;

// PLACE HOLDER TO THEN REPLACE THE ACTUAL THING FROM CLIENT MANAGER
var playerOne = true;
//------------------------------------------------------------------

// RENDER ITEMS
var render = function () {

    // THE BOARD
    context.fillStyle = "#003300";
    context.fillRect(0, 0, width, height);

    // THE SCORE WRITING
    context.fillStyle = "#FFFFFF";
    context.font = "20pt Courier New";

    // Updating code to hold two separate scores
    // context.fillText("SCORE: " + clientManager.score.toString() + " %", 406, 15);
    context.fillText(clientManager.score1, 225, 25); //toString()
    context.fillText(clientManager.score2, 675, 25); //toString()

    context.fillStyle = "#FFFFFF";
    context.font = "12pt Courier New";
    if (clientManager.playerOne)
    {
        context.fillText("PLAYER 1", 5, 15);
        context.fillText(clientManager.username, 5, 35);
        context.fillText("PLAYER 2", 810, 15);
        context.fillText(clientManager.opponentUsername, 800, 35);
        context.fillText(clientManager.ping, 10, height - 20); //toString()
    }
    else
    {
        context.fillText("PLAYER 1", 5, 15);
        context.fillText(clientManager.opponentUsername, 5, 35);
        context.fillText("PLAYER 2", 810, 15);
        context.fillText(clientManager.username, 800, 35);
        context.fillText(clientManager.ping, width/2 + 10, height - 20); //toString()
    }
    // THE CENTER LINE
    context.fillStyle = "#FFFFFF";
    context.fillRect(width / 2, 0, 2, height);


    ball.x = clientManager.ballX;
    ball.y = clientManager.ballY;

    // RENDERING ALL OF THE OBJECTS
    player.render();
    player2.render();
    ball.render();
};

// UPDATING ACTIONS 
var update = function () {
   

    if (clientManager.playerOne){ // If you are player one then you are able to update it, so you can move the paddle
        player.update();
        clientManager.send(['game-state', player.paddle.x, player.paddle.y]);
        player2.paddle2.x = clientManager.opponentPx;
        player2.paddle2.y = clientManager.opponentPy;
    } else { // If not then only paddle two will update
        player2.update();
        clientManager.send(['game-state', player2.paddle2.x, player2.paddle2.y]);
        player.paddle.x = clientManager.opponentPx;
        player.paddle.y = clientManager.opponentPy;
    }

};

var step = function () {
    update();
    render();
    animate(step);
};

// CREATING PADDLE FUNCTION
function Paddle(x, y, width, height) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.x_speed = 0;
    this.y_speed = 0;
}

// CREATING PADDLE 2 FUNCTION
function Paddle2(x, y, width, height) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.x_speed = 0;
    this.y_speed = 0;
}

// RENDER PADDLE FUNCTION
Paddle.prototype.render = function () {
    context.fillStyle = "#FF0033";
    context.fillRect(this.x, this.y, this.width, this.height);
};

// RENDER PADDLE 2 FUNCTION
Paddle2.prototype.render = function () {
    context.fillStyle = "#FF0033";
    context.fillRect(this.x, this.y, this.width, this.height);
};

// PADDLE MOVEMENT
Paddle.prototype.move = function (x, y) {
    this.x += x;
    this.y += y;
    this.x_speed = x;
    this.y_speed = y;
    if (this.y < 0) { //AT THE TOP
        this.y = 0;
        this.y_speed = 0;
    } else if (this.y + this.height > 400) { // AT THE BOTTOM 
        this.y = 400 - this.height;
        this.y_speed = 0; 
    }
};

// PADDLE 2 MOVEMENT
Paddle2.prototype.move = function (x, y) {
    this.x += x;
    this.y += y;
    this.x_speed = x;
    this.y_speed = y;
    if (this.y < 0) { //AT THE TOP
        this.y = 0;
        this.y_speed = 0;
    } else if (this.y + this.height > 400) { // AT THE BOTTOM 
        this.y = 400 - this.height;
        this.y_speed = 0;
    }
};

// CREATE THE FIRST PLAYER
function Player() {
    // to get the x you subtract 20 from the width
    this.paddle = new Paddle(10, height / 2, 10, 80);
}

// CREATE THE SECOND PLAYER 
function Player2()
{
    this.paddle2 = new Paddle2(880, height / 2, 10, 80);
}

// RENDER PLAYER 1 PADDLE
Player.prototype.render = function () {
    this.paddle.render();
};

// RENDER PLAYER 2 PADDLE
Player2.prototype.render = function () {
    this.paddle2.render();
};

// APPLY PADDLE MOVEMENT [RIGHT SIDE]
Player.prototype.update = function () {
    for (var key in keysDown) {
        var value = Number(key);
        if (value == 40) { //DOWN ARROW
            this.paddle.move(0, 4);
        } else if (value == 38) { //UP ARROW
            this.paddle.move(0, -4);
        } 
    }
};

// APPLY PADDLE 2 MOVEMENT [LEFT SIDE]
Player2.prototype.update = function () {
    for (var key in keysDown) {
        var value = Number(key);
        if (value == 40) { //DOWN ARROW
            this.paddle2.move(0, 4);
        } else if (value == 38) { //UP ARROW
            this.paddle2.move(0, -4);
        }
    }
};


// CREATE THE BALL
function Ball(x,y) {
    this.x = x;
    this.y = y;
}

// RENDER BALL
Ball.prototype.render = function () {
    context.fillStyle = "#FFFF00";
    context.fillRect(this.x, this.y, 10, 10);
};

// THIS IS WHAT WILL MAKE EVERYTHING WORK
document.body.appendChild(canvas);
animate(step);

// CHECKS FOR THE PRESSING OF THE BUTTONS WHEN MOVING THE PADDLE
window.addEventListener("keydown", function (event) {
    keysDown[event.keyCode] = true;
});

window.addEventListener("keyup", function (event) {
    delete keysDown[event.keyCode];
});

} // Connect function END
