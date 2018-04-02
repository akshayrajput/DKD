//initialize GL and loads files

#include "Util.h"
#include "Texture.h"
#include"Map.h"
#include"Hero.h"
#include "direction.cpp"
#ifndef SERVER_H
#include"Server.h"
#endif
int textureIndex =0;
/* from Map.cpp */
extern node mySelf;
extern node table1[3];
extern bool alive1[3];
extern Hero heroes[NHEROES];
extern int CURR,ALLY,OPP1,OPP2;
extern int SID ;//Forgot!!
extern int TID;//Team ID
extern int isWin;
extern int TEMPLE_HEALTH[2];
extern int templeAttacked[2];
/* from Map.cpp */
//following stores (basically gives the destination X,Y of the player)
int ClkX;
int ClkY;
int Mp_X;//corresponding Mp_X for Map - corresponds to sY
int Mp_Y;//corresponding Mp_Y for Map - corresponds to sX

//TODO - code for networking
//NWhandler handlerC; // Client thread 
//NWhandler handlerS; // Server thread
//NWhandler Table[4];
int NW_x;//network hero mapX
int NW_y;//network hero mapY
int N_HeroId;
int N_OppId;//used only for attack
int x = 0;
//make connection and join/create network
bool initNW()
{
	
	
	for(int i = 0; i < 3; i++)
	{
		while(!table1[i].taken);
	}
	return true;
}
void initID(int x)
{

	CURR = x;
	if(CURR == 0 || CURR == 1)
		TID = 0;
	else
		TID = 1;

	if(CURR == 0)
	{
		ALLY = 1;
		OPP1 = 2;
		OPP2 = 3;
	}
	else if(CURR == 1)
	{
		ALLY = 0;
		OPP1 = 2;
		OPP2 = 3;
	}
	else if(CURR == 2)
	{
		ALLY = 3;
		OPP1 = 0;
		OPP2 = 1;
	}
	else if(CURR == 3)
	{
		ALLY = 2;
		OPP1 = 0;
		OPP2 = 1;
	}
	else
	{
		cout<<"What are you doing?\n";
	}

	mySelf.id = CURR;
	cout<<CURR<<" "<<ALLY<<" "<<OPP1<<" "<<OPP2<<"\n";

}

//load the OpenGL parts
bool initGL()
{
    //Set the viewport
    //glViewport( 0.f, 0.f, SCREEN_WIDTH,SCREEN_HEIGHT);

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH,SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    glViewport( 0.f, 0.f, SCREEN_WIDTH,SCREEN_HEIGHT);
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

//load all pics/sounds etc
bool loadMedia()
{
    //Load textures

    if(! loadTextures())
    {
	    printf("Unable to load textures!\n");
	    return false;
    }

    generateMap();

        return true;
}

void update()
{
	updateMap();
}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    renderMap();
       //Update screen
    glutSwapBuffers();
}

void mouseHandler(int Btn,int state,int x,int y)
{
	Mp_X = Screen2TileX(y);
	Mp_Y = Screen2TileY(x);
	
	if(TID == 0)
	{
		if(Mp_X > Mp_Y)
			return;
	}
	else if(TID == 1)
	{
		if((Mp_X +5) <= Mp_Y )
			return;
	}
	if(state == GLUT_DOWN && Btn == GLUT_RIGHT_BUTTON)
	{
	
		
		input();
	}
	
	else if(state == GLUT_DOWN && Btn == GLUT_LEFT_BUTTON)
	{

		handleAttack(1);
			
		
	}
	
	else if(state == GLUT_DOWN && Btn == GLUT_MIDDLE_BUTTON)
	{
		
		cout<<"Attack with Magic!!! \n";
		heroes[CURR].manaTimer = heroes[CURR].mana * 50;
		handleAttack(0);	
	}
	
}

