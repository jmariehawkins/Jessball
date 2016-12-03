#include "ball.h"

/*struct Ball {
  int x,y,vx,vy; //Note x,y specifies the center of the ball,
                 //not its draw location
};*/

struct Ball *balls;

const int w = 128/2;

byte lvl,lives,prevlives;

int score,prevscore;

void moveball (Ball *b) {
  //Test for collisions with walls of screen or barriers,
  // then increment x,y


  if (b->x <= 2 or bar_get(b->x - 2,b->y) or bar_get(b->x - 1,b->y)) {
    //Hit left side of screen
    b->vx = 0 - b->vx;
  } else if (b->x >= w-2 or bar_get(b->x + 2,b->y) or bar_get(b->x + 1,b->y)) {
    //Hit the right side of the screen
    b->vx = 0 - b->vx;
  }
  if (b->y <= 2 or bar_get(b->x,b->y - 2) or bar_get(b->x,b->y - 1)) {
    //Hit the top of the screen
    b->vy = 0 - b->vy;
  } else if (b->y >= 64-2 or bar_get(b->x,b->y + 2) or bar_get(b->x,b->y + 1)) {
    //Hit the bottom of the screen
    b->vy = 0 - b->vy;
  }
  
  b->x = b->x + b->vx;
  b->y = b->y + b->vy;
}

//width:5, height:5
PROGMEM const unsigned char ballimg[] =
{
  0b00001110,
  0b00011111,
  0b00011111,
  0b00011111,
  0b00001110
};

//width:5, height:5
PROGMEM const unsigned char cursorimg[] =
{
  0b00000100,
  0b00000100,
  0b00011111,
  0b00000100,
  0b00000100
};

//width:5, height:5
PROGMEM const unsigned char lifeimg[] =
{
  0b00001100,
  0b00010010,
  0b00011111,
  0b00010011,
  0b00011100
};

