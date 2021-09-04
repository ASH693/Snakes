#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING // Needed with c++17

#include <iostream>
#include <string>
#include "Snake.h"
#include "Fruit.h"
#include "textpixels_enums.h"
#include "textpixels.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;
using namespace textpixels;

// A collection of screens we want in game
enum Screen         
{
  MENU = 1,
  PLAY,
  PAUSE,
  GAMEOVER,
  EXIT,
  NONE
};

//constants
const int LEVEL_WIDTH = 35;
const int LEVEL_HEIGHT = 20;
const int GUI_HEIGHT = 15;
const int WALL_LEFT = 0;
const int WALL_SAFE_LEFT = 1;
const int WALL_RIGHT = LEVEL_WIDTH - 1;
const int WALL_TOP = 0;
const int WALL_SAFE_TOP = 1;
const int WALL_BOTTOM = LEVEL_HEIGHT - 1;

// Function declarations
void drawLevelAndGui(Snake snake, int score);
int displayMenuScreen();
int levelMode();
int playSnake(int mode);

/* ------------------------------------------------------------------------- MAIN */

int main()
{

  // textpixels creates our 30x30 console window
  textpixels::setupConsole( LEVEL_WIDTH, LEVEL_HEIGHT+GUI_HEIGHT, 20, 20 );  
  // I chose `screen` as a term for part of the program with its own visuals+controls
  int screen = Screen::MENU;    // Start the game at the menu.
 
  
  while (screen != Screen::EXIT)   // Outermost loop of the whole program
  {
    if (screen == Screen::MENU)
    {
      screen = displayMenuScreen(); // displays menu screen
    }
    else if (screen == Screen::PLAY)  
    {
        levelMode(); // display level selection menu   
        screen = Screen::MENU;    // Requesting menu as the next screen after game
    }
  }
  // Add some sort of exit screen
  textpixels::exit();   // Tell textpixels to clean up after itself.
} 

/* -------------------------------------------------------- FUNCTION DEFINITIONS */

// Draw the Level's ground and border wall to top of window,
// draw gui with score and any other info at bottom.
void drawLevelAndGui(Snake snake, int score)
{   
  fillWindow(DARK_GREEN);
  fillRect(1, 1, LEVEL_WIDTH - 2, LEVEL_HEIGHT - 2, BLACK);
  // Draw the gui background, starting at bottom of level
  fillRect(0, LEVEL_HEIGHT, LEVEL_WIDTH, GUI_HEIGHT, BLACK);

  // display score as string 
  string scoreAsString = to_string(score);
  drawString(LEVEL_WIDTH - 11, 0, "SCORE:", layerColours(FG_YELLOW, BG_DARK_GREEN));
  drawString(LEVEL_WIDTH - 5, 0, scoreAsString, layerColours(FG_YELLOW, BG_DARK_GREEN));

  // If the snake hits a wall,display message 
  // telling player he got hurt
  if (snake.snakeHitWall)
  {
      textpixels::setFps(1);
      drawString(6, 8, " zzz YOU GOT ZAPPED zzz", layerColours(FG_WHITE, BG_RED));
      drawString(8, 9, " YOUR SCORE: ", layerColours(FG_WHITE, BG_RED));
      drawString(22, 9, scoreAsString, layerColours(FG_WHITE, BG_RED));
  }

  //tell player they got hure when they hit an obsticle
  if (snake.hitPoison)
  {
      textpixels::setFps(1);
      drawString(6, 8, " %% YOU GOT SHREDDED%%", layerColours(FG_WHITE, BG_RED));
      drawString(8, 9, " YOUR SCORE: ", layerColours(FG_WHITE, BG_RED));
      drawString(22, 9, scoreAsString, layerColours(FG_WHITE, BG_RED));
  }

  // draw in GUI
  drawString(2, LEVEL_HEIGHT + 1, "(Q) ", FG_CYAN);
  drawString(6, LEVEL_HEIGHT + 1, "QUIT", FG_GREY);
  drawString(3, LEVEL_HEIGHT + 3, "(", layerColours(FG_DARK_YELLOW, BG_YELLOW));
  drawString(6, LEVEL_HEIGHT + 3, "A BANANA + 10        W ", FG_GREY);
  drawString(3, LEVEL_HEIGHT + 5, "@", layerColours(FG_DARK_RED, BG_RED));
  drawString(6, LEVEL_HEIGHT + 5, "AN APPLE + 5     A   S   D", FG_GREY);
  drawString(3, LEVEL_HEIGHT + 7, "X", layerColours(FG_YELLOW, BG_DARK_GREEN));
  drawString(6, LEVEL_HEIGHT + 7, "BAD FRUIT - 5    DIRECTIONS ", FG_GREY);
  drawString(3, LEVEL_HEIGHT + 9, "#", layerColours(FG_YELLOW, BG_BLACK));
  drawString(6, LEVEL_HEIGHT + 9, "SPIKE     KILL ", FG_GREY);
  }

