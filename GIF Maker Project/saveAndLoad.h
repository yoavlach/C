#ifndef SAVEANDLOADH
#define SAVEANDLOADH
#include <stdio.h>
#include <stdbool.h>
#include "filesInProjectManagment.h"
#include "Linkedlist.h"
void saveProject(FrameNode* head);
bool load(FrameNode* head);
void writeToFile(FrameNode* node, FILE* file);
void convertIntToStr(int num, char str[]);
void writeStrToFile(FILE* file, char str[], int mode);
void addCharToNumAndChangeBool(char ch, int* num, bool* success);
bool checkIfNameExists(char* name, int amountOfNodes);
FrameNode* loadFile();
#endif