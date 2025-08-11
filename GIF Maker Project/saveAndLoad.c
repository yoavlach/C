#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "filesInProjectManagment.h"
#include "Linkedlist.h"
#include "helpers.h"
#include "saveAndLoad.h"
#define BUFFER_LEN 200
#define LARGEST_INT_LEN 10
#define NULL_OPERATOR_SPACE 1
#define BACKSLASH_N_OPERATOR_SIZE 1
#define AMOUNT_OF_SEPERATORS 4
#define SEPERATOR ','
#define LINE_SEPERATOR '\n'
enum SEPERATOR_MODES {NEW_LINE, COMMA};
enum LINE_PARTS {NAME_LEN, NAME, DURATION, PATH_LEN, PATH};
/*Saves the project to a file by getting path
input and using the following format:
name length, name, duration, path length, path
input: a pointer to the head of the list
output: none*/
void saveProject(FrameNode* head)
{
    char saveFilePath[BUFFER_LEN] = {0};
    FILE* file = NULL;
    bool loadedFile = false;
    FrameNode* curr = head;
    do
    {
        printf("Where to save the project? enter a full path and file name\n");
        getStrInput(saveFilePath, BUFFER_LEN);
        file = fopen(saveFilePath, "w");
        loadedFile = file != NULL;
        if(!loadedFile)
        {
            printf("Cannot open file, please try again\n");
        }
    }
    while(!loadedFile);
    while(curr)
    {
        writeToFile(curr, file);
        curr = curr->next;
    }
    fclose(file);
}
/*Writes a frame to the file in the previously mentioned format
input: a pointer to the frame node and a pointer to the file
output: none*/
void writeToFile(FrameNode* node, FILE* file)
{
    int nameLen = strlen(node->frame->name), pathLen = strlen(node->frame->path);
    char strNameLen[LARGEST_INT_LEN] = {0};
    char strPathLen[LARGEST_INT_LEN] = {0};
    char strDuration[LARGEST_INT_LEN] = {0};
    convertIntToStr(nameLen, strNameLen);
    convertIntToStr(pathLen, strPathLen);
    convertIntToStr(node->frame->duration, strDuration);
    writeStrToFile(file, strNameLen, COMMA);
    writeStrToFile(file, node->frame->name, COMMA);
    writeStrToFile(file, strDuration, COMMA);
    writeStrToFile(file, strPathLen, COMMA);
    writeStrToFile(file, node->frame->path, NEW_LINE);
}
/*Converts an int to a string
input: the int to convert and a string to store the result
output: none*/
void convertIntToStr(int num, char str[])
{
    char tmpStr[LARGEST_INT_LEN] = {0};
    int i = 0, arrSize = 0;
    if(num == 0)
    {
        str[0] = '0';
    }
    else
    {
        for(i = 0; num != 0; i++)
        {
            tmpStr[i] = (num % 10) + '0';
            num = num / 10;
        }
        arrSize = i - 1;
        i--;
        for(; i >= 0; i--)
        {
            str[arrSize - i] = tmpStr[i];
        }
    }
}
/*Writes a string to the file followed by a separator or newline
input: a pointer to the file, the string to write, and the mode (COMMA or NEW_LINE)
output: none*/
void writeStrToFile(FILE* file, char str[], int mode)
{
    int i = 0;
    for(i = 0; str[i]; i++)
    {
        fputc(str[i], file);
    }
    switch(mode)
    {
        case COMMA:
            fputc(SEPERATOR, file);
            break;
        case NEW_LINE:
            fputc(LINE_SEPERATOR, file);
            break;
    }
}
char** nodesNames = NULL;
/*Loads the project from a file into a list by getting path
input and reading frames in the previously mentioned format
input: none
output: a pointer to the head of the loaded list*/
FrameNode* loadFile()
{
    char filePath[BUFFER_LEN] = {0};
    FILE* file = NULL;
    FrameNode* head = NULL;
    FrameNode* currFrameNode = NULL;
    Frame* currFrame = NULL;
    char ch = ' ';
    int linePart = NAME_LEN, nameLen = 0, pathLen = 0, nameCharCounter = 0, pathCharCounter = 0, amountOfNodes = 0;
    bool success = true, startedName = false, startedPath = false, startedNewLine = true, startedReadingFile = false, isEmpty = true;
    printf("Enter the path of the project (including project name):\n");
    getStrInput(filePath, BUFFER_LEN);
    file = fopen(filePath, "r");
    if(file == NULL)
    {
        printf("Error!- cant open file, creating a new project\n");
        success = false;
    }
    while(success && ((ch = (char)fgetc(file)) != EOF))
    {
        isEmpty = false;
        // if there is a need to initialize a new node
        if(startedNewLine)
        {
            pathCharCounter = 0, nameLen = 0, nameCharCounter = 0, pathLen = 0;
            /* in the first loop the frame will be null so there is no need to
            link it to the list yet*/
            if(startedReadingFile)
            {
                head = linkFrameToList(currFrame, head, &currFrameNode);
            }
            if(success)
            {
                startedReadingFile = true;
                currFrame = (Frame*)malloc(sizeof(Frame));
                if(currFrame == NULL)
                {
                    printf("Unsuccessful malloc!");
                    success = false;
                }
                else
                {
                    currFrame->duration = 0;
                    startedPath = false;
                }
                startedNewLine = false;
            }
        }
        if(success)
        {
            if(ch == LINE_SEPERATOR)
            {
                startedNewLine = true;
                linePart = NAME_LEN;
                amountOfNodes++;
            }
            else
            {
                switch(linePart)
                {
                    case NAME_LEN:
                        if(ch == SEPERATOR)
                        {
                            linePart = NAME;
                        }
                        else
                        {
                            addCharToNumAndChangeBool(ch, &nameLen, &success);
                        }
                        break;
                    case NAME:
                        if(!startedName)
                        {
                            currFrame->name = (char*)calloc(nameLen + NULL_OPERATOR_SPACE, sizeof(char));
                            if(currFrame->name == NULL)
                            {
                                printf("Unsuccessful calloc!");
                                success = false;
                                free(currFrame);
                            }
                            startedName = true;
                        }
                        if(success)
                        {
                            if(ch == SEPERATOR && nameCharCounter >= nameLen)
                            {
                                linePart = DURATION;
                                startedName = false;
                                if(checkIfNameExists(currFrame->name, amountOfNodes))
                                {
                                    printf("There are 2 or more frames with the same name in this file. ");
                                    free(currFrame->name);
                                    free(currFrame);
                                    success = false;
                                }
                                else
                                {
                                    nodesNames = (char**)realloc(nodesNames, (amountOfNodes + 1) * sizeof(char*));
                                    if(nodesNames == NULL)
                                    {
                                        printf("Unsuccessful realloc!");
                                        freeFrame(currFrame);
                                        success = false;
                                    }
                                    else
                                    {
                                        nodesNames[amountOfNodes] = currFrame->name;
                                    }
                                }
                            }
                            else
                            {
                                currFrame->name[nameCharCounter] = ch;
                                nameCharCounter++;
                            }
                        }
                        break;
                    case DURATION:
                        if(ch == SEPERATOR)
                        {
                            linePart = PATH_LEN;
                        }
                        else
                        {
                            addCharToNumAndChangeBool(ch, &currFrame->duration, &success);
                        }
                        break;
                    case PATH_LEN:
                        if(ch == SEPERATOR)
                        {
                            linePart = PATH;
                        }
                        else
                        {
                            addCharToNumAndChangeBool(ch, &pathLen, &success);
                        }
                        break;
                    case PATH:
                        if(!startedPath)
                        {
                            currFrame->path = (char*)calloc(pathLen + NULL_OPERATOR_SPACE, sizeof(char));
                            if(currFrame->path == NULL)
                            {
                                printf("Unsuccessful calloc!");
                                success = false;
                                free(currFrame);
                            }
                            startedPath = true;
                        }
                        if(success && ch != LINE_SEPERATOR)
                        {
                            currFrame->path[pathCharCounter] = ch;
                            pathCharCounter++;
                        }
                        if(pathCharCounter > pathLen && ch != NEW_LINE)
                        {
                            success = false;
                        }
                        break;
                }
            }
        }
    }
    if(success)
    {
        if(!isEmpty)
        {
            // the loop exits before linking the last line to the end of the list so Im doing it here
            head = linkFrameToList(currFrame, head, &currFrameNode);
            printf("File loaded successfully\n");
        }
        else
        {
            printf("File is empty\n");
            freeAllFrameNodes(head);
            head = NULL;
        }
        free(nodesNames);
    }
    else
    {
        printf("Unable to load file\n");
    }
    return head;
}
/*Adds a digit character to an int and updates success flag
input: the character to add, a pointer to the int, and a pointer to the success flag
output: none*/
void addCharToNumAndChangeBool(char ch, int* num, bool* success)
{
    if(ch >= '0' && ch <= '9')
    {
        *num = *num * 10 + (ch - '0');
    }
    else
    {
        *success = false;
    }
}
/*Checks if there is more than one frame with the same name in the file
by comparing the names in the list that contains all of the names
input: a pointer to the name to check and the amount of nodes in the list
output: none*/
bool checkIfNameExists(char* name, int amountOfNodes)
{
    int i = 0;
    bool exists = false;
    for(i = 0; i < amountOfNodes && !exists; i++)
    {
        exists = !(strcmp(name, nodesNames[i]));
    }
    return exists;
}