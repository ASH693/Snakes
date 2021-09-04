#ifndef __FRUIT__
#define __FRUIT__
#include "Point2d.h"
#include "textpixels_enums.h"
using textpixels::Point2d;
using textpixels::Direction;

class Fruit
{
public:

	// Position and speed use data types/enums are defined in textpixels_enums.h

	//----movement------//
	Point2d location{ 1,1 };
	short colour = 0x0;
	
	std::string kind = "";
	int pointsValue = 1;
	int growSnakeBy = 1; //how much must a snake grow after eating
	
 
	
};

#endif


