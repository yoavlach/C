/*********************************
* Class: MAGSHIMIM Final Project *
* Play function declaration          *
**********************************/

#ifndef VIEWH
#define VIEWH

#include "raylib.h" 
#include "Linkedlist.h" 

#define GIF_REPEAT 5

void play(FrameNode* list, int mode, int value, bool applyForAll, char nodeApplicationName[]);
void applyEffect(Image* img, int mode, int value);

#endif