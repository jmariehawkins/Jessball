

#include "barrier.h"

//barrierimg is the left half of the screen where the player has
//successfully blocked off some pixels. 1 means the pixel has been
//blocked off. Stored in the same format as drawBitmap.
unsigned char barrierimg[(128/2)*8];

int numpixcomplete;
byte percentcompl;

//Set this screen position to a 1 in barrierimg
void bar_set(int x, int y) {
  if (x < 0 or y < 0 or x > w-1 or y > 63)
    return;
  
  int hexi = w*(y/8) + x;  //Convert (x,y) coordinates, which are horizontal and
  int hexj = y % 8;      //vertical pixels, to (hexi,hexj). hexi indexes barrierimg array,
                         //and hexj is the bit position within a byte.
                         
  //We will possibly change barrierimg[hexi] to a new hexstring
  //e.g. if hexj = 2 then 0x00000000   <- barrierimg[hexi]
  //              becomes 0x00100000   <- getstr
  //In other words, barrierimg[hexi] = barrierimg[hexi] OR 0x00100000
  
  unsigned char getstr = 1 << (8-hexj-1); //left bit-shift (8-hexj-i) places
  byte temp = barrierimg[hexi];
  barrierimg[hexi] = getstr | barrierimg[hexi];
  if (temp != barrierimg[hexi])
    numpixcomplete = numpixcomplete + 1;
}

//Return the bit value of the given screen position in barrierimg
bool bar_get(int x, int y) {
  if (x < 0 or y < 0 or x > w-1 or y > 63)
    return false;
  
  int hexi = w*(y/8) + x;  //Convert (x,y) coordinates, which are horizontal and
  int hexj = y % 8;      //vertical pixels, to (hexi,hexj). hexi indexes barrierimg array,
                         //and hexj is the bit position within a byte.

  //We want the bit in the hexj'th position in barrierimg[hexi]
  //In other words, if hexj == 2,
  //we want barrierimg[hexi] AND 0x00100000

  unsigned char getstr = 1 << (8-hexj-1); //left-shift (8-hexj-i) places
  return getstr & barrierimg[hexi];

}

//Return true if there is a ball at location x,y
bool hitaball(int x, int y) {

  //For each ball:
  for (byte i = 0; i < lvl; i++) {
    //If any part of the ball is at this location, return true
    if (x > balls[i].x - 4 and x < balls[i].x + 4
    and y > balls[i].y - 4 and y < balls[i].y + 4) {
      if (lives > 0) {
        lives = lives - 1;
      }
      return true;
    }
  }

  return false;
}

//Return true if there is at least one ball in the specified rectangle
//(We already know that no balls intersect the borders of the rectangle,
//so only need to test the centerpoints of the balls, not their edges)
bool ballinrect(int xmin, int xmax, int ymin, int ymax)
{
  //For each ball:
  for (byte i = 0; i < lvl; i++) {
    //If this ball's center is inside the rect:
    if (balls[i].x > xmin and balls[i].x < xmax
    and balls[i].y > ymin and balls[i].y < ymax) {
      return true;
    }
  }

  return false;
  
}

//White out the specified rectangle
void fillrect(int xmin, int xmax, int ymin, int ymax) {

  for (int x = xmin; x <= xmax; x++) {
    for (int y = ymin; y <= ymax; y++) {
      bar_set(x,y);
    }
  }
}


//dir: true means trying to place a vertical line, false horizontal
//x,y is the location of the cursor
void trytoplaceline (bool dir, int x, int y) {

  //If cursor is in a whited out area, return
  if (bar_get(x,y))
    return;

  int linemax, linemin; //The endpoints of what will be the new line

  //If vertical:
    //Find upward border and downward border. If any balls are
    //on this line, return.
  if (dir)
  {
    linemax = y;
    bool notfound = true;
    while (notfound) {
      if (hitaball(x,linemax))
        return;
      
      if (linemax == 64-1 or bar_get(x,linemax)) {
        //Hit the bottom of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        linemax = linemax + 1;
      }
    }

    linemin = y;
    notfound = true;
    while (notfound) {
      if (hitaball(x,linemin))
        return;
        
      if (linemin == 0 or bar_get(x,linemin)) {
        //Hit the top of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        linemin = linemin - 1;
      }
    }
    
  }
  else
  {
  //If horizontal:
    //Find rightward border and leftward border. If any balls are
    //on this line, return.

    linemax = x;
    bool notfound = true;
    while (notfound) {
      if (hitaball(linemax,y))
        return;
      
      if (linemax == w-1 or bar_get(linemax,y)) {
        //Hit the right of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        linemax = linemax + 1;
      }
    }

    linemin = x;
    notfound = true;
    while (notfound) {
      if (hitaball(linemin,y))
        return;
        
      if (linemin == 0 or bar_get(linemin,y)) {
        //Hit the left of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        linemin = linemin - 1;
      }
    }
  }

  //Now we have found linemin and linemax, what will be the endpoints
  //of the new line, and established that there are no balls in the way
  //and that (x,y) is not in a space that is already whited out.

  //If vertical:
    //Find the rightward boundary of the right rectangle, and the
    //leftward boundary of the left rectangle.

  int highbound,lowbound;

  if (dir)
  {
    highbound = x;
    bool notfound = true;
    
    while (notfound) {
      if (highbound == w-1 or bar_get(highbound,y)) {
        //Hit the right of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        highbound = highbound + 1;
      }
    }

    lowbound = x;
    notfound = true;
    while (notfound) {
      if (lowbound == 0 or bar_get(lowbound,y)) {
        //Hit the left of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        lowbound = lowbound - 1;
      }
    }
  }
  else
  {

  //If horizontal:
    //Find the downward boundary of the down rectangle, and the upward
    //boundary of the up rectangle.
    
    highbound = y;
    bool notfound = true;
    
    while (notfound) {
      if (highbound == 64-1 or bar_get(x,highbound)) {
        //Hit the bottom of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        highbound = highbound + 1;
      }
    }

    lowbound = y;
    notfound = true;
    while (notfound) {
      if (lowbound == 0 or bar_get(x,lowbound)) {
        //Hit the left of the screen or a whited out pixel, so return
        notfound = false;
      } else {
        lowbound = lowbound - 1;
      }
    }
  }

  //Now we have the corner coordinates of two rectangles, one on either
  //side of the new line to be drawn. Draw the line, and check whether
  //each rectangle contains balls.
  //If either rectangle is empty of balls, fill it. Then return.

  if (dir) {
    for (int i = linemin; i <= linemax; i++) {
      bar_set(x,i);
    }

    //Rightward rect
    if (! ballinrect(x,highbound,linemin,linemax))
      fillrect(x,highbound,linemin,linemax);

    //Leftward rect
    if (! ballinrect(lowbound,x,linemin,linemax))
      fillrect(lowbound,x,linemin,linemax);
    
  } else {
    for (int i = linemin; i <= linemax; i++) {
      bar_set(i,y);
    }

    //Downward rect
    if (! ballinrect(linemin,linemax,y,highbound))
      fillrect(linemin,linemax,y,highbound);

    //Upward rect
    if (! ballinrect(linemin,linemax,lowbound,y))
      fillrect(linemin,linemax,lowbound,y);
      
  }

}





