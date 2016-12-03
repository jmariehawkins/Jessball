#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif


#ifndef BALL_H
#define BALL_H

#include "barrier.h"

#define byte uint8_t


struct Ball {
  int x,y;   //Note x,y specifies the center of the ball,
                 //not its draw location
  int vx,vy; //Velocities
};

extern struct Ball *balls;

extern const int w; //width of the screen to use for the game, to save memory

extern byte lvl,lives,prevlives; //current level i.e. number of balls, current number of lives,
                            //number of lives in the previous frame (to prevent losing a bunch at once)
extern int score,prevscore;

void moveball (Ball *b);

extern const unsigned char ballimg[];    //ball image
extern const unsigned char cursorimg[];  //cursor image
extern const unsigned char lifeimg[];    //life image

#endif

