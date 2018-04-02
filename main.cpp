//main game program
#include"Server.h"
#include"Util.h"
extern node mySelf;
extern node table1[3];
extern bool alive1[3];
int n_id;

void runMainLoop( int val );
void init(int argc,char* args[])
{
	//initialize ip address
	strcpy(mySelf.ip, getIPAddr());
	//initialize hero id and network mode
    int mode = args[1][0] - '0';
    int h_id = args[2][0] - '0';
    //cout<<h_id<<"\n";
    initID(h_id);
    // if single player game is played
    if(mode == 0)
    {
    	
    }
    else
    {
		n_id = args[3][0] - '0';
		//if creating server
		if(n_id == 0)
		{
			int port = atoi(args[4]);
			createServer(port);
			mySelf.port = port;
		}
		
		//if joining server
		else
		{
			char ip[16], buffer[256];
			int port;
			mySelf.port = atoi(args[4]);
			strcpy(ip, args[5]);
			port = atoi(args[6]);
			join(ip, port);
		}
		
    }
	for(int i = 0; i < 3; i++)
	{
		while(!table1[i].taken);
	}
	/*char newbuffer[256];
	for(int i = 0; i < 3; i++)
	{
		
		if(i != 2)
		{
			sprintf(newbuffer,"join %s %d %d",table1[i].ip,table1[i].port,table1[i].id);
			client(newbuffer, table1[2].ip, table1[2].port);
			cout<<"SENDTO"<<table1[2].ip<<":"<< table1[2].port;
		}
		
	}
	
	sprintf(newbuffer,"join %s %d %d",mySelf.ip,mySelf.port,mySelf.id);
	client(newbuffer, table1[2].ip, table1[2].port);*/

     //Initialize FreeGLUT
    glutInit( &argc, args );

    //Create OpenGL 2.1 context
    glutInitContextVersion( 2, 1 );

    //Create Double Buffered Window
    glutInitDisplayMode( GLUT_DOUBLE );
    glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
    glutCreateWindow( "Delhi ka Dota!!!" );

    //Do post window/context creation initialization
    if( !initGL() )
    {
        printf( "Unable to initialize graphics library!\n" );
        exit(1);
    }

    //Load media
    if( !loadMedia() )
    {
        printf( "Unable to load media!\n" );
        exit(2);
    }

    //Set rendering function
    glutDisplayFunc( render );

    //Set mouse handler
    glutMouseFunc(mouseHandler);

    //Set main loop
    //glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );
    glutTimerFunc( 20, runMainLoop, 0 );
    //Start GLUT main loop
    glutMainLoop();

}

void runMainLoop( int val )
{
    //Frame logic
    update();
    render();

    glutTimerFunc( 20, runMainLoop, val );
}

void shutdown()
{
}

int main( int argc, char* args[] )
{
    if(argc != 5 && argc != 7)
    {
    	cout<<"Usage: for server ./DkD 1 <hid(0-4)> 0 <myport>\n";
    	cout<<"for client ./DkD 1 <hid(0-4)> 1 <myport> <server ip> <server port>\n";
    	cout<<"Note all four players have to choose different <hid>\n";
	    exit(0);
	}
    init(argc,args);
    //shutdown();

    return 0;
}


