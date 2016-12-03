/*
 * A copy of the bounce game I had on my laptop as a kid
 * Jessica Hawkins 10/20/16
 */

//#include "ball.h"
#include "barrier.h"

#include "Arduboy.h"


Arduboy arduboy;

int cursorx,cursory; //location of cursor


//Draw the barrier using drawpixel, since drawbitmap won't work
void drawbarrier() {

  unsigned char tempc;
  int hexi = 0;    //Index in the barrierimg array
  int coolv = 0;   //Just for making the cool background image
  for (int x = 0; x < w; x++) {           //x is the horizontal pixel coordinate
    int y = 0;
    coolv = x % (lvl+2);
    for (int y = 0; y < HEIGHT/8; y++) {  //y is the vertical coordinate (8 pixels tall)
      hexi = w*y + x;   //Convert (x,y) coordinates to hexi coordinate
      tempc = barrierimg[hexi];
      for (int k = 7; k > -1; k--) {      //k is the bit index within a byte

        //if (tempc % 2), then this bit is a 1 in barrierimg.
        //The other conditions are just for making the background pattern more interesting.
        if (tempc % 2 and (k+4*x) % 2 and k >= coolv)
          arduboy.drawPixel(x,y*8+k,WHITE);
        tempc = tempc >> 1;    //bit shift right
      }
    }
  }

}

//Initialize the game variables for a new level
void newlevelinit(int level) {

  arduboy.clear();
  delay(200);

  if (level < 10) {
    lvl = level;
  } else {
    lvl = 10;
  }
  numpixcomplete = 0;
  percentcompl = 0;
  //prevscore = score;
  score = 0;

  //Clear barrierimg to all 0's (black)
  for (int i = 0; i < w*8; i++) {
    barrierimg[i] = 0b00000000;
  }

  //Put the cursor in the middle of the screen
  cursorx = w/2;
  cursory = HEIGHT/2;

  //Stagger the balls around the screen and
  //give them random velocities
  for (byte i = 0; i < lvl; i++) {
    balls[i].x = 5 + i*6;
    balls[i].y = 20 + i;

    byte r = random(2);
    if (r==1)
      balls[i].vx = r + 1;
    else
      balls[i].vx = 0 - (r + 1);

    r = random(2);
    if (r==1)
      balls[i].vy = r + 1;
    else
      balls[i].vy = 0 - (r + 1);
  }
}

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.display();
  //intro();
  arduboy.clear();

  //Initialize game variables
  prevlives = 8; //Lives of previous frame (to ensure only <=1 lost per frame)
  lives = 8;
  lvl = 1;
  score = 0;
  prevscore = 0;

  //Only allocate once when Arduboy is turned on
  //balls = malloc(lvl * 10 * sizeof(int));
  balls = malloc(10 * 4 * sizeof(int));

  randomSeed(analogRead(0));  //Seed the random number generator
  newlevelinit(lvl);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!(arduboy.nextFrame()))
    return;
    
  //Clear the screen
  arduboy.clear();

  //Draw the whited out parts of the screen
  drawbarrier();

  //Move and draw all the balls
  for (byte i = 0; i < lvl; i++) {
    moveball(&balls[i]);
    arduboy.drawBitmap(balls[i].x - 2,balls[i].y - 2,ballimg,5,5,WHITE);
  }

  //Draw the cursor
  arduboy.drawBitmap(cursorx - 2,cursory - 2,cursorimg,5,5,WHITE);

  //Test for player input and respond accordingly
  if (arduboy.pressed(UP_BUTTON) and cursory>0) {
    cursory = cursory - 1;
  }
  if (arduboy.pressed(DOWN_BUTTON) and cursory<HEIGHT-1) {
    cursory = cursory + 1;
  }
  if (arduboy.pressed(LEFT_BUTTON) and cursorx>0) {
    cursorx = cursorx - 1;
  }
  if (arduboy.pressed(RIGHT_BUTTON) and cursorx<w-1) {
    cursorx = cursorx + 1;
  }
  if (arduboy.pressed(A_BUTTON)) {
    trytoplaceline(true,cursorx,cursory);
  } else if (arduboy.pressed(B_BUTTON)) {
    trytoplaceline(false,cursorx,cursory);
  }


  if (numpixcomplete > 3700 - lvl*25) {
    //Start new level
    newlevelinit(lvl + 1);
  }

  if (lives < prevlives - 1) {
    //Only allow <=1 live lost per frame
    lives = prevlives - 1;
    prevlives = lives;
  }

  if (lives < 1 or lives > 200) { //>200 because I had some uint overflow
    //Game over
  //  arduboy.clear();
  //  arduboy.setCursor(30,HEIGHT/2-15);
  //  arduboy.print("WIPEOUT");
  //  delay(400);

    prevscore = score;
    prevlives = 8;
    lives = 8;
    newlevelinit(1);
  }

  //Show the score and previous game's score
  arduboy.setCursor(w+4, 9);
  arduboy.print("SCORE:");
  score = (lvl - 1)*100 + numpixcomplete/41;
  arduboy.print(score);
  
  if (prevscore > 0) {
    arduboy.setCursor(w+4,30);
    arduboy.print("PREV SCORE");
    arduboy.setCursor(w+4,40);
    arduboy.print(prevscore);
  }

  //Show the number of lives (as images)
  for (byte i = 0; i < lives; i++) {
    arduboy.drawBitmap(w + 3 + 6*i,20,lifeimg,5,5,WHITE);
  }

  //Game logo
  arduboy.setCursor(w+22,50);
  arduboy.print("J");
  arduboy.setCursor(w+29,48);
  arduboy.print("E");
  arduboy.setCursor(w+35,46);
  arduboy.print("S");
  arduboy.setCursor(w+41,44);
  arduboy.print("S");
  arduboy.drawBitmap(w+49,43,ballimg,5,5,WHITE);

  arduboy.display();

}

