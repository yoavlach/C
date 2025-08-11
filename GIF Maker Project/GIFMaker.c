#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "filesInProjectManagment.h"
#include "editFiles.h"
#include "view.h"
#include "Linkedlist.h"
#include "helpers.h"
#include "saveAndLoad.h"
#define MAX_COLOR 5
#define MIN_COLOR 1
#define MIN_BLUR 1
#define BUFFER_LEN 50
int showProjectMenu();
bool getFrameApplicationInput(char buffer[], int size, FrameNode* head);
int getActionInput();
int getColorInput();
int getBlurInput();
void listFrames(FrameNode* head);
enum PROJECT_OPTIONS { CREATE_A_NEW_PROJECT, LOAD_EXISITING_PROJECT };
enum ACTIONS { EXIT, ADD_NEW_FRAME, REMOVE_A_FRAME, CHANGE_FRAME_INDEX, CHANGE_FRAME_DURATION, CHANGE_DURATION_OF_ALL_FRAMES, LIST_FRAMES, PLAY_MOVIE, SAVE_PROJECT, APPLY_B_AND_W_EFFECT, BLUR_IMAGE, APPLY_THEME };
enum FILTER_MODES { BLACK_AND_WHITE, BLUR, APPLY_TINT, NONE };
int main()
{
    int action = 0, createOrLoad = -1, color = 0, strength = 0;
    char effectFrameName[BUFFER_LEN] = {0};
    FrameNode* head = NULL;
    Frame* currFrame = NULL;
    bool applyForAll = true;
    createOrLoad = showProjectMenu();
    if(createOrLoad == LOAD_EXISITING_PROJECT)
    {
        head = loadFile();
    }
    do
    {
        action = getActionInput();
        // I only had time for the effects bonus :(
        if(action >= APPLY_B_AND_W_EFFECT && action <= APPLY_THEME)
        {
            applyForAll = getFrameApplicationInput(effectFrameName, BUFFER_LEN, head);
        }
        switch(action)
        {
            case ADD_NEW_FRAME:
                currFrame = initFrame(head);
                if(currFrame != NULL)
                {
                    head = linkFrameToList(currFrame, head, NULL);
                }
                break;
            case REMOVE_A_FRAME:
                head = removeNode(head);
                break;
            case CHANGE_FRAME_INDEX:
                head = changeFrameIndex(head);
                break;
            case CHANGE_FRAME_DURATION:
                changeFrameDuration(head);
                break;
            case CHANGE_DURATION_OF_ALL_FRAMES:
                changeDurationOfAllFrames(head);
                break;
            case LIST_FRAMES:
                listFrames(head);
                break;
            case PLAY_MOVIE:
                play(head, NONE, 0, true, effectFrameName);
                break;
            case SAVE_PROJECT:
                saveProject(head);
                break;
            case APPLY_B_AND_W_EFFECT:
                play(head, BLACK_AND_WHITE, 0, applyForAll, effectFrameName);
                break;
            case BLUR_IMAGE:
                strength = getBlurInput();
                play(head, BLUR, strength, applyForAll, effectFrameName);
                break;
            case APPLY_THEME:
                color = getColorInput();
                play(head, APPLY_TINT, color, applyForAll, effectFrameName);
        }
    }
    while (action != EXIT);
    printf("Bye!\n");
    freeAllFrameNodes(head);
    getchar();
    return 0;
}
/*Shows menu on whether to create or load a project
input: none
output: an int representing the user's choice*/
int showProjectMenu()
{
    int action = 0;
    printf("Welcome to Magshimim Movie Maker! what would you like to do?\n");
    do
    {
        printf(" [0] Create a new project\n");
        printf(" [1] Load existing project\n"); 
        action = getIntInput();
        if(action != CREATE_A_NEW_PROJECT && action != LOAD_EXISITING_PROJECT)
        {
            printf("Invalid choice, try again:\n");
        }
    }
    while (action != CREATE_A_NEW_PROJECT && action != LOAD_EXISITING_PROJECT);
    return action;
}
/*Shows menu and gets action input
input: none
output: the chosen action*/
int getActionInput()
{
    bool validInput = true;
    int action = 0;
    do
    {
        printf("What would you like to do?\n");
        printf(" [0] Exit\n");
        printf(" [1] Add new Frame\n");
        printf(" [2] Remove a Frame\n");
        printf(" [3] Change frame index\n");
        printf(" [4] Change frame duration\n");
        printf(" [5] Change duration of all frames\n");
        printf(" [6] List frames\n");
        printf(" [7] Play movie!\n");
        printf(" [8] Save project\n");
        printf(" [9] Apply black and white effect\n");
        printf(" [10] Blur image\n");
        printf(" [11] Apply theme to image\n");
        action = getIntInput();
        validInput = action >= EXIT && action <= APPLY_THEME;
        if(!validInput)
        {
            printf("You should type one of the options - 0-8!\n");
        }
    }
    while (!validInput);
    return action;
}
/*Prints a list of the frames in the project
input: a pointer to the head of the frames list
output: none*/
void listFrames(FrameNode* head)
{
    int i = 0;
    for(i = 0; head; i++)
    {
        printf("%d) Name: %s\nDuration- %d ms\nPath- %s\n\n", i + 1,  head->frame->name, head->frame->duration, head->frame->path);
        head = head->next;
    }
}
/*Gets input for which frame to apply an effect to
input: a string to store the frame name, the size of the array,
and a pointer to the head of the list
output: whether to apply the effect to all frames*/
bool getFrameApplicationInput(char buffer[], int size, FrameNode* head)
{
    bool applyForAll = true;
    printf("Enter the name of the frame you would like to apply the effect on to\n");
    printf("Enter a frame with a name that doesn't exist in order to apply the effect on to every picture\n");
    getStrInput(buffer, size);
    applyForAll = !getNodeByName(head, buffer);
    return applyForAll;
}
/*Gets user input for a color
input: none
output: the selected color as an int*/
int getColorInput()
{
    int color = 0;
    printf("Choose a color:\n");
    do
    {
        printf(" [1] Red\n [2] Green\n [3] Blue\n [4] Yellow\n [5] Brown\n");
        color = getIntInput();
        if(color > MAX_COLOR || color < MIN_COLOR)
        {
            printf("Color not in range! Please try again\n");
        }
    }
    while (color > MAX_COLOR || color < MIN_COLOR);
    return color - 1;   
}
/*Gets input for blur strength
input: none
output: the chosen blur strength as an int*/
int getBlurInput()
{
    int strength = 0;
    do
    {
        printf("Choose blur strength (has to be larger than 1):\n");
        strength = getIntInput();
        if(strength < MIN_BLUR)
        {
            printf("Choice not in range! Please try again\n");
        }
    }
    while (strength < MIN_BLUR);
    return strength;
}