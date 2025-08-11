#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Linkedlist.h"
#include "editFiles.h"
#include "helpers.h" 
#define BUFFER_LEN 200
#define MIN_INDEX 1
#define HEAD_INDEX -1
#define ALIGNMENT_DECREASE_VALUE 2
/*Changes frame's duration
input: a pointer to the head of the list
output: none*/
void changeFrameDuration(FrameNode* head)
{
    char nameOfFrameToChange[BUFFER_LEN] = {0};
    FrameNode* curr = head;
    FrameNode* nodeToChange = NULL;
    printf("enter the name of the frame\n");
    getStrInput(nameOfFrameToChange, BUFFER_LEN);
    if(checkIfFrameExists(head, nameOfFrameToChange))
    {
        for(curr = head; curr && !nodeToChange; curr = curr->next)
        {
            if(!strcmp(curr->frame->name, nameOfFrameToChange))
            {
                nodeToChange = curr;
            }
        }
        printf("Enter the new duration\n");
        nodeToChange->frame->duration = getDurationInput();
    }
}
/*Changes the duration of all the frames in the list
input: a pointer to the head of the head of the list
output: none*/
void changeDurationOfAllFrames(FrameNode* head)
{
    int duration = 0;
    printf("Enter the duration for all frames:\n");
    duration = getDurationInput();
    for(; head; head = head->next)
    {
        head->frame->duration = duration;
    }
}
/*Changes frame's index in the list
input: a pointer to the head of the list
output: a pointer to the head of the updated list*/
FrameNode* changeFrameIndex(FrameNode* head)
{
    char frameName[BUFFER_LEN] = {0};
    FrameNode* frameToChange = NULL;
    FrameNode* previous = NULL;
    FrameNode* nodeAtChangeIndex = NULL;
    FrameNode* nodeAfterNodeAtChangeIndex = NULL;
    int newIndex = 0;
    printf("Enter the name of the frame\n");
    getStrInput(frameName, BUFFER_LEN);
    if(checkIfFrameExists(head, frameName))
    {
        /*if the frame we need to change is the head there is no point in getting the previous node
        since it doesn't exist*/
        if(!strcmp(head->frame->name, frameName))
        {
            previous = NULL;
            frameToChange = head;
        }
        else
        {
            previous = getPreviousNodeByNextNodeName(head, frameName);
            frameToChange = previous->next;
        }
        do
        {
            printf("Enter the new index in the movie you wish to place the frame\n");
            newIndex = getIntInput();
            if(newIndex < MIN_INDEX)
            {
                printf("The index must be equal to 1 or larger\n");
            }
        }
        while (newIndex < MIN_INDEX);
        if(head->next != NULL) // if the length of the list is 1 there is no point in changing the indexes
        {
            // getting the frame we need to change out of the list
            if(previous == NULL)
            {
                head = frameToChange->next;
            }
            else if(frameToChange->next == NULL)
            {
                previous->next = NULL;
            }
            else
            {
                previous->next = frameToChange->next;
            }
            frameToChange->next = NULL;
            newIndex -= ALIGNMENT_DECREASE_VALUE; // decreasing 1 in order to align with the list's counting and another 1 because we removed a node
            nodeAtChangeIndex = getNodeByIndex(head, newIndex);
            // putting the frame in the correct index
            if(newIndex == HEAD_INDEX) // if the user wants to put the node at the start of the list he would choose one and then we decrease 2 which is -1
            {
                frameToChange->next = head;
                head = frameToChange;
            }
            else if(nodeAtChangeIndex->next == NULL)
            {
                nodeAtChangeIndex->next = frameToChange;
            }
            else
            {
                frameToChange->next = nodeAtChangeIndex->next;
                nodeAtChangeIndex->next = frameToChange;
            }
        }
    }
    return head;
}