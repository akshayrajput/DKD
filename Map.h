#ifndef MAP_H
#define MAP_H

#include"allstdlib.h"
#include"Texture.h"
#define TILEW 32
#define MAPW 25
#define MAPH 25
#define NTILES 4
#define NTEMPLES 2
#define NGEMS 6

bool loadTiles();
bool loadRocks();
bool loadTrees();
bool loadTextures();
void freeTiles();
void moveHero();
int Screen2TileY(GLfloat);
int Screen2TileX(GLfloat); 

enum TILE_TYPE {FREE=0,OBS,ITEM};
enum GEM_TYPE {NONE = 0,RED,BLUE,PURPLE,GREEN,YELLOW,LEMON};

class MapNode
{
	private:
	int TILEID ; //the TileNo. stored here
	int MapX; // Map X   - measured in no. of tiles 
	int MapY;// Map Y - measured in no. of tiles
	TILE_TYPE type;//whether current tile is free/obstacle/or has a gem	
	
	GLfloat screenX; // screen co-ordinate where the mapnode is  shown
	GLfloat screenY; // screen co-ordinate where the mapnode is shown 

	GEM_TYPE Gem; // true if gem is present at this node	
	public:
	//Timers
	
	int LTimer;
	int GTimer;
	int BTimer;
	int RedTimer;
	int PTimer;
	int YTimer;
	//ctors
	MapNode();
	~MapNode();

	//getter-setter
	void setMapX(int);
	void setMapY(int);
	void setScreenX(GLfloat);
	void setScreenY(GLfloat);
	void setTILEID(int);
	void setType(TILE_TYPE);
	void setGem(GEM_TYPE);

	TILE_TYPE getType();
	int getMapX(void);
	int getMapY(void);
	int getTILEID(void);
	GEM_TYPE getGem(void);
	GLfloat getScreenX(void);
	GLfloat getScreenY(void);

};

void generateMap();
void updateMap();
void renderMap();

#endif
