//
// Created by sethw on 11/27/2016.
//
//adds a node to the specified list
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lists.h"


void addToVarList(varNode** list, varNode* varToAdd){
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

void addToOpArrayList(operationArrayNode** list, operationArrayNode* nodeToAdd)
{
    operationArrayNode* tNode = *list;
    if (tNode == NULL){
        *list = nodeToAdd;
        return;
    }
    while (tNode->next != NULL){
        tNode = tNode->next;
    }
    tNode->next = nodeToAdd;
}

void addToIfNodeList(ifNodes** list, ifNodes* nodeToAdd)
{
    ifNodes* tNode1 = *list;
    ifNodes* tNode2 = *list;
    if(tNode1 == NULL)
    {
        *list = nodeToAdd;
        return;
    }

    while (tNode1->next != NULL)
    {
        tNode1 = tNode1->next;
        tNode2 = tNode1;
    }
    tNode1->next = nodeToAdd;
    nodeToAdd->prev = tNode2;

}

void addToSlackList(slackNode** list, slackNode* nodeToAdd)
{
    slackNode* tNode = *list;
    if (tNode == NULL){
        *list = nodeToAdd;
        return;
    }
    while (tNode->next != NULL){
        tNode = tNode->next;
    }
    tNode->next = nodeToAdd;
}

void initOpNode(operationNode* newNode)
{
    newNode->operation = NULL;
    newNode->opType = -1;
    newNode->alapCycle = -1;
    newNode->listRCycle = -1;
    newNode->dependencies = NULL;
    newNode->dependents = NULL;
    newNode->input1 = NULL;
    newNode->input2 = NULL;
    newNode->input3 = NULL;
    newNode->output = NULL;
    newNode->next = NULL;
    newNode->printstr = NULL;
}

void addOpToSchedule(operationArrayNode** list, operationNode* nodeToAdd){
    operationArrayNode* tNode = *list, *add;

    add = (operationArrayNode*)malloc(sizeof(operationArrayNode));
    add->element = nodeToAdd;
    add->next = NULL;

    if (tNode == NULL){
        *list = add;
        return;
    }
    while (tNode->next != NULL){
        tNode = tNode->next;
    }
    tNode->next = add;
}

void freeOpArrayNodeList(operationArrayNode* head){
    operationArrayNode* temp;
    while (head!=NULL){
        temp = head->next;
        free(head);
        head = temp;
    }
}