#ifndef SERVER_H
#define SERVER_H

typedef struct g
{
	char ip[16];
	int port;
	int id;
	bool taken = false;
	bool recv;
}node;



/**************Function Declarations******************/
char* getIPAddr();
void* server(void*);
void createServer(int);
void client(char buffer[256], char ip[16], int port);
void* ping_all(void*);
void join(char ip[16], int port);

/*****************ENDS HERE************************/

/**************Wrapper function's Declarations*******************/
//USE These functions

void sendHeroStatus( int id, int dx, int dy, int health, int speed, int power, int mana);
void sendTempleStatus(int myid, int templeid);
void sendAttack(int type,int attackerid, int victimidi);
void dead(int heroid);
void alive (int heroid);
#endif