void input()
{
	
		if(!heroes[CURR].isAlive)
			return;
		
		heroes[CURR].pathIndex = 1000;
		heroes[CURR].path.assign(0,STOP);
		heroes[CURR].path = direction(heroes[CURR].MapX,heroes[CURR].MapY,Mp_X,Mp_Y);
		heroes[CURR].pathIndex = 0;//basically make hero follow the path sequence
		
		sendHeroStatus(CURR,Mp_X,Mp_Y,heroes[CURR].health,heroes[CURR].speed,heroes[CURR].power,heroes[CURR].mana);

	
}

void* inputS(void * x)
{
	
		int *id = (int *)x;
		cout<<*id<<"\n";
		heroes[(*id)].pathIndex = 1000;
		heroes[(*id)].path.assign(0,STOP);
		heroes[(*id)].path = direction(heroes[N_HeroId].MapX,heroes[N_HeroId].MapY,NW_x,NW_y);
		heroes[(*id)].pathIndex = 0;//basically make hero follow the path sequence	
		
		int status;
		pthread_exit(&status);
	
}

void handleAttack(int type)
{
	//cout<<"Handle attack called\n";
	if(Mp_X == heroes[OPP1].MapX && Mp_Y == heroes[OPP1].MapY)
	{
		heroes[CURR].attack(type,heroes[OPP1]); sendAttack(type, CURR, OPP1);
	}
	else if(Mp_X == heroes[OPP2].MapX && Mp_Y == heroes[OPP2].MapY)
	{
		heroes[CURR].attack(type,heroes[OPP2]); sendAttack(type, CURR, OPP2);
	}
	else if((Mp_X >= 2 && Mp_X <=5) && (Mp_Y >=5 && Mp_Y <=8) && (CURR == 2 || CURR == 3))
	{
		if((abs(heroes[CURR].MapX - Mp_X) <= 3 && abs(heroes[CURR].MapY - Mp_Y) <= 3 && type == 1) || (abs(heroes[CURR].MapX - Mp_X) <= 5 && abs(heroes[CURR].MapY - Mp_Y) <= 5 && type == 0) )
		{
			cout<<"Temple 0 attack\n";
			TEMPLE_HEALTH[0] -= heroes[CURR].power;
			sendTempleStatus(CURR, 0);
			templeAttacked[0] = 100;
			if(TEMPLE_HEALTH[0] <= 0)
			{
				TEMPLE_HEALTH[0] = 0;
				//TODO second team wins;
				isWin = 1;
			}
		}
	}
	else if((Mp_X >= 16 && Mp_X <=19) && (Mp_Y >=19 && Mp_Y <=22) && (CURR == 0 || CURR == 1))
	{
		if((abs(heroes[CURR].MapX - Mp_X) <= 1 && abs(heroes[CURR].MapY - Mp_Y) <= 1 && type == 1) || (abs(heroes[CURR].MapX - Mp_X) <= 3 && abs(heroes[CURR].MapY - Mp_Y) <= 3 && type == 0) )
		{
			cout<<"Temple 1 attack\n";
			TEMPLE_HEALTH[1] -= heroes[CURR].power;
			sendTempleStatus(CURR, 1);
			templeAttacked[1] = 100;
			if(TEMPLE_HEALTH[1] <= 0)
			{
				TEMPLE_HEALTH[1] = 0;
				//TODO first team wins;
				isWin = 1;
			}
		}
	}
	//handle network output	
	
}

void* handleAttackS(void* x)
{
	//cout<<N_HeroId<<" "<<CURR<<" "<<N_OppId<<" "<<OPP2<<" "<<OPP2<<endl;
	//assert(N_HeroId == CURR && (N_OppId == OPP1 || N_OppId == OPP2));
	int *a = (int*)x;
	heroes[a[1]].attack(a[0],heroes[a[2]]);

/*if(NW_x == heroes[OPP1].MapX && NW_y == heroes[OPP1].MapY)
		heroes[N_HeroId].attack(1,heroes[OPP1]);
	else if(NW_x == heroes[OPP2].MapX && NW_y == heroes[OPP2].MapY)
		heroes[N_HeroId].attack(1,heroes[OPP2]);
*/	
	int status;
	pthread_exit(&status);
}
