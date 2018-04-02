#include"Map.h"
#include"Hero.h"
#include"static_map.cpp"

MapNode MAP[MAPH][MAPW];
Texture TILES[NTILES];
Texture ROCKIMG;
Texture TREEIMG;
Texture TEMPLEIMG[2];
Texture GEMS[6];
Texture G_WIN;
Texture CLOUD;
Texture G_LOSE;
Hero heroes[NHEROES];
int CURR , ALLY, OPP1 , OPP2; //IDs of different hero for this node
int TEMPLE_HEALTH[] = {1000,1000};
int isWin = 0;	//0 - going,1 - won, 2 - lost
//int TEMPLE_POS[][2] = {{5,2},{19,16}};
int SID; // system ID fetched from command line 
int TID;
int templeAttacked[2]={0,0};
const string path= "./resources/images/";
extern int N_HeroId;

bool loadHeroes()
{
	bool bheroes[4];
	for(int i =0 ; i < NHEROES; i++)
	{
		int HeroMapX = STATIC_HERO[i][0];
		int HeroMapY = STATIC_HERO[i][1];
		int magicid = STATIC_HERO_PROP[i][0]; 
		int agile = STATIC_HERO_PROP[i][1]; 
		int hid = STATIC_HERO_PROP[i][2]; 
		int tid = STATIC_HERO_PROP[i][3]; 
		int health = STATIC_HERO_PROP[i][4]; 
		int speed = STATIC_HERO_PROP[i][5]; 
		int power = STATIC_HERO_PROP[i][6]; 
		int mana = STATIC_HERO_PROP[i][7]; 
		
		MAGICPOWER magic;
		if(magicid == 0)
			magic = DISABLER;
		else if(magicid == 1)
			magic = SLOWER;
		else if(magicid == 2)
			magic = BURST;
		else if(magicid == 3)
			magic = STUNNER;
		bool bagile = (agile == 1)?true:false ;	

		heroes[i] =  Hero(magic,bagile,HeroMapX,HeroMapY,hid,tid,health,speed,power,mana);
		//heroes[i] =  Hero(BURST,true,HeroMapX,HeroMapY,0,0);//try initting heroes from a static array later
		bheroes[i] = heroes[i].loadTextures(i);
	}

	return bheroes[0] && bheroes[1] && bheroes[2] && bheroes[3]; 
}


bool loadTiles()
{
	bool tile0,tile1,tile3,tile2;
	tile0 = TILES[0].loadTextureFromFile(path+"tile0.png");
	tile1 = TILES[1].loadTextureFromFile(path+"tile1.png");
	tile2 = TILES[2].loadTextureFromFile(path+"tile2.png");
	tile3 = TILES[3].loadTextureFromFile(path+"tile3.png");

	return (tile0 && tile1 && tile2 && tile3);
}


bool loadRocks()
{
	
	return (ROCKIMG.loadTextureFromFile(path+"rock.png"));
}

bool loadTrees()
{
	return (TREEIMG.loadTextureFromFile(path+"tree.png"));
}

bool loadTemple()
{
	bool temple1,temple2;

	temple1 = TEMPLEIMG[0].loadTextureFromFile(path+"temple1.png");
	temple2 = TEMPLEIMG[1].loadTextureFromFile(path+"temple2.png");

	return temple1 && temple2;
}

bool loadGems()
{
	string file = path+"gem-";
	bool bGem = true;
	for(int i = 0 ; i < NGEMS ; i++)
	 {
		 file = path+"gem-";
		 char c = i + '0';
		 file += c;
		 file += ".png";
		 
		bGem = bGem && GEMS[i].loadTextureFromFile(file);
	 }
	return bGem;
}

bool loadTextures()
{
	bool tiles,tree,rock,temples,heroes,gems, g_win, g_lose,cl;
	string w = path+"win.png";
	string l = path+"lose.png";
	string c = path+"cloud.png";
	tiles = loadTiles();
	tree = loadTrees();
	rock = loadRocks();
	temples = loadTemple();
	heroes = loadHeroes();
	gems = loadGems();
	g_win = G_WIN.loadTextureFromFile(w);
	g_lose = G_LOSE.loadTextureFromFile(l);
	cl = CLOUD.loadTextureFromFile(l);
	return tiles && tree && rock && temples && heroes && gems;
}

