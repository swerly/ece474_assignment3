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
    char* operation;                    //holds the string that represents the operation cant just be a char because >> exists
	char* printstr;                     //left in for now, probably not gonna stay
	varNode* input1;
	varNode* input2;
	varNode* input3;
    varNode* output;
    struct opArrayNode* dependents;
    struct opArrayNode* dependencies;
    int ALAPcycle;
    int ListRcycle;
	int opType; 						//corresponds to 0-2 where 0 is add/sub, 1 is logic, 2 is mult
    struct opNode* next;
} operationNode;

typedef struct opArrayNode{
    operationNode * element;
    struct opArrayNode* next;
} operationArrayNode;

//structure to hold all of our lists to pass easily between functions
typedef struct{
    varNode* variables;
	operationNode* operations;
    char inputFilename[30];
	//operationArrayNode* operationNodes;
	int maxLatency;
} mainContainer;


void addToVarList(varNode** list, varNode* varToAdd);
void addToOpList(operationNode** list, operationNode* opToAdd);
void initOpNode(operationNode* newNode);
void addToOpArrayList(operationArrayNode** list, operationArrayNode* nodeToAdd);
void printList(varNode* head);
#endif //ECE474_ASSIGNMENT3_LISTS_H
