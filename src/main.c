//
// Created by sethw on 11/17/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lists.h"
#include "scheduler.h"
#include "linkOperationNodes.h"

void printCycles(mainContainer* mCont);

int main(int argc, char** argv){
    mainContainer container;
    operationArrayNode* scheduledNodes[atoi(argv[2])];
    int i;

    container.variables = NULL;
    container.operations = NULL;
    container.ifNodeList = NULL;
    container.errorCode = 0;

    //check for correct argument count
    if (argc != 4){
        printf("\nUsage:\n\nhlsyn cFile latency verilogFile\n\n");
        return 0;
    }
    container.maxLatency = atoi(argv[2]);
    strcpy(container.inputFilename, argv[1]);
    container.scheduledNodes = scheduledNodes;
    for (i = 0; i < container.maxLatency; i++){
        container.scheduledNodes[i] = NULL;
    }

    printf("Starting Parsing...");
    beginParsing(&container);
    if(container.errorCode == 0)
    {
        printf("Linking Operations...");
        linkOpNodes(&container);
        printf(" Done\nLinking Operations...");
        linkOpNodes(&container);
        printf(" Done\nScheduling Operations...\n");
        if(startListR(&container)==-1)return 0;
        printCycles(&container);
        printf(" Done\n");
    }
    else
    {
        return container.errorCode;
    }

    return 0;
}

void printCycles(mainContainer* mCont){
    operationArrayNode* temp;
    int i;

    for (i = 0; i<mCont->maxLatency; i++){
        printf("\ncycle %d: ", i+1);
        temp = mCont->scheduledNodes[i];
        while (temp!=NULL){
            printf("%s, ", temp->element->output->name);

            temp=temp->next;
        }
    }
    printf("\n\n");
}