// Responsible for core snake game loop: listening for input,
// simulating changes in the game world, drawing the new state to screen.
// Returns score 
int playSnake(int mode)
{

  textpixels::setFps(200);      
  bool playerHasQuit = false;
  int score = 0;
  srand(time(0)); // seed for random

  //snake
  Snake slithers;            
  slithers.colour = CYAN;

  //snake speed according to level
  if (mode == 1) {
      slithers.moveInterval = 40;
  }
  else if (mode == 2) {
      slithers.moveInterval = 30;
  }

  slithers.location = Point2d(10, 5);  
  slithers.xDir = Direction::NONE;
  slithers.yDir = Direction::DOWN;
   
  //banana
  Fruit banana;
  banana.colour = textpixels::YELLOW;
  banana.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP); // generate in random location
  banana.pointsValue = 10;

  //apple
  Fruit apple;
  apple.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
  apple.colour = textpixels::RED;
  apple.pointsValue = 5;

  //bad fruit
  Fruit rotten;
  rotten.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
  rotten.colour = textpixels::DARK_GREY;
  rotten.pointsValue = -5;

  //collection of spikes obstacles
  vector<Fruit> poison;
  for (int i = 1; i < 5; i++)
  {
      Fruit temppoison;
      temppoison.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);  
      poison.push_back(temppoison);  // A copy of the temp banana will survive in the vector
  }
  
  do
  {
    textpixels::startFrame();  
    /* ---------------------------------------------------------------------- INPUT */
    if (keyIsDown('Q'))
    {
      playerHasQuit = true;
      break;  // End the DO loop now, go to the line after it ends (return score?)
    }

    // Check if a, d, left/right arrow is pressed, store it 
    Direction xDirectionInput = textpixels::getLeftRightInput();
    Direction yDirectionInput = textpixels::getUpDownInput();

    /* ---------------------------------------------------------------- PROCESSING */

    if (xDirectionInput != Direction::NONE)
    {
      // Change our snake direction to match the direction that was entered by the player
      slithers.xDir = xDirectionInput;
      slithers.yDir = Direction::NONE;
    }  
    else if (yDirectionInput != Direction::NONE)   //did they press a y direction??
    {
      slithers.xDir = Direction::NONE;
      slithers.yDir = yDirectionInput;
    }
    
    // If the snake's moveInterval divides evenly into the current frame count, move her.
    if (textpixels::frame() % slithers.moveInterval == 0)  
    {
      slithers.location.x += slithers.xDir; // xDir is -1, 0 or 1 (see Direction enum) 
      slithers.location.y += slithers.yDir;
      
      //check if snake hit walls for easy mode and teleport it to other side
      if (slithers.location.x == WALL_LEFT && mode == 1 ) {
         slithers.location.x = WALL_RIGHT - 1;
      }
       else if (slithers.location.x == WALL_RIGHT && mode == 1) {   
          slithers.location.x = WALL_SAFE_LEFT;
      }
      else if (slithers.location.y == WALL_TOP && mode == 1) {    
         slithers.location.y = WALL_BOTTOM - 1;
      }
      else if (slithers.location.y == WALL_BOTTOM && mode == 1) { 
         slithers.location.y = WALL_SAFE_TOP;
      }

      //check if snake hit wall for hard mode and kill it
      else if (slithers.location.x == WALL_LEFT && mode == 2) {
          slithers.snakeHitWall = true;
          playerHasQuit = true;
      }
      else if (slithers.location.x == WALL_RIGHT && mode == 2) {
          slithers.snakeHitWall = true;
          playerHasQuit = true;
      }
      else if (slithers.location.y == WALL_TOP && mode == 2) {
          slithers.snakeHitWall = true;
          playerHasQuit = true;
      }
      else if (slithers.location.y == WALL_BOTTOM && mode == 2) {
          slithers.snakeHitWall = true; 
          playerHasQuit = true;
      }
      
      //check for collisions
      for (int i = 1; i < 4; ++i) {

          if (slithers.location == banana.location) {
              score = score + banana.pointsValue;
              slithers.moveInterval = slithers.moveInterval - 0.2;
              banana.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);

              if (banana.location == slithers.location || banana.location == apple.location || banana.location == poison[i].location || banana.location == rotten.location) {
                  banana.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
              }
          }

          else if (slithers.location == apple.location) {
              score = score + apple.pointsValue;
              slithers.moveInterval = slithers.moveInterval - 0.2;
              apple.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);

              if (apple.location == slithers.location || apple.location == banana.location || apple.location == poison[i].location || apple.location == rotten.location) {
                  apple.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
              }
          }

          else if (slithers.location == rotten.location) {
              score = score + rotten.pointsValue;
              rotten.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
              if (rotten.location == slithers.location || rotten.location == banana.location || rotten.location == poison[i].location || rotten.location == apple.location) {
                  rotten.location = Point2d(rand() % (WALL_RIGHT - 1) + WALL_SAFE_LEFT, rand() % (WALL_BOTTOM - 1) + WALL_SAFE_TOP);
              }
          }

          else if (slithers.location == poison[i].location) {
              slithers.hitPoison = true;
              playerHasQuit = true;
          }
      }
    }
    /* ---------------------------------------------------------------- OUTPUT */
   
    // draw everything in the game
    drawLevelAndGui(slithers, score);
    drawString(banana.location.x, banana.location.y, "(", layerColours(FG_DARK_YELLOW, BG_YELLOW));
    drawPixel(slithers.location, slithers.colour);
    drawString(apple.location.x, apple.location.y, "@", layerColours(FG_DARK_RED, BG_RED));
    drawString(rotten.location.x, rotten.location.y, "X", layerColours(FG_YELLOW, BG_DARK_GREEN));

    //draw spike
    for (int i = 1; i < 4; ++i) {
        drawString(poison[i].location.x, poison[i].location.y, "#", layerColours(FG_YELLOW, BG_BLACK));
    }
    
    if (mode == 1) {   
        drawString(6, LEVEL_HEIGHT + 12, "!!WALK THROUGH WALLS!!", layerColours(FG_YELLOW, BG_BLACK));
    }

    else if (mode == 2) {  
        drawString(4, LEVEL_HEIGHT + 12, "!!BEWARE OF ELECTRIC WALLS!!", layerColours(FG_YELLOW, BG_BLACK));
    }
    textpixels::endFrame();  
  } while (!playerHasQuit); 
  return score; // Return placeholder score for now
}

