#include"Hero.h"
#include"Map.h"
#include"Util.h"
#include"Server.h"
#include"static_map.cpp"
const string heropath0 = "./resources/images/hero-0/";
const string heropath1 = "./resources/images/hero-1/";
const string heropath2 = "./resources/images/hero-2/";
const string heropath3 = "./resources/images/hero-3/";
extern MapNode MAP[MAPH][MAPW];

Hero::Hero()
{
	health = 100;
	magic = DISABLER;
	agile = false;
	speed = 2;
	att_lat = 0;
	moving = STOP;
	isAlive = true;
	healer = false;
	MapX = 0;
	MapY = 0;
	spriteIndex = 0;
	screenX = MAP[MapX][MapY].getScreenX();
	screenY = MAP[MapX][MapY].getScreenY();
	pathIndex = 1000;//set a large value to make moveHero return without update - to simulate the stop position
	HID = 0;
	TID = 0;
	power = 5;
	mana = 10;
	attacked = 0;

}

Hero::Hero(MAGICPOWER magic, bool agile,int X,int Y,int hid,int tid,int health,int speed,int power,int mana)
{
	this->magic = magic;
	this->agile = agile;
	this->spriteIndex = 0;
	this->MapX = X;
	this->MapY = Y;
	this->pathIndex = 1000;
	this->screenX = MAP[MapX][MapY].getScreenX();
	this->screenY = MAP[MapX][MapY].getScreenY();
	this->HID = hid;
	this->TID = tid;
	this->power = power;
	this->mana =mana;
	this->health = health;
	this->speed = speed;
	this->healer = false;
	this->speed_reduce_timer = 0;
	this->manaTimer = 0;
	this->attacked = 0;
	isAlive = true;
	moving = STOP;
}

	
void Hero::move(MOVEMENT m)
{
	if(m == N)
	{
		moving = N;
		moveN();
	}
	else if(m == S)
	{
		moving = S;
		moveS();
	}
	else if(m == E)
	{
		moving = E;
		moveE();
	}
	else if(m == W)
	{
		moving = W;
		moveW();
	}
	else if(m == NE)
	{
		moving = NE;
		moveNE();
	}
	else if(m == NW)
	{
		moving = NW;
		moveNW();
	}
	else if(m == SE)
	{
		moving = SE;
		moveSE();
	}
	else if(m == SW)
	{
		moving = SW;
		moveSW();
	}

	//check if current tile is gem - update hero params and disable gem
	if(MAP[MapX][MapY].getGem() != NONE)
	{
		if(MAP[MapX][MapY].getGem() == RED)
		{
			MAP[MapX][MapY].RedTimer = 2000;
			if(health +50 <= 250) health += 50;
			else health = 250;
		}
		else  if(MAP[MapX][MapY].getGem() == PURPLE)
		{
			MAP[MapX][MapY].PTimer = 10000;
			healer = true;
			healerCount = 4000;
		}
		else if(MAP[MapX][MapY].getGem() == LEMON)
		{
			MAP[MapX][MapY].LTimer = 1000;
			mana -= 1;
			manaCount = 2000;
		}
		else if(MAP[MapX][MapY].getGem() == BLUE)
		{
			MAP[MapX][MapY].BTimer = 2000;
			if ((speed * 2) <= 16) speed = speed * 2;
			else speed = 16;
		}
		else if(MAP[MapX][MapY].getGem() == YELLOW)
		{
			MAP[MapX][MapY].YTimer = 100;
			
		}
		else if(MAP[MapX][MapY].getGem() == GREEN)
		{
			MAP[MapX][MapY].GTimer = 2000;
			if(power+5 <= 25)
			{
				power += 5; 
			}
			else power = 25;
		}	

		MAP[MapX][MapY].setGem(NONE);
			
		//TODO  - update hero params
	}
}

