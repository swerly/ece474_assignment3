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
    char operation;
	char* printstr;
	varNode* input1;
	varNode* input2;
	varNode* input3;
    varNode* output;
    struct opArrayNode* dependents;
    struct opArrayNode* dependencies;
    int ALAPcycle;
    int ListRcycle;
    struct opNode* next;
} operationNode;

typedef struct opArrayNode{
    operationNode * element;
    struct opArrayNode* next;
} operationArrayNode;
//structure to hold all of our lists to pass easily between functions
typedef struct{
    varNode* variables;
    char inputFilename[30];
} mainContainer;


void addToList(varNode** list, varNode* varToAdd);
void printList(varNode* head);
#endif //ECE474_ASSIGNMENT3_LISTS_H
