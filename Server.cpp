#include"allstdlib.h"
#include"Server.h"
#include"Map.h"
#include"Util.h"
#include"static_map.cpp"
//Global Declaration

node mySelf;
node table1[3];
bool alive1[3];
extern Hero heroes[4];
extern MapNode MAP[MAPW][MAPH];
extern int NW_x;
extern int NW_y;
extern int N_HeroId;
extern int N_OppId;
extern int n_id;
extern int TEMPLE_HEALTH[2];
extern int isWin;
extern int TID;
extern int templeAttacked[2];
int sex = 1;
bool isSend = false;


/**************Wrapper function's Definitions*******************/

void dead(int heroid)
{
	char buffer[256];
	sprintf(buffer,"iamdead %d",heroid);
	for(int i = 0; i < 3; i++)
	{
		client(buffer, table1[i].ip, table1[i].port);
	}	
}

void alive (int heroid)
{
	char buffer[256];
	sprintf(buffer,"iamalive %d",heroid);
	for(int i = 0; i < 3; i++)
	{
		client(buffer, table1[i].ip, table1[i].port);
	}	
}

void sendHeroStatus(int id, int dx, int dy, int health, int speed, int power, int mana)
{
	char buffer[256];
	sprintf(buffer, "hero %d %d %d %d %d %d %d", id, dx, dy, health, speed, power, mana);
	//cout<<"[log]:[sendHeroStatus] "<<buffer<<"\n";
	for(int i = 0; i < 3;i++)
	{
		if(table1[i].taken)
			client(buffer, table1[i].ip, table1[i].port);
	}
}

void sendTempleStatus(int myid, int templeid)
{
	char buffer[256];
	sprintf(buffer, "temple %d %d",myid,templeid);
	for(int i = 0; i < 3; i++)
	{
		if(table1[i].taken)
			client(buffer, table1[i].ip, table1[i].port);
	}
}

void sendAttack(int attacktype, int attackerid, int victimid)
{
	char buffer[256];
	sprintf(buffer, "attack %d %d %d", attacktype, attackerid, victimid);
	for(int i = 0; i < 3;i++)
	{
		if(table1[i].taken)
			client(buffer, table1[i].ip, table1[i].port);
	}
}


/**********************END****************************/


/**************Function Definitions******************/

char* getIPAddr()
{
	char *a, *b;
	a = (char*)malloc(sizeof(char)*16);
	b = (char*)malloc(sizeof(char)*16);
	for(int i = 0; i < 16; ++i)
	{
		a[i] = b[i] = '\0';
	}
	system("ifconfig | grep 'inet addr' | cut -d':' -f2 | cut -d' ' -f1 > ipvalue");
	FILE *f1 = fopen("ipvalue","r");
	fscanf(f1, "%s%s", a, b);
	fclose (f1);
	if(strcmp(a,"127.0.0.1"))
		return a;
	else
		return b;
}

void join(char ip[16], int port)
{
	char buffer[256];
	//cout<<"In join- ID- "<<mySelf.id<<"\n";
	sprintf(buffer,"join %s %d %d",mySelf.ip, mySelf.port, mySelf.id);
	pthread_t stid, ptid;
	int retServ = pthread_create(&stid, NULL, server,NULL);
	if(retServ)
	{
		//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retServ);
		exit(-1);
	}
	int retPing = pthread_create(&ptid, NULL, ping_all, NULL);
	if(retPing)
	{
		//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retPing);
		exit(-1);
	}
	//fprintf(stdout,"[log]:JOIN: Sending join request %s %d %s\n",ip,port,buffer);
	client(buffer, ip, port);
	//fprintf(stdout,"[log]:JOIN: Send join request %s %d %s\n",mySelf.ip,mySelf.port,buffer);
}

