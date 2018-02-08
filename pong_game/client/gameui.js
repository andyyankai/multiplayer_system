var context;
var HEIGHT=600;
var WIDTH=800;
var canvas;
var player = {
    width: 20, height: 100,
 x: null, y: null,
 update:function(x,y,offset)
 {
     this.x = x; this.y = y; this.y += offset;
     this.y = Math.max(Math.min(this.y,1000-this.height),0);
 },
 draw: function () { context.fillRect(this.x, this.y, this.width, this.height); }
     };

var score = {
    playerID:null,pscore:0,
    update: function (pid, ps) {
        this.playerID = pid;
        this.pscore = ps;
    },
    draw: function () {
        context.font = "24px ariel";
        //context.textAlign = 'center';
        context.fillText("score:"+this.playerID + ": " + this.pscore, WIDTH / 2, 50);
    }
};
var ball={
    x:null, y:null, radius:20,
    update:function(x,y){
        this.x=x;this.y=y;
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
    player.x = player.width
    player.y = (HEIGHT - player.height) / 2;
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
    score.draw();
    
    
    context.restore();
}
main();