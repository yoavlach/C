#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "filesInProjectManagment.h"
#include "Linkedlist.h"
#include "helpers.h"
#define BUFFER_LEN 200
#define NULL_OPERATOR_SPACE 1
/*Gets input in order to initialize a frame
input: a pointer to the head of the list
output: a pointer to the new frame*/
Frame* initFrame(FrameNode* head)
{
    FILE* file = NULL;
    Frame* currFrame = NULL;
    FrameNode* checkFrame = NULL;
    char buffer[BUFFER_LEN] = {0};
    bool frameExists = false;
    currFrame = (Frame*)malloc(sizeof(Frame));
    if(currFrame == NULL)
    {
        printf("Unsuccessful malloc!");
    }
    else
    {
        printf("*** Creating new frame ***\n");
        printf("Please insert frame path:\n");
        getStrInput(buffer, BUFFER_LEN);
        currFrame->path = (char*)calloc(strlen(buffer) + NULL_OPERATOR_SPACE, sizeof(char));
        if(currFrame->path == NULL)
        {
            printf("Unsuccessful calloc!");
            free(currFrame);
        }
        else
        {
            strcpy(currFrame->path, buffer);
            do
            {
                printf("Please insert frame duration(in miliseconds):\n");
                currFrame->duration = getDurationInput();
                if(currFrame->duration <= 0)
                {
                    printf("Frame duration must be positive!\n");
                }
            }
            while (currFrame->duration <= 0);
            do
            {
                printf("Please choose a name for that frame:\n");
                getStrInput(buffer, BUFFER_LEN);
                checkFrame = getNodeByName(head, buffer);
                frameExists = checkFrame != NULL;
                if(frameExists)
                {
                    printf("A frame with this name already exists. ");
                }
            }
            while (frameExists);
            currFrame->name = (char*)calloc(strlen(buffer) + NULL_OPERATOR_SPACE, sizeof(char));
            if(currFrame->name == NULL)
            {
                printf("Unsuccessful calloc!");
                free(currFrame->path);
                free(currFrame);
            }
            else
            {
                strcpy(currFrame->name, buffer);
                file = fopen(currFrame->path, "rb");
                if(file == NULL)
                {
                    printf("Can't find file! Frame will not be added\n");
                    freeFrame(currFrame);
                    currFrame = NULL;
                }
                else
                {
                    fclose(file);
                }
            }
        }
    }
    return currFrame;
}
/*Links a frame to the list
input: a pointer to the frame to link a pointer
to the head of the list and a pointer to a pointer to the current
frame node we need to put the frame in (can be NULL if not needed)
output: a pointer to the head of the list*/
FrameNode* linkFrameToList(Frame* frameToLink, FrameNode* head, FrameNode** currFrameNode)
{
    FrameNode* lastFrameNode = NULL;
    FrameNode* headOfUpdatedList = head;
    FrameNode* newFrameNode = (FrameNode*)malloc(sizeof(FrameNode));
    if(newFrameNode == NULL)
    {
        printf("Unsuccessful malloc!");
        freeFrame(frameToLink);
    }
    else
    {
        if(currFrameNode)
        {
            (*currFrameNode) = newFrameNode;
        }
        newFrameNode->next = NULL;
        newFrameNode->frame = frameToLink;
        if(head == NULL)
        {
            headOfUpdatedList = newFrameNode;
        }
        else
        {
            lastFrameNode = findLastFrameNode(head);
            lastFrameNode->next = newFrameNode;
        }
    }
    return headOfUpdatedList;
}
/*Removes a node from the list
input: a pointer to the head of the list
output: a pointer to the head of the list*/
FrameNode* removeNode(FrameNode* head)
{
    char nameToRemove[BUFFER_LEN] = {0};
    FrameNode* frameToRemove = NULL;
    FrameNode* previous = NULL;
    FrameNode* headOfUpdatedList = head;
    bool foundFrame = true;
    printf("Enter the name of the frame you wish to erase\n");
    getStrInput(nameToRemove, BUFFER_LEN);
    if(head != NULL && !strcmp(head->frame->name, nameToRemove))
    {
        previous = NULL;
        frameToRemove = head;
    }
    else
    {
        previous = getPreviousNodeByNextNodeName(head, nameToRemove);
        if(previous == NULL)
        {
            printf("The frame was not found\n");
            foundFrame = false;
        }
        else
        {
            frameToRemove = previous->next;
        }
    }
    if(foundFrame)
    {
        if(frameToRemove == head)
        {
            headOfUpdatedList = frameToRemove->next;
        }
        else if(frameToRemove->next == NULL)
        {
            previous->next = NULL;
        }
        else
        {
            previous->next = frameToRemove->next;
        }
        freeFrameNode(frameToRemove);
    }
    return headOfUpdatedList;
}