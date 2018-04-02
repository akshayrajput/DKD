#ifndef OPENGL_H
#define OPENGL_H

#include<GL/freeglut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<cstdio>
#include<png.h>
#include<string>
#include<vector>
#include<utility>
#include<iostream>
#include<climits>
#include<cmath>
#include<vector>
#include<algorithm>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<cstdlib>
#include<cassert>
enum GAME_STATE {RUN = 0, PAUSE,QUIT};
enum GAME_MENU {GAME=0, MAIN,INST};

#define vit vector<int>::iterator

#define TRV(c,it) \
	for(vector<int>::iterator it = (c).begin(); it != (c).end(); it++)

using namespace std;
#endif
