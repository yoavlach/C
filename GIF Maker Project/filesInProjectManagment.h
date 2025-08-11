#ifndef FILESINPROJECTMANAGMENTH
#define FILESINPROJECTMANAGMENTH
#include "Linkedlist.h"
Frame* initFrame(FrameNode* head);
FrameNode* linkFrameToList(Frame* frameToLink, FrameNode* head, FrameNode** currFrameNode);
FrameNode* removeNode(FrameNode* head);
#endif