void freeTiles()
{
	TILES[0].freeTexture();
	TILES[1].freeTexture();
	TILES[2].freeTexture();
	TILES[3].freeTexture();
}

MapNode::MapNode()
{
	MapX = 0;
	MapY = 0;
	TILEID = 0;
	type = FREE;
	screenX = 0.f;
	screenY = 0.f;
	Gem = NONE;
	RedTimer = GTimer = YTimer = LTimer = PTimer= BTimer = 0;
}

MapNode::~MapNode()
{
	MapX = 0;
	MapY = 0;
	TILEID = 0;

	screenX = 0.f;
	screenY = 0.f;
	Gem = NONE;

}

void MapNode::setGem(GEM_TYPE b)
{
	this->Gem = b;
}

GEM_TYPE MapNode::getGem()
{
	return this->Gem;
}

void MapNode::setType(TILE_TYPE type)
{
	this->type = type;

}

TILE_TYPE MapNode::getType()
{
	return this->type;
}
void MapNode::setMapX(int x)
{
	this->MapX = x;
}

void MapNode::setMapY(int x)
{
	this->MapY = x;
}

void MapNode::setTILEID(int x)
{
	this->TILEID = x;
}

int MapNode::getMapX()
{
	return MapX;
}

int MapNode::getMapY()
{
	return MapY;
}


int MapNode::getTILEID()
{
	return TILEID;
}

void MapNode::setScreenY(GLfloat x)
{
	this->screenY = x;
}

void MapNode::setScreenX(GLfloat x)
{
	this->screenX = x;
}

GLfloat MapNode::getScreenX()
{
	return screenX;
}

GLfloat MapNode::getScreenY()
{
	return screenY;
}

void generateMap()
{
	//Generte Game lose win images
	
	
	//Calc positions of static gems
	//Gems are statically placed and don't change
	
	//Generate TILE pattern to generate the map
	for(int i = 0; i<MAPH ; i++)
		for(int j = 0; j < MAPW ; j++)
		{
			MAP[i][j].setMapX(i);
			MAP[i][j].setMapY(j);
			MAP[i][j].setTILEID(STATIC_MAP[i][j]);
			MAP[i][j].setScreenX(j*32);
			MAP[i][j].setScreenY(i*32);

		}

	//once tile map is ready, set the screen coordinates of hero
	for(int i = 0 ; i < NHEROES;i++)
	{
		heroes[i].screenX = MAP[heroes[i].MapX][heroes[i].MapY].getScreenX();//FIXME - can find a better method of initializing
		heroes[i].screenY = MAP[heroes[i].MapX][heroes[i].MapY].getScreenY();
	}

	//iterate over RED GEM map 
	for(int i = 0 ; i < 2; i ++)
	{
		int Y = STATIC_RGEM[i][0];
		int X = STATIC_RGEM[i][1];
		MAP[X][Y].setGem(RED);
	}

	//iterate over BLUE GEM map 
	for(int i = 0 ; i < 2; i ++)
	{
		int Y = STATIC_BGEM[i][0];
		int X = STATIC_BGEM[i][1];
		MAP[X][Y].setGem(BLUE);
	}
	
	//iterate over GREEN GEM map 
	for(int i = 0 ; i < 2; i ++)
	{
		int Y = STATIC_GGEM[i][0];
		int X = STATIC_GGEM[i][1];
		MAP[X][Y].setGem(GREEN);
	}
	
	//iterate over  LEMON GEM map 
	for(int i = 0 ; i < 2; i ++)
	{
		int Y = STATIC_LGEM[i][0];
		int X = STATIC_LGEM[i][1];
		MAP[X][Y].setGem(LEMON);
	}

	//iterate over YELLOW GEM map 
	for(int i = 0 ; i < 2; i ++)
	{
		int Y = STATIC_YGEM[i][0];
		int X = STATIC_YGEM[i][1];
		MAP[X][Y].setGem(YELLOW);
	}

	//iterate over PURPLE GEM map
	int Y = STATIC_PGEM[0][0];
	int X = STATIC_PGEM[0][1];
	MAP[X][Y].setGem(PURPLE);
	cout<<"PGEM X-"<<MAP[X][Y].getScreenX()<<"\n";
	cout<<"PGEM Y-"<<MAP[X][Y].getScreenY()<<"\n";
	cout<<"PGEM Map X-"<<X<<"\n";
	cout<<"PGEM Map Y-"<<Y<<"\n";

}