void Hero::attack(int type,Hero& h)
{
	/*if(abs(h.MapX-MapX)>1 || abs(h.MapY - MapY)>1)
	{
		//chase!!
		path = direction(MapX,MapY,h.MapX,h.MapY);
			
	}*/
	cout<<"Attack happened:-"<<h.health<<"\n";
	
	
	if (type == 0)
	{
			if(abs(h.MapX - MapX)<= 3 && abs(h.MapY - MapY) <= 3)
			{
			
	 		//type 0 magic attack

			if(magic == DISABLER && manaTimer ==0)
			{
				h.attacked = 100;
				h.speed = 0;h.speed_reduce_timer = 100;
			}
			else if(magic == SLOWER && manaTimer ==0)
			{
				h.attacked = 100;
				h.speed /= 2; h.slower_timer = 100;
			}
			else if(magic == BURST && manaTimer ==0)
			{	
				h.attacked = 100;
				h.health /= 2;
				if(h.health <= 0)
				{
					h.health = 0;
					cout<<h.HID<<" dead\n";
					h.isAlive = false;
					h.spawnTimer = 1000;
					MAP[h.MapX][h.MapY].setType(FREE);
					dead(h.HID);
					
				h.health = STATIC_HERO_PROP[h.HID][4];
			
				h.speed = STATIC_HERO_PROP[h.HID][5];
			
				h.healer = false;
			
				h.power = STATIC_HERO_PROP[h.HID][6];
				h.mana = STATIC_HERO_PROP[h.HID][7];
			
				}	
			}
			else if(magic == STUNNER && manaTimer ==0)
			{
				h.attacked = 100;
				h.speed -=2;
				h.stuntime = 100;
				h.health -= 15;
				if(h.health <= 0)
				{
					h.health = 0;
					cout<<h.HID<<" dead\n";
					h.isAlive = false;
					h.spawnTimer = 1000;
					MAP[h.MapX][h.MapY].setType(FREE);
					dead(h.HID);
					h.health = STATIC_HERO_PROP[h.HID][4];
			
				h.speed = STATIC_HERO_PROP[h.HID][5];
			
				h.healer = false;
			
				h.power = STATIC_HERO_PROP[h.HID][6];
				h.mana = STATIC_HERO_PROP[h.HID][7];
				}	
			}
		}
	}

		else if(type == 1)
		{
			
			//cout<<"type to set ho rha hai:\n";
			if(abs(h.MapX - MapX)<= 1 && abs(h.MapY - MapY) <= 1)
			{
				h.attacked = 100;
				cout<<HID<<":Attacking Hero - "<<h.HID<<"\n"; 
				//type 1 melee attack
				h.health -= power;
				if(h.health <= 0)
				{
					h.health = 0;
					cout<<h.HID<<" dead\n";
					h.isAlive = false;
					h.spawnTimer = 1000;
					MAP[h.MapX][h.MapY].setType(FREE);
					dead(h.HID);
					h.health = STATIC_HERO_PROP[h.HID][4];
			
					h.speed = STATIC_HERO_PROP[h.HID][5];
			
					h.healer = false;
				
					h.power = STATIC_HERO_PROP[h.HID][6];
					h.mana = STATIC_HERO_PROP[h.HID][7];
			
				}
				
			//	cout<<" health is - "<<h.health<<"\n";
				//if dead take necessary actions	
			}
		}
		
			
}

void Hero::renderHero()
{
	//TODO - render the hero here
	if(!isAlive)
	{
		if(HID == 0 )
		{
			MapX =1;
			MapY = 22; 

		}
		else if(HID == 1)
		{
			MapX = 2;
			MapY = 24;
		}
		else if(HID == 2)
		{
			MapX = 24;
			MapY = 2;
		}
		else if(HID == 3)
		{
			MapX = 22;
			MapY = 1;
		}
		
		spawnTimer--;
		if(spawnTimer <= 0)
		{
			isAlive = true;
			alive(HID);
		}

		screenX = MAP[MapX][MapY].getScreenX();
		screenY = MAP[MapX][MapY].getScreenY();
	
		return;
	}
	if((attacked == 0) || ((attacked % 10) == 0) )
	HEROIMAGES[spriteIndex].render(screenX,screenY);
	MAP[MapX][MapY].setType(OBS);
	if( attacked > 0) attacked--;

}

void Hero::moveE()
{
	if(MapY == MAPW-1)
		return;

	if(screenX == MAP[MapX][MapY+1].getScreenX())
	{
		
		MAP[MapX][MapY].setType(FREE);
		MapY++  ; 
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		return;
	}
	screenX+= speed;
	spriteIndex++;
	if(!(spriteIndex >=6 && spriteIndex <= 8))
		spriteIndex = 6;
	
}

void Hero::moveW()
{
	if(MapY == 0)
		return;

	if(screenX == MAP[MapX][MapY-1].getScreenX())
	{
		MAP[MapX][MapY].setType(FREE);
		MapY--  ; 
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		return;
	}

	screenX-= speed;
	spriteIndex++;
	if(!(spriteIndex >=3 && spriteIndex <= 5))
		spriteIndex = 3;
	
}

