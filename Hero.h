#ifndef HERO_H
#define HERO_H

#include"allstdlib.h"
#include"Texture.h"

#define NHEROES 4

enum MAGICPOWER { DISABLER=0,SLOWER,BURST,STUNNER};
enum MOVEMENT {STOP = 0,N,S,E,W,NE,NW,SE,SW};

class Hero
{
	public:
	int health;
	MAGICPOWER magic;
	bool agile;//if true then agile else heallthy
	int speed;//speed of movement
	int att_lat;//the reduction factor in attack increases after every 2s
	Texture HEROIMAGES[12];
	MOVEMENT moving;
	bool isAlive;
	int spriteIndex;
	int MapX;
	int MapY;
	int S_MapX;
	int S_MapY;
	vector<MOVEMENT> path;
	int pathIndex;
	GLfloat screenX;
	GLfloat screenY;
	int power;
	int mana;
	bool healer;
	int healerCount;
	int manaCount;
	int spawnTimer;//for counting amount of time before next spawn
	int manaTimer;//for counting amount of time before next attack
	int HID;//hero id
	int TID;// team id
	Hero();
	Hero(MAGICPOWER,bool,int,int,int,int,int,int,int,int );//MAGICPOWER , agility as input
	void move(MOVEMENT);// to be called by update
	void attack(int,Hero&);//attack type,victim hero id as parameter
	void renderHero();//to be called by render
	bool loadTextures(int);
	int speed_reduce_timer;
	int slower_timer;
	int stuntime;
	int attacked;

	private:

	void moveN();
	void moveS();
	void moveE();
	void moveW();
	void moveSE();
	void moveSW();
	void moveNE();
	void moveNW();

};	

#endif
