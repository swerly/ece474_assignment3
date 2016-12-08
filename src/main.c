//
// Created by sethw on 11/17/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lists.h"
#include "outputWriter.h"
#include "scheduler.h"
#include "linkOperationNodes.h"

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
    container.outputFilename = argv[3];
    container.scheduledNodes = scheduledNodes;
    for (i = 0; i < container.maxLatency; i++){
        container.scheduledNodes[i] = NULL;
    }

    beginParsing(&container);
    if(container.errorCode == 0)
    {
        linkOpNodes(&container);
        linkOpNodes(&container);
        if(startListR(&container)==-1)return 0;
        //printCycles(&container);
        writeFile(&container);
    }
    else
    {
        return container.errorCode;
    }

    return 0;
}
