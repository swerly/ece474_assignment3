//
// Created by sethw on 11/27/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "lists.h"

#define LINE_SIZE 80

void beginParsing(mainContainer* container){
    //const int LINE_SIZE = 80;
    FILE* netlistIn;
    char currentLine[LINE_SIZE];
    char* token;
    char* commentCheck;

    //open input file for reading
    netlistIn = fopen(container->inputFilename, "r");

    //check if our input can be found, if not exit
    if (netlistIn == NULL){
        printf("\nFile \"%s\" not found. Exiting...\n\n", container->inputFilename);
        return;
    }
    //start scanning line by line
    while( fgets(currentLine, LINE_SIZE, netlistIn)) {

        //get first token
        token = strtok(currentLine, " \t");
        if (token == NULL){
            continue;
        }

        //allocate string to check and see if the first 2 chars
        //of the current line are a comment
        commentCheck = (char *) malloc(sizeof(char) * 3);
        //copy the first 2 chars from the string to our commentCheck var
        strncpy(commentCheck, token, 2);
        //set the null terminating flag
        commentCheck[2] = '\0';

        //check if commentCheck is equal to a comment
        //if so free and exit, else free and continue
        if ((strcmp(commentCheck, "//") == 0) || (strcmp(commentCheck, "\n") == 0)) {
            //this is a comment line or a new line and we can skip it
            free(commentCheck);
            continue;
        }
        free(commentCheck);

        //while our current token isn't NULL
        while (token != NULL) {
            //check what we need to do with the current line
            if (strcmp(token, INPUT_STRING) == 0) {
                parseVariables(&(container->variables), INPUT);
            } else if (strcmp(token, OUTPUT_STRING) == 0) {
                parseVariables(&(container->variables), OUTPUT);
            } else if (strcmp(token, VARIABLE_STRING) == 0) {
                parseVariables(&(container->variables), VARIABLE);
            } else {
                //parsing c code
            }

            token = strtok(NULL, " \t\n");
        }
    }
}

int getWidth(char* token) {
    switch (token[strlen(token) - 1]) {
        case '1':
            return 1;
        case '2':
            return (token[strlen(token) - 2] == '3') ? 32 : 2;
        case '4':
            return 64;
        case '6':
            return 16;
        case '8':
            return 8;
        default:
            return -1;
    }
}

int isDataType(char* token) {
    return (strncmp(token, "Int", 3) == 0) || (strncmp(token, "UIn", 3) == 0);
}

void parseVariables(varNode** variableList, variableType type){
    varNode* tempVar;
    char* token = strtok(NULL, " ,\t\n");
    int currentIsSigned = -1;
    int currentWidth = -1;
    char* tempName;

    //while we are still pulling things from the line
    while (token != NULL) {
        //if we need to set the initial attributes
        if (isDataType(token)) {
            //set attributes based on dataType token
            currentIsSigned = (token[0] == 'I') ? 1 : 0;
            currentWidth = getWidth(token);
        } else {
            //make a new node and add it to the list
            tempVar = (varNode*)malloc(sizeof(varNode));
            tempVar->isSigned = currentIsSigned;
            tempName = (char*)malloc(sizeof(char)*strlen(token));
            strcpy(tempName, token);
            tempVar->name = tempName;
            tempVar->width = currentWidth;
            tempVar->next = NULL;
            tempVar->varType = type;

            addToVarList(variableList, tempVar);
        }

        token = strtok(NULL, " ,\t\n");
    }
}