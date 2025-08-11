#include <stdio.h>
#include <stdbool.h>
#include "dirent.h"
#include <string.h>
#include <stdlib.h>
#define MIN_PARAMETERS 3
#define VIRUS_SIGNATURE_INDEX 2
#define SCAN_FOLDER_INDEX 1
#define NORMAL_SCAN 0
#define NULL_OPERATOR_SPACE 1
#define SLASH_OPERATOR_SPACE 1
#define SLASH_OPERATOR "/"
#define QUICK_SCAN_FIRST_START_PERCENT 0
#define QUICK_SCAN_FIRST_FINISH_PERCENT 20
#define QUICK_SCAN_SECOND_START_PERCENT 80
#define QUICK_SCAN_SECOND_FINISH_PERCENT 100
#define NORMAL_SCAN_START_PERCENT 0
#define NORMAL_SCAN_FINISH_PERCENT 100
#define DIVISION_TO_GET_PRECENT 100
#define COUNT_FILES_ERROR -1
enum FILE_SCAN_RETURN_OPTIONS {INFECTED, CLEAN, FIRST_TWENTY_PERCENT, LAST_TWENTY_PERCENT, FILE_TOO_SMALL_TO_CONTAIN_SIGNATURE, INFECTED_BUT_NOT_IN_FIRST_AND_LAST_TWENTY_PERCENT};
bool checkIfThereAreEnoughParameters(int argc);
void printWelcomeMsg(char* folderPath, char* virusSignature);
int getActionInput();
char* makeFilePath(char* fileName, char* filePath, int folderPathSize);
FILE* getFile(char* filePath);
char* readDataFromBinaryFile(FILE* file);
int sendNormalScan(char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen);
int sendQuickScan(char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen);
long getFileLen(FILE* file);
char* manageFolderReading(char* folderPath, char* virusSignaturePath, int action, char* virusSignature, long virusSignatureLen);
int scanFile(int startpercent, int finishpercent, char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen);
char* insertFileDataToMsg(char* filePath, int infectionValue, char* msg, int msgLen);
int countFilesInFolder(char* folderPath);
void sortFilesAlphabetically(char** filesNames, int filesCount);
char* createMsgStart(int scanningMethodInt, char* folderPathPath, char* virusSignaturePath);
void freeArrOfPointers(char** arr, int size);
void insertMsgToFile(char* msg, char* folderPath);
int main(int argc, char** argv)
{
    FILE* virusSignatureFile = NULL;
    char* virusSignaturePath = NULL;
    char* virusSignature = NULL;
    long virusSignatureLen = 0;
    int action = 0;
    char* folderPath = NULL;
    char* ansTxt = NULL;
    if(checkIfThereAreEnoughParameters(argc))
    {
        virusSignaturePath = argv[VIRUS_SIGNATURE_INDEX];
        virusSignatureFile = fopen(virusSignaturePath, "rb");
        if(virusSignatureFile == NULL)
        {
            printf("File opening error\n");
            return 1;
        }
        folderPath = argv[SCAN_FOLDER_INDEX];
        printWelcomeMsg(folderPath, virusSignaturePath);
        action = getActionInput();
        printf("Scanning began...\nThis process may take several minutes...\n\n");
        virusSignatureLen = getFileLen(virusSignatureFile);
        virusSignature = readDataFromBinaryFile(virusSignatureFile);
        if(virusSignature == NULL)
        {
            return 1;
        }
        ansTxt = manageFolderReading(folderPath, virusSignaturePath, action, virusSignature, virusSignatureLen);
        insertMsgToFile(ansTxt, argv[SCAN_FOLDER_INDEX]);
        free(virusSignature);
        free(ansTxt);
        fclose(virusSignatureFile);
    }
    getchar();
    getchar();
    return 0;
}
/*Checks if there are enough arguments to run the code
input: the amount of arguments added to main
output: if there are enough arguments the output is true
otherwise it's false*/
bool checkIfThereAreEnoughParameters(int argc)
{
    bool enoughArugments = true;
    if(argc < MIN_PARAMETERS)
    {
        printf("Invalid execution.\nUsage: antiVirusProject.exe <scan folder path> <virus signature path>\n");
        enoughArugments = false;
    }
    return enoughArugments;
}
/*Prints a welcoming message and telling the user what folder we
scan and what is the file used for the virus signature
input: a pointer to the scan folder's name and a pointer to the virus
signature's name
output: none*/
void printWelcomeMsg(char* folderPath, char* virusSignature)
{
    printf("Welcome to my virus scan!\n\n");
    printf("Folder to scan: %s\n", folderPath);
    printf("Virus signature: %s\n\n", virusSignature);
}
/*Gets the user's input on wether he wants to use a normal or
a quick scan and telling him about each option
input: none
output: the user's choice*/
int getActionInput()
{
    int input = 0;
    printf("Scanning options:\n");
    printf("  1. Normal Scan- goes through each byte in the file and checks if the virus signature is there\n");
    printf("  2. Quick Scan- only goes thorugh the first & last 20 percent of the file and checks if the virus signature is there\n  if the signature wasn't found it will check the entire file\n");
    printf("Press 0 for a normal scan or any other key for a quick scan: ");
    scanf("%d", &input);
    return input;
}
/*Genreates a file path according to it's folder and name
input: a pointer to the file's name, a pointer to the folder's path, and the folder's path size
output: a pointer to a complete path*/
char* makeFilePath(char* fileName, char* filePath, int folderPathSize)
{
    int fileNameLen = strlen(fileName) + NULL_OPERATOR_SPACE + SLASH_OPERATOR_SPACE;
    char* newFilePath = (char*)calloc(folderPathSize + fileNameLen, sizeof(char));
    if(newFilePath == NULL) 
    {
        printf("Unsuccessful realloc!");
    }
    else
    {
        strncat(newFilePath, filePath, folderPathSize);
        strncat(newFilePath, SLASH_OPERATOR, SLASH_OPERATOR_SPACE + NULL_OPERATOR_SPACE);
        strncat(newFilePath, fileName, fileNameLen);
    }
    return newFilePath;
}
/*Opens a file
input: the file's path
output: a pointer to it*/
FILE* getFile(char* filePath)
{
    FILE* file = fopen(filePath, "rb");
    if(file == NULL)
    {
        printf("Unable to open file\n");
    }
    return file;
}
/*Reads data from a binary file into a buffer
input: a pointer to the file to read from
output: a pointer to a buffer containing the file's data*/
char* readDataFromBinaryFile(FILE* file)
{
    long lSize;
    char* buffer = NULL;
    size_t result;
    lSize = getFileLen(file);
    buffer = (char*)malloc(sizeof(char)*lSize);
    if (buffer == NULL)
    {
        printf("Unsuccessful malloc!");
    }
    else
    {
        result = fread (buffer,sizeof(char),lSize,file);
        if (result != lSize)
        {
            printf("Reading error\n");
        }
    }
    return buffer;
}
/*Gets the length of the data in a certain binary file
input: a pointer to the file
output: it's size as long*/
long getFileLen(FILE* file)
{
    long fileLen = 0;
    fseek (file , 0, SEEK_END);
    fileLen = ftell (file);
    rewind (file);
    return fileLen;
}
/*Sends a normal scan request to scanFile
input: the virus signature, its length, the buffer containing the data inside the file and its length
output: the value scanFile returned which represents the state of the file*/
int sendNormalScan(char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen)
{
    int infected = scanFile(NORMAL_SCAN_START_PERCENT, NORMAL_SCAN_FINISH_PERCENT, virusSignature, bufferLen, buffer, virusSignatureLen);
    return infected;
}
/*Sends a quick scan request to scanFile, if the file wasn't found it will use sendNormalScan to perform a normal scan
input: the virus signature, its length, the buffer containing the data inside the file and its length
output: the value scanFile infected which represents the state of the file*/
int sendQuickScan(char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen)
{
    int infected = scanFile(QUICK_SCAN_FIRST_START_PERCENT, QUICK_SCAN_FIRST_FINISH_PERCENT, virusSignature, bufferLen, buffer, virusSignatureLen);
    if(infected == INFECTED)
    {
        infected = FIRST_TWENTY_PERCENT;
    }
    else if(infected == CLEAN)
    {
        infected = scanFile(QUICK_SCAN_SECOND_START_PERCENT, QUICK_SCAN_SECOND_FINISH_PERCENT, virusSignature, bufferLen, buffer, virusSignatureLen);
        if(infected == INFECTED)
        {
            infected = LAST_TWENTY_PERCENT;
        }
        else
        {
            infected = sendNormalScan(virusSignature, bufferLen, buffer, virusSignatureLen);
            if(infected == INFECTED)
            {
                infected = INFECTED_BUT_NOT_IN_FIRST_AND_LAST_TWENTY_PERCENT;
            }
        }
    }
    return infected;
}
/*Manages all the folder reading, responsible for getting each file out of the folder, calling the functions the scan it and calling the functions
that construct the output
input: a pointer to the folder path, a pointer to the virus signature path, the action the user chosen, a pointer to
the data in the virus signature file and it's length in long
output: a pointer to a dynamic array that contains the text we need to print and put in the file*/
char* manageFolderReading(char* folderPath, char* virusSignaturePath, int action, char* virusSignature, long virusSignatureLen)
{
    printf("Scanning:\n");
    struct dirent* dir = 0;
    DIR* d = NULL;
    int infectionValue = 0;
    bool successfulFunc = true;
    FILE* file = NULL;
    char* buffer = NULL;
    char* fileName = NULL;
    char* filePath = NULL;
    char* msg = NULL;
    char** filesNames = NULL;
    int fileNameLen = 0;
    int folderPathSize = strlen(folderPath) + NULL_OPERATOR_SPACE;
    int currFile = 0;
    long bufferLen = 0;
    int amountOfFiles = countFilesInFolder(folderPath);
    int i = 0;
    if(amountOfFiles == COUNT_FILES_ERROR)
    {
        successfulFunc = false;
    }
    else
    {
        filesNames = (char**)malloc((amountOfFiles + NULL_OPERATOR_SPACE) * sizeof(char*));
        if(filesNames == NULL)
        {
            printf("Unsuccessful malloc!\n");
            successfulFunc = false;
        }
        d = opendir(folderPath);
        if (d == NULL)
        {
            printf("Error opening directory\n");
            successfulFunc = false;
        }
    }
    if(successfulFunc)
    {
        //Inserting all the files names into an array of pointers and sorting it
        msg = createMsgStart(action, folderPath, virusSignaturePath);
        while ((dir = readdir(d)) != NULL && successfulFunc)
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && dir->d_type != DT_DIR)
            {
                fileName = dir->d_name;
                filesNames[currFile] = (char*)malloc((strlen(dir->d_name) + NULL_OPERATOR_SPACE) * sizeof(char));
                if(filesNames[currFile] == NULL)
                {
                    printf("Unsuccessful malloc!\n");
                    successfulFunc = false;
                }
                else
                {
                    strcpy(filesNames[currFile], fileName);
                    filesNames[currFile][strlen(dir->d_name)] = '\0';
                    currFile++;
                }
            }
        }
        
        if(successfulFunc)
        {
            sortFilesAlphabetically(filesNames, currFile);
        }
        for(int i = 0; i < currFile && successfulFunc; i++)
        {
            fileName = filesNames[i];
            filePath = makeFilePath(fileName, folderPath, folderPathSize);
            if(filePath == NULL)
            {
                printf("Unsuccessful memory allocation\n");
                successfulFunc = false;
            }
            else
            {
                file = getFile(filePath);
                successfulFunc = file != NULL;
                if(successfulFunc)
                {
                    bufferLen = getFileLen(file);
                    buffer = readDataFromBinaryFile(file);
                    successfulFunc = buffer != NULL;
                    if(successfulFunc)
                    {
                        if(action == NORMAL_SCAN)
                        {
                            infectionValue = sendNormalScan(virusSignature, bufferLen, buffer, virusSignatureLen);                            
                        }
                        else
                        {
                            infectionValue = sendQuickScan(virusSignature, bufferLen, buffer, virusSignatureLen);
                        }
                        msg = insertFileDataToMsg(filePath, infectionValue, msg, strlen(msg));
                        successfulFunc = msg != NULL;
                        free(filePath);
                        fclose(file);
                        free(buffer);
                    }
                }
            }
        }
        freeArrOfPointers(filesNames, currFile);
        closedir(d);
    }
    printf("Scan Completed.\n");
    return msg;
}
/*Frees all the pointer in an array of pointers (has to be pointer to char)
input: the array of pointer and the amount of pointers
it contains
output: none*/
void freeArrOfPointers(char** arr, int size)
{
    int i = 0;
    for(i = 0; i < size; i++)
    {
        free(arr[i]);
    }
}
/*Sorts the files by alphabetical order
input: an array of pointers containing the files names and the amount
of files in the folder
output: none*/
void sortFilesAlphabetically(char** filesNames, int filesCount)
{
    int i = 0, j = 0;
    char* temp = NULL;
    bool swapped = true;
    for (i = 0; i < filesCount - 1 && swapped; i++)
    {
        swapped = false;
        for (j = 0; j < filesCount - i - 1; j++)
        {
            if (strcmp(filesNames[j], filesNames[j + 1]) > 0)
            {
                temp = filesNames[j];
                filesNames[j] = filesNames[j + 1];
                filesNames[j + 1] = temp;
                swapped = true;
            }
        }
    }
}
/*Counts the amount of files in the folder the user requested to scan
input: a pointer to the file's path
output: the amount of files in the folder*/
int countFilesInFolder(char* folderPath)
{
    int counter = 0;
    DIR* d = opendir(folderPath);
    struct dirent* dir;
    if(d == NULL)
    {
        counter = COUNT_FILES_ERROR;
    }
    else
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") && dir->d_type != DT_DIR)
            {
                counter++;
            }
        }
    }
    closedir(d);
    return counter;
}
/*Scans a file from a certain percent range an sees if the file is infected, clean or to small to contain the virus signature
input: the percent to scan from, the percent to scan to, a pointer to the data virus signature contains, it's length in long
a pointer to the data in the current file and it's length in long
output: an int representing if the file is infected, clean or to small to contain the virus signature*/
int scanFile(int startpercent, int finishpercent, char* virusSignature, long bufferLen, char* buffer, long virusSignatureLen)
{
    bool currInfected = false;
    bool foundInfection = false;
    int infected = CLEAN;
    float start = (float)startpercent / DIVISION_TO_GET_PRECENT;
    float finish = (float)finishpercent / DIVISION_TO_GET_PRECENT;
    long startByte = (int)(start * bufferLen);
    long finishByte = (int)(finish * bufferLen);
    long currByte = 0;
    long cmpByte = 0;
    /*currByte <= finishByte - virusSignatureLen + 1 means that if for example the buffer is "abcd" and the virus is "vi" so the finish index will be 4 (100%)
    and the virus length will be 2. However, if we calculate that again it becomes 3 (4 - 2 + 1) which is the actual last index we need to scan to because
    if we haven't found a match yet and we are at index 3 the virus signature can't start there because it's 2 chars long and there is only one car left*/
    long byteToReadTo = finishByte - virusSignatureLen + 1;
    if(byteToReadTo < startByte)
    {
        byteToReadTo = startByte;
    }
    if(virusSignatureLen > bufferLen)
    {
        infected = FILE_TOO_SMALL_TO_CONTAIN_SIGNATURE;
    }
    else
    {
        for(currByte = startByte; currByte <= byteToReadTo && !foundInfection; currByte++)
        {
            currInfected = true;
            for(cmpByte = 0; cmpByte < virusSignatureLen && currInfected && !foundInfection; cmpByte++)
            {
                if(buffer[currByte + cmpByte] != virusSignature[cmpByte])
                {
                    currInfected = false;
                }
            }
            if(currInfected)
            {
                infected = INFECTED;
                foundInfection = true;
            }
        }
        if(!foundInfection)
        {
            infected = CLEAN;
        }
    }
    return infected;
}
/*Adds a certain file and it's state (clean/ infected) to the message
input: a pointer to the file path as string, an int representing the infection state, a pointer
to the already existsing message and its length
output: a pointer to a dynamic array representing the updated message*/
char* insertFileDataToMsg(char* filePath, int infectionValue, char* msg, int msgLen)
{
    int newMsgLen = 0;
    char* fileState = NULL;
    switch(infectionValue)
    {
        case CLEAN:
            fileState = "- Clean\n";
            break;
        case INFECTED:
            fileState = "- Infected!\n";
            break;
        case FIRST_TWENTY_PERCENT:
            fileState = "- Infected! (first 20%)\n";
            break;
        case LAST_TWENTY_PERCENT:
            fileState = "- Infected! (last 20%)\n";
            break;
        case FILE_TOO_SMALL_TO_CONTAIN_SIGNATURE:
            fileState = "- Clean (file to small to contain virus signature)\n";
            break;
        case INFECTED_BUT_NOT_IN_FIRST_AND_LAST_TWENTY_PERCENT:
            fileState = "- Infected! (not found in the first and last 20%)\n";
            break;
    }
    newMsgLen = strlen(fileState) + msgLen + strlen(filePath) + NULL_OPERATOR_SPACE;
    msg = (char*)realloc(msg, newMsgLen * sizeof(char));
    if(msg == NULL)
    {
        printf("Unsuccessful realloc!\n");
    }
    else
    {
        strncat(msg, filePath, strlen(filePath));
        strncat(msg, fileState, strlen(fileState));
        printf("%s %s", filePath, fileState);
        msg[newMsgLen] = '\0';
    }
    return msg;
}
/*Creates the first part of the message which mostly contains the info about the scanning
input: an int representing the scanning method, a pointer to the path of the folder we scan and
a pointer to the path of the file containing the virus signature
output: a pointer to a dynamic array that contains the message*/
char* createMsgStart(int scanningMethodInt, char* folderPathPath, char* virusSignaturePath)
{
    int totalLen = 0;
    const char* headerLineOne = "Scanning results\n";
    const char* headerLineTwo = "\nScan folder- ";
    const char* headerLineThree = "\nVirus signature- ";
    const char* headerLineFour = "\n\nScanning method- ";
    char* scanningMethod = NULL;
    char* startMsg = NULL;
    if(scanningMethodInt == NORMAL_SCAN)
    {
        scanningMethod = "Normal\n\n";
    }
    else
    {
        scanningMethod = "Quick scan\n\n";
    }
    totalLen =  strlen(headerLineOne) + strlen(headerLineTwo) + strlen(headerLineThree) + strlen(headerLineFour) + strlen(folderPathPath) + strlen(virusSignaturePath) + strlen(scanningMethod);
    startMsg = (char*)calloc(totalLen + NULL_OPERATOR_SPACE, sizeof(char));
    strncat(startMsg, headerLineOne, strlen(headerLineOne));
    strncat(startMsg, headerLineTwo, strlen(headerLineTwo));
    strncat(startMsg, folderPathPath, strlen(folderPathPath));
    strncat(startMsg, headerLineThree, strlen(headerLineThree));
    strncat(startMsg, virusSignaturePath, strlen(virusSignaturePath));
    strncat(startMsg, headerLineFour, strlen(headerLineFour));
    strncat(startMsg, scanningMethod, strlen(scanningMethod));
    startMsg[totalLen] = '\0';
    return startMsg;
}
/*Inserts the final message into a file and prints where the user can find it
input: a pointer to the message and a pointer to the folder path
output: none*/
void insertMsgToFile(char* msg, char* folderPath)
{
    char* fileName = "AntiVirusLog.txt";
    char* filePath = makeFilePath(fileName, folderPath, strlen(folderPath));
    int currLetter = 0;
    FILE* logFile = fopen(filePath, "w");
    if(logFile == NULL)
    {
        printf("Unsuccessful file opening!\n");
    }
    else
    {
        for(currLetter = 0; currLetter < strlen(msg); currLetter++)
        {
            fputc(msg[currLetter], logFile);
        }
        fclose(logFile);
        printf("See log path for results: %s\n", filePath);
    }
    free(filePath);
}