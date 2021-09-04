#ifndef __SNAKE__
#define __SNAKE__
#include "Point2d.h"
#include "textpixels_enums.h"
using textpixels::Point2d;
using textpixels::Direction;

class Snake
{
public:

	// Position and speed use data types/enums are defined in textpixels_enums.h

	//----movement------//
  Point2d location { 1,1 };
  int xDir = Direction::NONE;
  int yDir = Direction::NONE;
  //move on every nth frame
  int moveInterval = 10;


  //---other properties---//
  bool snakeHitWall = false;
  short colour = 0x0;
  bool hitPoison = false;
};

#endif

