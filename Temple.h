#ifndef TEMPLE_H
#define TEMPLE_H

#include"allstdlib.h"
#include"Texture.h"

#define MAX_TEMPLE_HEALTH 1000

class Temple
{
	public:
	int health
	int TeamID;
	bool isAlive;
	Texture templeImg;

	Temple();

}
#endif
