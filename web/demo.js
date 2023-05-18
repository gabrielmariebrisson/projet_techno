Module.onRuntimeInitialized = () => { start(); }

/* ******************** global variables ******************** */

var canvas = document.getElementById('mycanvas');
var canvasButtomRestart = document.getElementById('buttonrestart');
var canvasButtomSolve = document.getElementById('buttonsolve');
var canvasButtomRandom = document.getElementById('buttonrandom');
var canvasButtomUndo = document.getElementById('buttonundo');
var canvasButtomRedo = document.getElementById('buttonredo');


//variable peut etre utile venant du fichier c
var PERC_OF_SQUARE= 0.0667;
var RESIZE_SQUIRREL=2.4;
var READJUST_SQUIRREL=0.01;
var  SIDE_MARGIN = 0;            // SPACE BETWEENT THE THE LEFT EDGE AND THE GRID
var  HEIGHT_MARGIN= 0;          // SPACE BETWEENT THE THE TOP EDGE AND THE GRID
var  PERC_OF_SQUARE= 0.0667;       // PERCENTAGE OF THE SQUARE IN THE WINDOW
var  DEFAULT_WINDOW= 600.0;        // DEFAULT WINDOW SIZE
var  HEIGHT_MARGIN_BUTTONS= 0.24;  // SPACE BETWEEN THE TOP EDGE AND THE 2 FIRST BUTTONS
var  MARGIN_BUTTONS= 0.18;         // SPACE BETWEEN TWO BUTTONS
var  RESIZE_BUTTONS= 1.5;          // RESIZE THE BUTTONS
var  RESIZE_SQUIRREL= 2.4;         // RESIZE THE ANGRY SQUIRREL
var  SIZE_OF_BUTTON= 0.1;

// load images
var fire_hydrant = new Image();
fire_hydrant.src = "./src/bouche-incendie.png";
var water = new Image();
water.src = "./src/water.png";
var marks = new Image();
marks.src = "./src/marks.png";
var flame = new Image();
flame.src = "./src/flame.png";
var error = new Image();
error.src = "./src/error.png";
var quit = new Image();
quit.src = "./src/Quit.png";
var redo = new Image();
redo.src = "./src/Redo.png";
var undo = new Image();
undo.src = "./src/Undo.png";
var restart = new Image();
restart.src = "./src/Restart.png";
var random = new Image();
random.src = "./src/random.png";
var solve = new Image();
solve.src = "./src/Solve.png";
var wall_U = new Image();
wall_U.src = "./src/wall_U.png";
var wall_1 = new Image();
wall_1.src = "./src/wall_1.png";
var wall_2 = new Image();
wall_2.src = "./src/wall_2.png";
var wall_3 = new Image();
wall_3.src = "./src/wall_3.png";
var wall_4 = new Image();
wall_4.src = "./src/wall_4.png";
var wall_0 = new Image();
wall_0.src = "./src/wall_0.png";
var won = new Image();
won.src = "./src/won.png";


var g;
var ctx;

//global variable
var mouse_x;
var mouse_y;
var nb_rows;
var nb_cols;
var height_box;
var width_box;
var width_game;
var height_game;
var L;
var H;

// initial position in canvas

/* ******************** register events ******************** */

window.addEventListener('load', windowLoad);              // window load
canvas.addEventListener('click', canvasLeftClick);        // left click event
canvasButtomRedo.addEventListener('click', canvasButtonRedoLeftClick);       // left click event
canvasButtomUndo.addEventListener('click', canvasButtonUndoLeftClick);
canvasButtomSolve.addEventListener('click', canvasButtonSolveLeftClick);
canvasButtomRestart.addEventListener('click', canvasButtonRestartLeftClick); 
canvasButtomRandom.addEventListener('click', canvasButtonRandomLeftClick);
/* ******************** event callback ******************** */

function canvasLeftClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    mouse_x = event.offsetX;
    mouse_y = event.offsetY;
    console.log("left click at position:", mouse_x, mouse_y);
    
    x = ~~(mouse_x/width_box);
    y = ~~(mouse_y/height_box);

    // set new state depending current state
    switch (Module._get_state(g, y, x)) {  
      case 0:
        Module._play_move(g, y, x, 1);
        break;

      case 1:
        Module._play_move(g, y, x, 2);
        break;

      case 2:
        Module._play_move(g, y, x, 0);
        break;

      default:
          break;
    }

    printGame(g);
}