// Draws the menu screen, waits for user input.
// Returns the player's choice using a constant from enum Screen.
int displayMenuScreen()
{
  // 60fps is plenty fast for a menu.
  textpixels::setFps();
  int choice = Screen::NONE;
  do                            // Keeps looping, waiting for input
  {
    textpixels::startFrame();   // Needed always at start of game loop
    fillWindow(BLACK);
  
   // draw snake in main menu
    for (int x = 12; x < 24; x++) {
        textpixels::drawCharacter(x, 12, '-', layerColours(FG_YELLOW, BG_DARK_GREEN));
    }
    drawPixel(11, 12, DARK_YELLOW);
    drawString(24, 12, ":", layerColours(FG_YELLOW, BG_GREEN));
    drawString(11, 15, "(P) Play Snake", layerColours(FG_WHITE, BG_BLACK));
    drawString(11, 18, "(X) Exit", layerColours(FG_WHITE, BG_BLACK));

   //check for key presses
    if (keyIsDown('P'))
    {
      choice = Screen::PLAY;
    }
    else if (keyIsDown('X'))
    {
      choice = Screen::EXIT;
    }
    textpixels::endFrame();     // Needed always at end of game loop.
  } while (choice == Screen::NONE);     // Only stop when playerHasQuit  
  return(choice);
}

//draw the select level menu
//
int levelMode()
{
    // 60fps is plenty fast for a menu.
    textpixels::setFps(200);
    int choice = Screen::NONE;
    int score = 0;
    int mode = 0;

    do                            // Keeps looping, waiting for input
    {
        textpixels::startFrame();   // Needed always at start of game loop
        fillWindow(BLACK);

        for (int x = 12; x < 24; x++) {
            textpixels::drawCharacter(x, 12, '-', layerColours(FG_YELLOW, BG_DARK_GREEN));
        }
        drawPixel(11, 12, DARK_YELLOW);
        drawString(24, 12, ":", layerColours(FG_YELLOW, BG_GREEN));
        drawString(11, 15, "(E) Play EASY", layerColours(FG_WHITE, BG_BLACK));
        drawString(11, 18, "(H) GO HARD", layerColours(FG_WHITE, BG_BLACK));

        // WARNING: displayMenuScreen will end in milliseconds or less, and the user
        // will still have the key down. Whatever screen shows after this should avoid
        // using P or X keys as input or they'll instantly trigger.. unless you can think
        // of a fix?
        if (keyIsDown('E'))
        {
            
             mode = 1;
            mode = playSnake(mode);
            choice = Screen::MENU;

        }
        else if (keyIsDown('H'))
        {
            mode = 2;
            mode = playSnake(mode);
            choice = Screen::MENU;
        }
        textpixels::endFrame();     // Needed always at end of game loop.
    } while (choice == Screen::NONE);     // Only stop when playerHasQuit  
    return(mode);
}