void Hero::moveN()
{
	if(MapX == 0)
		return;

	if(screenY == MAP[MapX-1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX--  ;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		return;
	}
	screenY-= speed;
	spriteIndex++;
	if(!(spriteIndex >=9 && spriteIndex <= 11))
		spriteIndex = 9;
	
	
}

void Hero::moveS()
{
	if(MapX == MAPH-1)
		return;

	if(screenY == MAP[MapX+1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX++  ;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		return; 
	}
	screenY+= speed;
	spriteIndex++;
	if(!(spriteIndex >=0 && spriteIndex <= 2))
		spriteIndex =  0;
	
	

}

void Hero::moveSE()
{
	if(MapX == MAPH-1)
		return;
	if(MapY == MAPW-1)
		return;

	int flag = 0;	
	if(screenY == MAP[MapX+1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX++  ;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;
	}

	if(screenX == MAP[MapX][MapY+1].getScreenX())
	{
		MAP[MapX][MapY].setType(FREE);
		MapY++  ; 
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;
	}

	if(flag == 2){ flag = 0; return;}

	screenX+= speed;
	screenY+= speed;
	spriteIndex++;
	if(!(spriteIndex >=6 && spriteIndex <= 8))
		spriteIndex = 6;

}

void Hero::moveNE()
{
	if(MapX == 0)
		return;

	if(MapY == MAPW-1)
		return;
	
	int flag = 0;	
	if(screenY == MAP[MapX-1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX--;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;
	}

	if(screenX == MAP[MapX][MapY+1].getScreenX())
	{
		MAP[MapX][MapY].setType(FREE);
		MapY++;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;
	}

	if(flag == 2)
	{ flag = 0; return;}

	screenX+= speed;
	screenY-= speed;
	spriteIndex++;
	if(!(spriteIndex >=6 && spriteIndex <= 8))
		spriteIndex = 6;

}

void Hero::moveNW()
{
	if(MapX == 0)
		return;
	if(MapY == 0)
		return;
	int flag = 0;	

	if(screenY == MAP[MapX-1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX--;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;	
	}

	if(screenX == MAP[MapX][MapY-1].getScreenX())
	{
		MAP[MapX][MapY].setType(FREE);
		MapY--;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag ++;
	}

	if(flag == 2)
	{flag = 0;return;}

	screenX-= speed;
	screenY-= speed;
	spriteIndex++;
	if(!(spriteIndex >=3 && spriteIndex <= 5))
		spriteIndex = 3;

}

void Hero::moveSW()
{
	if(MapX == MAPH-1)
		return;
	if(MapY == 0)
		return;
	int flag = 0;	
	if(screenY == MAP[MapX+1][MapY].getScreenY())
	{
		MAP[MapX][MapY].setType(FREE);
		MapX++  ;
		MAP[MapX][MapY].setType(OBS);
		moving = STOP;
		flag++;
		//pathIndex++; //can we eliminate the use of STOP here
		//renderHero();
	}

	if(screenX == MAP[MapX][MapY-1].getScreenX())
	{
		MAP[MapX][MapY].setType(FREE);
		MapY-- ; 
		MAP[MapX][MapY].setType(OBS);
		//pathIndex++;//can we eliminate the use of STOP here
		moving = STOP;
		flag++;
		//renderHero();
		return;
	}

	if(flag == 2){flag = 0;	return;	}
	screenX-= speed;
	screenY+= speed;
	spriteIndex++;
	if(!(spriteIndex >=3 && spriteIndex <= 5))
		spriteIndex = 3;

	//renderHero();

}

bool Hero::loadTextures(int x)
{
	bool hero_t = true;
	string heropath;
	if(x == 0)
		heropath = heropath0;
	else if(x == 1)
		heropath = heropath1;
	else if(x == 2)
		heropath = heropath2;
	else if(x == 3)
		heropath = heropath3;

	int l = 0 ;
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			char a = i+'0';
			char b = j+'0';

			string filename = heropath ;
			filename+=a;
			filename+="-";
			filename+= b ;
			filename+=".png";
			hero_t = hero_t && HEROIMAGES[l++].loadTextureFromFile(filename);
		}
	}

	return hero_t;

}
