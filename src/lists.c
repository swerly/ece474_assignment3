//
// Created by sethw on 11/27/2016.
//
//adds a node to the specified list
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lists.h"

void addToList(varNode** list, varNode* varToAdd){
    varNode* tNode = *list;

    //if our list is null, set the new variable as the head
    if (tNode == NULL) {
        *list = varToAdd;
        return;
    }

    //loop through until we find the last node
    while (tNode->next != NULL)
        tNode = tNode->next;

    //make the last node point to the new last node
    tNode->next = varToAdd;
}
void addToOpList(operationNode** list, operationNode* opToAdd)
{
    operationNode* tOpNode = *list;
    if (tOpNode == NULL)
    {
        *list = opToAdd;
        return;
    }
    while(tOpNode->next != NULL)
    {
        tOpNode = tOpNode->next;
    }

    tOpNode->next = opToAdd;
}

//prints the list starting at the header
void printList(varNode* head) {
    varNode* tNode = head;
    char type[10];

    while (tNode != NULL) {
        switch(tNode->varType){
            case INPUT:
                strcpy(type, "INPUT\t");
                break;
            case OUTPUT:
                strcpy(type, "OUTPUT\t");
                break;
            case VARIABLE:
                strcpy(type, "VARIABLE");
                break;
            default:
                break;
        }
        printf("\n    %s\t%d\t%s\t%s", tNode->name, tNode->width, type, tNode->isSigned ? "Signed" : "Unsigned");
        tNode = tNode->next;
    }
}