void* server(void* kal)
{
	int sockfd, newsockfd, n;
	char buffer[256];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clilen;
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		//fprintf(stdout, "[log]:SERVER: ERROR, opening socket\n");
	}

	bzero((struct sockaddt_in*) &serverAddr, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(mySelf.port);
	if(bind(sockfd, (const struct sockaddr *) &serverAddr, (socklen_t)sizeof(serverAddr)) < 0)
	{ 
		//fprintf(stdout,"[log]: SERVER: Socket already taken.");exit(-1);
	}
	
	while(1)
	{
		
		if(n_id == 0)//Only server creator will do this
		{
			int count = 0;
			if(!isSend)
			{
				for(int i = 0; i < 3; i++)
				{
					if(table1[i].taken) count++; 
				}
				if(count == 3)
				{
					isSend = true;
					for(int i = 0; i < 3; ++i)
					{
						for(int j = 0; j < 3; ++j)
						{
							if(i != j)
							{
								char newbuffer[256];
								sprintf(newbuffer,"join %s %d %d",table1[i].ip,table1[i].port,table1[i].id);
								client(newbuffer, table1[j].ip, table1[j].port);
							}
						}
			
					}
					char newbuffer[256];
					sprintf(newbuffer,"join %s %d %d",mySelf.ip,mySelf.port,mySelf.id);
					for(int i = 0; i < 3; i++)
					{
						client(newbuffer, table1[i].ip, table1[i].port);
					}
				}
			}
		}
		listen(sockfd,1000);
		
		clilen = sizeof(clientAddr);

		newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
		
		if(newsockfd < 0)
		{
			fprintf(stderr,"SERVER: ERROR, Not accepting new socket\n");
			exit(1);
		}
		
		//bzero(buffer,256);
		n = read(newsockfd,buffer,256);
		//printf("SERVER:CLient send ");puts(buffer);printf("\n");fflush(stdout);
		if (n < 0)
		{
			fprintf(stdout,"SERVER: ERROR, reading from socket\n");
			exit(1);
		}
		//cout<<"[log]:SERVER: buffer is "<<buffer<<"\n";
		if(strncmp("join", buffer, 4) == 0)
		{
			if(n_id != 0)
			{
				cout<<"GOT buffer join "<<buffer<<endl;
			}
			fprintf(stdout,"[log]:SERVER: GOT join request %s\n",buffer);
			int i = 5, j=0;
			char buf[20], temp[6],temp2[2];
			temp2[1] = '\0';
			int port;
			while(buffer[i] != ' ')
			{
				buf[j] = buffer[i];
				i++;j++;
			}
			buf[j] = '\0';
			//fprintf(stdout,"[log]:SERVER: join ip is k%sk\n",buf);
			i++;j = 0;
			while(buffer[i] != ' ')
			{
				//fprintf(stdout,"[log]:SERVER: join ip is k%sk %d\n",buf,i);
				temp[j] = buffer[i];
				j++;i++;
			}
			i++;
			temp[j] = '\0';
			port = atoi(temp);
			//fprintf(stdout,"[log]:SERVER: join ip port is %s %d\n",buf,port);
			int id;
			j = 0;
			
			temp2[0] = buffer[i];
			
			id = atoi(temp2);
			//fprintf(stdout,"[log]:SERVER: join ip port id is %s %d %d\n",buf,port,id);
			//bool isPresent = false;
			
			for(int k = 0; k < 3; k++)
			{
				if(!table1[k].taken)
				{
					table1[k].taken = true;
					strcpy(table1[k].ip, buf);
					table1[k].port = port;
					table1[k].id = id;
					break;
				}
			}
			
			cout<<"SERVER: MY TABLE is : \n";
			for(int k = 0; k < 3; k++)
			{
				cout<<table1[k].ip<<":"<<table1[k].port<<endl;
			}
			
		}
		
		else if(strncmp("ping", buffer, 4) == 0)
		{
			//cout<<"got ping request\n";
			char newip[16], cport[6],cid[2];
			cid[1] = '\0';
			int newport;
			int i = 5, j = 0;
			char newbuffer[256];
			while(buffer[i] != ' ')
			{
				newip[j]=buffer[i];
				j++;i++;
			}
			newip[j] = '\0';
			i++;j=0;
			
			while(buffer[i] != ' ')
			{
				cport[j] = buffer[i];
				j++;i++;
			}
			newport = atoi(cport);
			i++;j=0;
			cid[0] = buffer[i];
			int id = atoi(cid);
			
			////fprintf(stdout,"GOT ping request from %s %d %d",mySelf.ip, mySelf.port,id);
			sprintf(newbuffer, "alive %s %d %d", mySelf.ip, mySelf.port, mySelf.id);
			client(newbuffer, newip, newport);
		}
		
		else if(strncmp("alive", buffer, 5) == 0)
		{
			char newip[16], temp[6], temp2[2];
			temp2[1] = '\0';
			int newport;
			int i = 6, j = 0;
			while(buffer[i] != ' ')
			{
				newip[i-6] = buffer[i];
				i++;
			}
			newip[i-6] = '\0';
			i++;
			
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				j++;i++;
			}
			i++;
			newport = atoi(temp);
			temp2[0] = buffer[i];
			int id = atoi(temp2);
			for(int i = 0; i < 3; i++)
			{
				if(!strcmp(table1[i].ip, newip) && (table1[i].port == newport) && (table1[i].id == id))
					alive1[i] = true;
			}
		}
		
		else if(strncmp("hero", buffer, 4) == 0)
		{
			int i = 5, j = 0;
			char temp[100];
			int id, dx, dy, health, speed, power, magic;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			id = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			dx = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			dy = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			health = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			speed = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			power = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			magic = atoi(temp);
			//TODO : change hero status according to id
			NW_x = dx;
			NW_y = dy;
			N_HeroId = id;
			if(id == 0)
			{
				heroes[0].health = health;
				heroes[0].power = power;
				heroes[0].mana = magic;
				heroes[0].speed = speed;
			}
			else if(id == 1)
			{
				heroes[1].health = health;
				heroes[1].power = power;
				heroes[1].mana = magic;
				heroes[1].speed = speed;
			}
			else if(id == 2)
			{
				heroes[2].health = health;
				heroes[2].power = power;
				heroes[2].mana = magic;
				heroes[2].speed = speed;
			}
			else if(id == 3)
			{
				heroes[3].health = health;
				heroes[3].power = power;
				heroes[3].mana = magic;
				heroes[3].speed = speed;
			}
			else
			{
				cout<<"hero status wrong hero id"<<id<<"\n";
			}
			pthread_t itid;
			int hid = id;
			int inp = pthread_create(&itid, NULL, inputS,(void*)&id);
			if(inp)
			{
				//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retServ);
				exit(-1);
			}
				

			
		}
		
		
		else if(strncmp("temple",buffer,6) == 0)
		{
			int i = 7, j = 0;
			char temp[100];
			int id, templeId;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			temp[j] = '\0';
			id = atoi(temp);
			j = 0;i++;
			
			while(buffer[i] != ' ')
			{
				temp[j] = buffer[i];
				i++;j++;
			}
			i++;
			temp[j] = '\0';
			templeId = atoi(temp);
			TEMPLE_HEALTH[templeId] -= heroes[id].power;
			templeAttacked[templeId] = 100;
			if(TEMPLE_HEALTH[templeId] <= 0)
			{
				if(templeId != TID)
				{
					//TODO team wins
					isWin = 1;
				}
				else
				{
					//TODO team lost
					isWin = 2;
				}
			}
			
			//TODO: change temple status, using the power of hero(id).
		}
		
		else if(strncmp("attack",buffer,6) == 0)
		{
			cout<<"Attack event occured\n";
			int i = 7, j = 0, a[3];
			char atype[2], aid[2], vid[2];
			int attacktype, attackerid, victimid;
			atype[0] = buffer[7];
			atype[1] = '\0';
			aid[0] = buffer[9];
			aid[1] = '\0';
			vid[0] = buffer[11];
			vid[1] = '\0';
			
			attacktype = atoi(atype);
			attackerid = atoi(aid);
			victimid = atoi(vid);
			//TODO: reduce victim health using attacktype hero id and power..
			a[0] = attacktype;
			a[1] = attackerid;
			a[2] = victimid;
			cout<<a[1]<<" attacks "<<a[2]<<"\n";
			pthread_t itid;
			int inp = pthread_create(&itid, NULL, handleAttackS,(void *)a);
			if(inp)
			{
				//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retServ);
				exit(-1);
			}
		}
		
		else if(strncmp("iamdead",buffer,7) == 0)
		{
			char cheroid[2];
			int heroid;
			cheroid[1] = '\0';
			cheroid[0] = buffer[8];
			heroid = atoi(cheroid);
			heroes[heroid].isAlive = false;
			int X = heroes[heroid].MapX;
			int Y = heroes[heroid].MapY;
			MAP[X][Y].setType(FREE);
			
			heroes[heroid].health = STATIC_HERO_PROP[heroid][4];
			
			heroes[heroid].speed = STATIC_HERO_PROP[heroid][5];
			
			heroes[heroid].healer = false;
			
			heroes[heroid].power = STATIC_HERO_PROP[heroid][6];
			heroes[heroid].mana = STATIC_HERO_PROP[heroid][7];
			
			//TODO Make the hero of heroid dead
		}
		
		else if(strncmp("iamalive", buffer, 8) == 0)
		{
			char cheroid[2];
			int heroid;
			cheroid[1] = '\0';
			cheroid[0] = buffer[9];
			heroid = atoi(cheroid);
			heroes[heroid].isAlive = true;
			cout<<"Hero - "<<heroid<<" is alive\n";
		}
		
    	else
    	{
    		//fprintf(stdout,"Iam not suppose to enter here please check parsing.");exit(-1);
    	}
    
	}
}

