#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif


#ifndef BARRIER_H
#define BARRIER_H

#include "ball.h"

#define byte uint8_t

/*
 * Keep track of which parts of the screen have been
 * blocked off by the player, and methods for testing
 * when a new line is placed, what to block off
 */


extern unsigned char barrierimg[(128/2)*8]; //for drawBitmap

extern int numpixcomplete;
extern byte percentcompl; //numpixcomplete stores the real number,
                            //percentcompl is an integer approximation
                            //to display on the screen

//Set this screen position to a 1 in barrierimg
void bar_set(int x, int y);

//Return the bit value of the given screen position in barrierimg
bool bar_get(int x, int y);

//Return true if there is a ball at location x,y
bool hitaball(int x, int y);

//Return true if there is at least one ball in the specified rectangle
bool ballinrect(int xmin, int xmax, int ymin, int ymax);

//White out the specified rectangle
void fillrect(int xmin, int xmax, int ymin, int ymax);

//Draw the barrier using drawpixel, since drawbitmap won't work
//void drawbarrier();

//dir: true means trying to place a vertical line, false horizontal
//x,y is the location of the cursor
void trytoplaceline (bool dir, int x, int y);


#endif