void renderGems(GEM_TYPE g,GLfloat sX,GLfloat sY)
{
	int index = 0 ; 
	if(g == BLUE)
		index = 0;
	else if(g == GREEN)
		index = 1;
	else if(g == PURPLE)
		index = 2;
	else if(g == LEMON)
		index = 3;
	else if(g == RED)
		index = 4;
	else if(g == YELLOW)
		index = 5;

	GEMS[index].render(sX,sY);
}


void renderMap()
{
	if(isWin == 0)
	{
		for(int i = 0 ; i < MAPH; i++)
			for(int j = 0 ; j < MAPW ; j++)
			{
				int index = MAP[i][j].getTILEID();
				GLfloat sX = MAP[i][j].getScreenX();
				GLfloat sY = MAP[i][j].getScreenY();
				 
				TILES[index].render(sX,sY);

				if(MAP[i][j].getGem() != NONE)
				 {
					GEM_TYPE g = MAP[i][j].getGem();
					renderGems(g,sX,sY);	
				 }

				 if(MAP[i][j].RedTimer > 0 )
				 {
					MAP[i][j].RedTimer--;
					if(MAP[i][j].RedTimer == 0)
						MAP[i][j].setGem(RED);
					
				 }
				 if(MAP[i][j].PTimer > 0 )
				 {
					MAP[i][j].PTimer--;
					if(MAP[i][j].PTimer == 0)
						MAP[i][j].setGem(PURPLE);
					
				 }
				 if(MAP[i][j].GTimer > 0 )
				 {
					MAP[i][j].GTimer--;
					if(MAP[i][j].GTimer == 0)
						MAP[i][j].setGem(GREEN);
					
				 }
				 if(MAP[i][j].YTimer > 0 )
				 {
					MAP[i][j].YTimer--;
					if(MAP[i][j].YTimer == 0)
						MAP[i][j].setGem(YELLOW);
					
				 }
				 if(MAP[i][j].BTimer > 0 )
				 {
					MAP[i][j].BTimer--;
					if(MAP[i][j].BTimer == 0)
						MAP[i][j].setGem(BLUE);
					
				 }
				 if(MAP[i][j].LTimer > 0 )
				 {
					MAP[i][j].LTimer--;
					if(MAP[i][j].LTimer == 0)
						MAP[i][j].setGem(LEMON);
					
				 }
				 for(int i=0;i<4;i++)
				 {
				 	if(heroes[i].healer && heroes[i].healerCount > 0)
				 	{
				 		heroes[i].healerCount--;
				 	}
				 	else if(heroes[i].healer)
				 	{
				 		heroes[i].healer = false;	
				 	}
				 	if(heroes[i].mana != STATIC_HERO_PROP[i][7] && heroes[i].manaCount > 0)
				 	{
				 		heroes[i].manaCount--;
				 	}
				 	else if(heroes[i].mana != STATIC_HERO_PROP[i][7])
				 	{
				 		heroes[i].mana = STATIC_HERO_PROP[i][7];
				 	}
				 	
				 	for(int i = 0; i < 4; i++)
				 	{
				 		if(heroes[i].speed_reduce_timer > 0)
				 		{
				 			heroes[i].speed_reduce_timer--;
				 			if(heroes[i].speed_reduce_timer == 0)
				 			{
				 				heroes[i].speed = STATIC_HERO_PROP[i][5];
				 			}
				 		}
				 		
						
						if(heroes[i].slower_timer > 0)
						{
							heroes[i].slower_timer--;
							if(heroes[i].slower_timer == 0)
							{
								heroes[i].speed = STATIC_HERO_PROP[i][5];
							}
						}
						
						
						if(heroes[i].stuntime > 0)
						{
							heroes[i].stuntime--;
							if(heroes[i].stuntime == 0)
							{
								heroes[i].speed = STATIC_HERO_PROP[i][5];
							}
						}
						
						
						if(heroes[i].manaTimer > 0)
						{
							heroes[i].manaTimer--;
							if(heroes[i].manaTimer == 0)
							{
								heroes[i].mana = STATIC_HERO_PROP[i][7];
							}
						}
						
					}
				 }
				 

			}
	

		//Render Rocks	
		for(int i = 0 ; i < NROCKS; i++)
		{
			int RockY = STATIC_ROCKS[i][0];
			int RockX = STATIC_ROCKS[i][1];

			GLfloat sX = MAP[RockX][RockY].getScreenX();
			GLfloat sY = MAP[RockX][RockY].getScreenY();

			MAP[RockX][RockY].setType(OBS);

			ROCKIMG.render(sX,sY);
		}	

		//Render Heroes


		for(int i = 0 ; i < NHEROES; i++)
		{
			heroes[i].renderHero();
		}
		//Render Trees
		for(int i = 0 ; i < NTREES; i++)
		{
			int TreeY = STATIC_TREES[i][0];
			int TreeX = STATIC_TREES[i][1];

			GLfloat sX = MAP[TreeX][TreeY].getScreenX();
			GLfloat sY = MAP[TreeX][TreeY].getScreenY();

			MAP[TreeX+3][TreeY+1].setType(OBS);
			MAP[TreeX+3][TreeY+2].setType(OBS);

			TREEIMG.render(sX,sY);
		}	

		//Render Temples

		
		
		if(TID == 0)
		{
			for(int i = 1 ; i <= 24; i++)
				for(int j = 0 ; j < i ; j++)
				{
					GLfloat sX = MAP[i][j].getScreenX();
					GLfloat sY = MAP[i][j].getScreenY();
					TILES[0].render(sX,sY);
					
				
				}
		}
		else if(TID == 1)
		{
			for(int i = 0 ; i <= 24; i++)
				for(int j = i+5 ; j <= 24; j++)
				{
					GLfloat sX = MAP[i][j].getScreenX();
					GLfloat sY = MAP[i][j].getScreenY();
					TILES[0].render(sX,sY);
					
				
				}
		}
		
		for(int i = 0 ; i < NTEMPLES; i++)
		{
			int TempleY = STATIC_TEMPLE[i][0];
			int TempleX = STATIC_TEMPLE[i][1];
			//cout<<"Temple MapX - "<<TempleX<<"\n";
			//cout<<"Temple MapY - "<<TempleY<<"\n";

			GLfloat sX = MAP[TempleX][TempleY].getScreenX();
			GLfloat sY = MAP[TempleX][TempleY].getScreenY();
		
			for(int j=1; j <= 3; j++)
				for(int k=0; k <= 3 ; k++)
					MAP[TempleX + j] [TempleY + k ].setType(OBS);

			if((templeAttacked[i] == 0) || ((templeAttacked[i] % 10) == 0))
			TEMPLEIMG[i].render(sX,sY);
			
			if(templeAttacked[i] > 0) templeAttacked[i]--;
		}
	}
	else if(isWin == 1)
	{
		G_WIN.render(144,144);
	}
	
	else if(isWin == 2)
	{
		G_LOSE.render(144,144);
	}
	
	else
	{
		cout<<"WTF just happened!?\n";
	}
	
	
}

void moveHero(int x)//TODO - do this for other heroes with some id as parameter
{
	// move CURR hero of this node
	if(!heroes[x].isAlive)
	{
		//heroes[x].path.assign(0,STOP);
		//heroes[x].pathIndex = 1000;
		return;
	}

	if(heroes[x].pathIndex >= heroes[x].path.size())
	{
		heroes[x].pathIndex = 1000;
		return;
	}

	//MOVEMENT path[] = {S,E,S};		
	//hero1.path.assign(path,path+3);//initialize hero path sequence

	heroes[x].move(heroes[x].path[heroes[x].pathIndex]);
	//cout<<" hero - MapX- " <<heroes[x].MapX<<"\n";
	//cout<<" hero - MapY- "<<heroes[x].MapY<<"\n";
	
	if(heroes[x].moving == STOP)
		heroes[x].pathIndex++;
}

void updateMap()
{
	//give movement sequence
	moveHero(0);
	moveHero(1);
	moveHero(2);
	moveHero(3);
}

int Screen2TileY(GLfloat y)
{
	return ((int)y / TILEW);
}

int Screen2TileX(GLfloat x)
{
	return ((int)x / TILEW);
}

