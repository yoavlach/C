#ifndef HELPERSH
#define HELPERSH
#include "Linkedlist.h"
#include <stdbool.h>
void freeFrameNode(FrameNode* node);
void freeFrame(Frame* frame);
void freeAllFrameNodes(FrameNode* head);
FrameNode* getNodeByName(FrameNode* head, char name[]);
FrameNode* getNodeByIndex(FrameNode* head, int index);
FrameNode* findLastFrameNode(FrameNode* head);
FrameNode* getPreviousNodeByNextNodeName(FrameNode* head, char name[]);
bool checkIfFrameExists(FrameNode* head, char name[]);
int getIntInput();
void getStrInput(char buffer[], int size);
int getDurationInput();
#endif