//
// Created by sethw on 11/27/2016.
//

#ifndef ECE474_ASSIGNMENT3_LISTS_H
#define ECE474_ASSIGNMENT3_LISTS_H

typedef enum {VARIABLE, INPUT, OUTPUT} variableType;

//Structure for each variable/node in our list
typedef struct node{
    int isSigned;
    int width;
    char* name;
    variableType varType;
    struct node* next;
} varNode;

typedef struct opNode{
    varNode* input;
    varNode* output;
    struct opNode* next;
} operationNode;

//structure to hold all of our lists to pass easily between functions
typedef struct{
    varNode* variables;
    char inputFilename[30];
} mainContainer;


void addToList(varNode** list, varNode* varToAdd);
void printList(varNode* head);
#endif //ECE474_ASSIGNMENT3_LISTS_H
