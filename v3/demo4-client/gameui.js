var context;
var HEIGHT=600;
var WIDTH=800;
var canvas;
//players---------------------------
var player = {
    width: 20, height: 100,
 x: null, y: null,
 update:function(x,y,offset)
 {
     this.x = x; this.y = y; this.y += offset;
     this.y = Math.max(Math.min(this.y,600-this.height),0);
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
    update: function (pid,pid2,pid3,pid4, ps,ps2,ps3,ps4) {
        console.log(pid2);
        console.log(pid3);
        console.log(ps);
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


		context.fillText( this.playerID + "'s score: " + this.pscore+
		" "+this.playerID2 + "'s score: " + this.pscore2+
		" " + this.playerID3 + "'s score: " + this.pscore3 +
		" " + this.playerID4 + "'s score: " + this.pscore4, WIDTH / 2, 200);

    }
};
var ball={
    x:null, y:null, radius:20,
    update:function(x,y){
        this.x = x;
        this.y = y;
    },
    draw:function(){
        context.beginPath();
        context.arc(this.x,this.y,this.radius,0,2*Math.PI);
        context.fillStyle="white";
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
    var loop = function(){
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
    
    
    context.restore();
}
main();
