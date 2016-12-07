//
// Created by Bryan on 12/1/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkOperationNodes.h"
#include "lists.h"

void linkOpNodes(mainContainer* container)
{
    operationNode* currOpNode;                                  //current node being looked at to determine linking
    operationNode* searchNode;                                  //node being compared to inputs

    operationArrayNode* newOpArrayNodeIn;
    operationArrayNode* newOpArrayNodeOut;

    int found1 = 0, found2 = 0, found3 = 0;                     //search flags for all possible inputs

    currOpNode = container->operations;                         //sets the current node to the top of the list
    searchNode = container->operations->next;                   //starts the search at the node after because nodes cant be reliant on themselves

    while(currOpNode != NULL)
    {
        if(currOpNode->input1->varType == VARIABLE || currOpNode->input2->varType == VARIABLE || currOpNode->input3 != NULL)
        {
            while(searchNode != NULL && searchNode != currOpNode)//loop assumes that all dependencies have to come before the current node
            {
                if(!strcmp(searchNode->output->name,currOpNode->input1->name) && found1 == 0)
                {//if the output of a node matches the input of the current node and the node input hasnt been found yet
                    newOpArrayNodeIn = (operationArrayNode*)malloc(sizeof(operationArrayNode));//allocates memories for the new nodes
                    newOpArrayNodeOut = (operationArrayNode*)malloc(sizeof(operationArrayNode));
                    newOpArrayNodeIn->element = searchNode;  //assigns the nodes to the new opArray nodes
                    newOpArrayNodeOut->element = currOpNode;
                    addToOpArrayList(&currOpNode->dependencies,newOpArrayNodeIn);   //adds the new oparraynodes
                    addToOpArrayList(&searchNode->dependents, newOpArrayNodeOut);
                    found1 = 1; //sets the found flag
                }
                else if(!strcmp(searchNode->output->name,currOpNode->input2->name) && found2 == 0)
                {//this is an else if to avoid the same node being linked twice
                    newOpArrayNodeIn = (operationArrayNode*)malloc(sizeof(operationArrayNode));
                    newOpArrayNodeOut = (operationArrayNode*)malloc(sizeof(operationArrayNode));
                    newOpArrayNodeIn->element = searchNode;
                    newOpArrayNodeOut->element = currOpNode;
                    addToOpArrayList(&currOpNode->dependencies,newOpArrayNodeIn);
                    addToOpArrayList(&searchNode->dependents, newOpArrayNodeOut);
                    found2 = 1;
                }
                else if(strcmp(currOpNode->operation,"?") == 0 && !strcmp(searchNode->output->name,currOpNode->input3->name) && found3 == 0)
                {//links the last input if the op is a mux
                    newOpArrayNodeIn = (operationArrayNode*)malloc(sizeof(operationArrayNode));
                    newOpArrayNodeOut = (operationArrayNode*)malloc(sizeof(operationArrayNode));
                    newOpArrayNodeIn->element = searchNode;
                    newOpArrayNodeOut->element = currOpNode;
                    addToOpArrayList(&currOpNode->dependencies,newOpArrayNodeIn);
                    addToOpArrayList(&searchNode->dependents, newOpArrayNodeOut);
                    found3 = 1;
                }
                searchNode = searchNode->next;
            }
        }
        //resets all flags and sets search pointers
        found1 = 0;
        found2 = 0;
        found3 = 0;
        currOpNode = currOpNode->next;          //points to the next node
        searchNode = container->operations;     //sets the search node to the top of the list
    }

    return;
}