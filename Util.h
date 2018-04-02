#ifndef UTIL_H
#define UTIL_H

#include "allstdlib.h"
#include"Hero.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int SCREEN_FPS = 60;

bool initGL();
bool loadMedia();
void update();
void render();
void input();
void* inputS(void*);
void mouseHandler(int,int,int,int);
void initID(int);
vector<MOVEMENT> direction(int,int,int,int);
void handleAttack(int);
void* handleAttackS(void*);

#endif
