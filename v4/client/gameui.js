var context;
var HEIGHT=600;
var WIDTH=800;
var canvas;

var p1x = 30;
var p1y = 250;

var p2x = 750;
var p2y = 250;

var p3x = 350;
var p3y = 30;

var p4x = 350;
var p4y = 550;

//players---------------------------
var player = {
    width: 20, height: 100,
    x: null, y: null,
     update:function(x,y, old_x, old_y, offset)
     {
         this.old_x = old_x;
         this.old_y = old_y;
         this.x = x; this.y = y; this.y += offset;
         this.y = Math.max(Math.min(this.y,600-this.height),0);
     },
     draw: function () { context.fillRect(this.old_x, this.old_y, this.width, this.height); }
};

var player2 = {
    width: 20, height: 100,
    x: null, y: null,
    update: function (x, y, old_x, old_y, offset) {
        this.old_x = old_x;
        this.old_y = old_y;
        this.x = x; this.y = y; this.y += offset;
        this.y = Math.max(Math.min(this.y, 600 - this.height), 0);
    },
    draw: function () { context.fillRect(this.old_x, this.old_y, this.width, this.height); }
};
var player3 = {
    width: 100, height: 20,
    x: null, y: null,
    update: function (x, y, old_x, old_y, offset2) {
        this.old_x = old_x;
        this.old_y = old_y;
        this.x = x; this.y = y; this.x += offset2;
        this.x = Math.max(Math.min(this.x, 800 - this.width), 0);
    },
    draw: function () { context.fillRect(this.old_x, this.old_y, this.width, this.height); }
};
var player4 = {
    width: 100, height: 20,
    x: null, y: null,
    update: function (x, y, old_x, old_y, offset2) {
        this.old_x = old_x;
        this.old_y = old_y;
        this.x = x; this.y = y; this.x += offset2;
        this.x = Math.max(Math.min(this.x, 800 - this.width), 0);
    },
    draw: function () { context.fillRect(this.old_x, this.old_y, this.width, this.height); }
};

var ping = {
    p1ping: 0,
    p2ping: 0,
    p3ping: 0,
    p4ping: 0,

    update: function(p1p, p2p,p3p,p4p){
        this.p1ping = p1p;
        this.p2ping = p2p;
        this.p3ping = p3p;
        this.p4ping = p4p;
    },
    draw: function(){
        context.font = "22px ariel";
        context.textAlign = 'center';
        context.fillText( "Ping(ms): " + this.p3ping, WIDTH / 3.4, 50) //top

        context.fillText( "Ping(ms): " + this.p4ping, WIDTH / 1.52, 580) //bot

        context.fillText( "Ping(ms): " + this.p1ping, WIDTH / 12.1, 420) //left

        context.fillText( "Ping(ms): " + this.p2ping, WIDTH / 1.11, 220) //right
    }
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
        context.font = "22px ariel";
        context.textAlign = "center";


        context.fillText( this.playerID + "'s score: " + this.pscore, WIDTH / 11, 400) //left

        context.fillText( this.playerID2 + "'s score: " + this.pscore2, WIDTH / 1.1, 200) //right

        context.fillText( this.playerID3 + "'s score: " + this.pscore3, WIDTH / 3.3, 25) //top

        context.fillText( this.playerID4 + "'s score: " + this.pscore4, WIDTH / 1.5, 560) //bot
 


    }
};
var ball={
    x:null, y:null, radius:20, x_diff:null, y_diff:null,old_x:null,old_y:null,

    update:function(x,y,old_x,old_y){
        this.old_x = this.old_x;
        this.old_y = this.old_y;
        this.x_diff = x - this.x;
        this.y_diff = y - this.y;
        this.x = x;
        this.y = y;
    },
    draw:function(){
        context.beginPath();
        context.arc((this.old_x),
                    (this.old_y),
                    this.radius,0,2*Math.PI);

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

    player.old_x = player.width;
    player.old_y = (HEIGHT - player.height) / 2;

    player2.old_x = WIDTH - player2.width * 2;
    player2.old_y = (HEIGHT - player2.height) / 2;

    player3.old_x = (WIDTH - player3.width) / 2;
    player3.old_y = player3.height;

    player4.old_x = (WIDTH - player4.width) / 2;
    player4.old_y = HEIGHT - player4.height * 2;

    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;


    var loop = function(){

    	//ball and opponent players interpolation
        for ( var i = 0; i <= 150; i++)
        {
            ball.old_x = ball.old_x+((ball.x-ball.old_x)/2000);
            ball.old_y = ball.old_y+((ball.y-ball.old_y)/2000);

        }

        if (player.x) {
            p1x = player.x;
        }

        if (player.y) {
            p1y = player.y;
        }
        if (player2.x) {
            p2x = player2.x;
        }

        if (player2.y) {
            p2y = player2.y;
        }
        if (player3.x) {
            p3x = player3.x;
        }

        if (player3.y) {
            p3y = player3.y;
        }
        if (player4.x) {
            p4x = player4.x;
        }

        if (player.y) {
            p4y = player4.y;
        }

        for ( var i = 0; i <= 150; i++)
        {
            player.old_y = player.old_y+((p1y-player.old_y)/2000);
        }

        for ( var i = 0; i <= 150; i++)
        {
            player2.old_y = player2.old_y+((p2y-player2.old_y)/2000);
        }

        for ( var i = 0; i <= 150; i++)
        {
            player3.old_x = player3.old_x+((p3x-player3.old_x)/2000);
        }

        for ( var i = 0; i <= 150; i++)
        {
            player4.old_x = player4.old_x+((p4x-player4.old_x)/2000);
        }




        draw();
        // window.requestAnimationFrame(loop, canvas);
    };
    // window.requestAnimationFrame(loop, canvas);

    setInterval(loop,16);
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
    ping.draw();
    
    
    context.restore();
}
main();