function canvasButtonRedoLeftClick(events){
  console.log("redo");
  Module._redo(g);
  printGame(g);
}
function canvasButtonRestartLeftClick(events){
  console.log("restart");
  Module._restart(g);
  printGame(g);
}
function canvasButtonUndoLeftClick(events){
  console.log("undo");
  Module._undo(g);
  printGame(g);
}
function canvasButtonSolveLeftClick(events){
  console.log("solve");
  Module._solve(g);
  printGame(g);
}
function canvasButtonRandomLeftClick(events){
  console.log("random");
  createRandomGame();
  printGame(g);
}
function windowLoad() {
    console.log("window load");
    printGame(g);
}

/* ******************** canvas drawing ******************** */

function printGame(g) {
    console.log("print game");
    var text = "";

    // clear canvas
    ctx.clearRect(0, 0, width_game, height_game);
    ctx.save();
    ctx.strokeStyle = 'black';
    ctx.beginPath();
    for (var row = 0; row < nb_rows; row++) { //height
      ctx.moveTo(0,row*(height_box));
      ctx.lineTo(width_game,row*(height_box));
    }
    for (var col = 0; col < nb_cols; col++) {
        ctx.moveTo(col*(width_box),0);
        ctx.lineTo(col*(width_box),height_game);
    }  

    // draw some lines
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            if (Module._has_error(g, row, col)) {
                ctx.drawImage(error, col*width_box, row*height_box, width_box, height_box);
            } else if (Module._is_lighted(g, row, col) && !Module._is_lightbulb(g, row, col)) {
                ctx.drawImage(water, col*width_box, row*height_box, width_box, height_box);
            } else if (!Module._is_lighted(g, row, col) && Module._is_blank(g, row, col)) {
                ctx.drawImage(flame, col*width_box, row*height_box, width_box, height_box);
            } else if (Module._is_lightbulb(g, row, col)) {
                ctx.drawImage(fire_hydrant, col*width_box, row*height_box, width_box, height_box);
            } else if (Module._is_marked(g, row, col)) {
                ctx.drawImage(marks, col*width_box, row*height_box, width_box, height_box);
            } else if (Module._is_black(g, row, col)) {
                var b_n = Module._get_black_number(g, row, col);
                if (!Module._is_over(g)) {
                  switch (b_n) {
                    case 0:
                        ctx.drawImage(wall_0, col*width_box, row*height_box, width_box, height_box);
                      break;
                    case 1:
                        ctx.drawImage(wall_1, col*width_box, row*height_box, width_box, height_box);
                      break;
                    case 2:
                        ctx.drawImage(wall_2, col*width_box, row*height_box, width_box, height_box);
                      break;
                    case 3:
                        ctx.drawImage(wall_3, col*width_box, row*height_box, width_box, height_box);
                      break;
                    case 4:
                        ctx.drawImage(wall_4, col*width_box, row*height_box, width_box, height_box);
                    default:
                        ctx.drawImage(wall_U, col*width_box, row*height_box, width_box, height_box);
                  }
                } else {
                  ctx.drawImage(won, col*width_box, row*height_box, width_box, height_box);
                }
              }
        }
        text += "\n";
    }

    // put this text in <div> element with ID 'result'
    var elm = document.getElementById('result');
    elm.innerHTML = text;
    ctx.stroke(); // the stroke will actually paint the current lines
    ctx.restore();
}

/* --------------------------------------------------------------------------------------- */

function init_variables() {
  ctx = canvas.getContext('2d');
  
  width_game = canvas.width;
  height_game = canvas.height;
  
  nb_cols = Module._nb_cols(g);
  nb_rows = Module._nb_rows(g);
  
  width_box = width_game/nb_cols;
  height_box = height_game/nb_rows;

  console.log("game : ", width_game, " x ", height_game);
  console.log("nb_cols = ", nb_cols);
  console.log("nb_rows = ", nb_rows);  
  console.log("box : ", width_box, " x ", height_box);
}

function createDefaultGame() {
  g = Module._new_default();
  init_variables();
}

function createRandomGame() {
  g = Module._new_random(nb_rows, nb_cols, false, 10, false);
  init_variables();
}

function start() {
    console.log("call start routine");

    createDefaultGame();
}


/*
function end(){
  Module._delete(g);
}
*/