void client(char buffer[256], char ip[16], int port)
{
	int sockfd, n;
	struct sockaddr_in serverAddr;
	
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0)
	{
		fprintf(stderr,"Error opening socket\n");
		exit(1);
	}
	
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	if(connect(sockfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0)
	{
		fprintf(stderr,"CLIENT: ERROR, Host with ip => [%s] and port => [%d] Network Error.\n",ip, port);
		exit(1);
    }
    
     n = write(sockfd,buffer, 256);
    
	if (n < 0)
	{
		fprintf(stderr,"CLIENT: ERROR writing to socket\n");
	}
	close(sockfd);
}

void* ping_all(void*)
{
	char buffer[256];
	while(1)
	{
		for(int i = 0; i < 3; i++)
		{
			if(table1[i].taken)
			{
				sprintf(buffer,"ping %s %d %d",mySelf.ip, mySelf.port,mySelf.id);
				client(buffer, table1[i].ip, table1[i].port);
				alive1[i] = false;		
			}
		}

		usleep(5000000);
		for(int i = 0; i < 3; i++)
		{
			if(!alive1[i] && table1[i].taken)
			{
				//TODO wait for 60 secs and then resume game.
			}
		}
	}
}

void createServer(int port)
{
	char  *ip = getIPAddr();
	sprintf(mySelf.ip,"%s", ip);
	mySelf.port = port;
	mySelf.taken = true;
	//create thread of server and ping function
	pthread_t stid, ptid;
	int retServ = pthread_create(&stid, NULL, server,NULL);
	if(retServ)
	{
		//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retServ);
		exit(-1);
	}
	int retPing = pthread_create(&ptid, NULL, ping_all, NULL);
	if(retPing)
	{
		//fprintf(stdout,"[log]:CREATESERVER: Error creating server thread error code %d",retPing);
		exit(-1);
	}

	//fprintf(stdout,"[log]:CREATESERVER: A new Server is created with ip:port = %s:%d\n",mySelf.ip, mySelf.port);
}
