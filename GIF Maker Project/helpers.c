#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Linkedlist.h"
#include "helpers.h"
#define BUFFER_LEN 200
#define FIRST_INDEX 0
/*Frees a frame node
input: a pointer to the frame to free
output: none*/
void freeFrameNode(FrameNode* node)
{
    free(node->frame->path);
    free(node->frame->name);
    free(node->frame);
    free(node);
}
/*Frees all the frames in the list
input: a pointer to the head
output: none*/
void freeAllFrameNodes(FrameNode* head)
{
    FrameNode* curr = head;
    FrameNode* tmp = head;
    while(curr)
    {
        tmp = curr;
        curr = curr->next;
        freeFrameNode(tmp);
    }
}
/*Gets a node by its name
input: a pointer to the head of the list and the name
of the wanted node
output: a pointer to the node with the same name*/
FrameNode* getNodeByName(FrameNode* head, char name[])
{
    FrameNode* curr = head;
    FrameNode* frameToReturn = NULL;
    for(; curr && !frameToReturn; curr = curr->next)
    {
        if(!strcmp(curr->frame->name, name))
        {
            frameToReturn = curr;
        }
    }
    return frameToReturn;
}
/*Gets a node by its index
inpuy: a pointer to the head of the list and the index
of the wanted node
outptu: a pointer to the node with the same index*/
FrameNode* getNodeByIndex(FrameNode* head, int index)
{
    int i = 0;
    FrameNode* curr = head;
    for(i = 0; i < index && curr->next; i++)
    {
        curr = curr->next;
    }
    return curr;
}
/*Gets an input and makes sure it's int
input: none
output: the input as int*/
int getIntInput()
{
    char buffer[BUFFER_LEN] = {0};
    char currCh = ' ';
    int i = 0;
    int frameDuration = 0;
    int bufferLen = 0;
    bool validInput = true;
    do
    {
        frameDuration = 0;
        validInput = true;
        getStrInput(buffer, BUFFER_LEN);
        bufferLen = strlen(buffer);
        if(buffer[FIRST_INDEX] == '\0')
        {
            validInput = false;
            printf("Input can't be empty\n");
        }
        else
        {
            for(i = 0; i < bufferLen && validInput; i++)
            {
                currCh = buffer[i];
                if(currCh >= '0' && currCh <= '9')
                {
                    frameDuration = frameDuration * 10 + (currCh - '0');
                }
                else
                {
                    validInput = false;
                }
            }
            if(!validInput)
            {
                printf("Please enter a number\n");
            }
        }
    }
    while (!validInput);
    return frameDuration;
}
/*Gets a string input and cleans the string from the '\n'
input: a pointer to the array to scan into and how many
chars to scan into the array
output: none*/
void getStrInput(char buffer[], int size)
{
    do
    {
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if(!buffer[FIRST_INDEX])
        {
            printf("Input can't be empty! Please try again\n");
        }
    }
    while (!buffer[FIRST_INDEX]);
}
/*Finds the last frame node in the list
input: a pointer to the head of the list
output: a pointer to the last node in the list*/
FrameNode* findLastFrameNode(FrameNode* head)
{
    while(head && head->next)
    {
        head = head->next;
    }
    return head;
}
/*Gets a pointer to a node that his next node has a certain name
input: a pointer to the head of the list and the required name
output: none
this function may look weird but if i were to get the node by its name it would be O(n)
and then getting the previous node is another O(n - 1) but like that it's way mowe efficient*/
FrameNode* getPreviousNodeByNextNodeName(FrameNode* head, char name[])
{
    FrameNode* curr = head;
    FrameNode* previous = NULL;
    for(; curr && !previous; curr = curr->next)
    {
        if(curr->next != NULL && !strcmp(curr->next->frame->name, name))
        {
            previous = curr;
        }
    }
    return previous;
}
/*Checks if a frame exists in the list
input: a pointer to the head of the list and the name of the
frame this function checks
output: whether the frame exists or not*/
bool checkIfFrameExists(FrameNode* head, char name[])
{
    bool frameExists = getNodeByName(head, name) != NULL;
    if(!frameExists)
    {
        printf("This frame doesn't exist\n");
    }
    return frameExists;
}
/*Frees a frame
input: a pointer to the frame to free
output: none*/
void freeFrame(Frame* frame)
{
    free(frame->name);
    free(frame->path);
    free(frame);
}
/*Gets duration input and makes sure it's positive
input: none
output: the duration*/
int getDurationInput()
{
    int duration = 0;
    do
    {
        duration = getIntInput();
        if(duration <= 0)
        {
            printf("Duration most be greater than 0\nPlease try again: ");
        }
    }
    while (duration <= 0);
    return duration;
}