//
// Created by sethw on 12/7/2016.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lists.h"

int bitCount(unsigned int n);
void printVars(mainContainer* mContainer, FILE* outp, variableType type);

void writeFile(mainContainer *mContainer){
    FILE* outp = fopen(mContainer->outputFilename, "w");
    operationArrayNode* tempNode;
    operationNode* tempOp;
    varNode* tempVar;

    int i;

    if (mContainer->printBlank){
    }

    if (outp == NULL){
        printf("Couldn't open \"%s\" for writing", mContainer->outputFilename);
        return;
    }



    //print header and module declaration
    fprintf(outp, "`timescale 1ns / 1ps\n\n");

    if (mContainer->printBlank){
        fprintf(outp, "//***************************ERROR***************************\n");
        fprintf(outp, "//***********************************************************\n\n//");
        switch (mContainer->errorCode){
            case 69:
                fprintf(outp, "Operations could not be schedule within the time constraint, printing blank schedule...", mContainer->maxLatency);
                break;
            case 98:
                fprintf(outp, "We tried, but were not able to successfully schedule if statements");
                break;
            case 100:
                fprintf(outp, "Output %s does not exist, printing blank schedule...", mContainer->errorCausingString);
                break;
            case 101:
                fprintf(outp, "Input %s does not exist, printing blank schedule...", mContainer->errorCausingString);
                break;
            case 102:
                fprintf(outp, "%s is an invalid operation, printing blank schedule...", mContainer->errorCausingString);
                break;

        }
        fprintf(outp, "\n\n//***********************************************************\n");
        fprintf(outp, "//***********************************************************\n\n");

    }


    fprintf(outp, "module HLSM (Clk, Rst, Start, Done");
    tempVar = mContainer->variables;
    while (tempVar != NULL){
        if (tempVar->varType == INPUT || tempVar->varType == OUTPUT){
            fprintf(outp, ", %s",tempVar->name);
        }
        tempVar = tempVar->next;
    }
    fprintf(outp, ");\n    input Clk, Rst, Start;\n    output reg Done;\n");

    printVars(mContainer, outp, INPUT);
    printVars(mContainer, outp, OUTPUT);
    printVars(mContainer, outp, VARIABLE);

    if (mContainer->printBlank) mContainer->maxLatency = 0;
    //get the bit width of the max cycles and
    i = bitCount(mContainer->maxLatency + 2);
    fprintf(outp, "\n    reg [0:%d] State, NextState;\n\n", i-1);
    fprintf(outp, "    initial begin\n");
    tempVar = mContainer->variables;
    while (tempVar != NULL){
        if (tempVar->varType == VARIABLE){
            fprintf(outp, "        %s <= 0;\n", tempVar->name);
        }
        tempVar = tempVar->next;
    }
    fprintf(outp, "        State <= 0;\n        NextState <= 0;\n    end\n\n");

    //state register behavior
    fprintf(outp, "    always @(posedge Clk) begin\n");
    fprintf(outp, "        if (Rst == 1) begin\n");
    fprintf(outp, "            State <= 0;\n");
    fprintf(outp, "            NextState <= 0;\n");
    tempVar = mContainer->variables;
    while (tempVar != NULL){
        if (tempVar->varType == VARIABLE){
            fprintf(outp, "            %s <= 0;\n", tempVar->name);
        }
        tempVar = tempVar->next;
    }
    fprintf(outp, "        end\n        else begin\n");
    fprintf(outp, "            State <= NextState;\n");
    fprintf(outp, "        end\n");
    fprintf(outp, "    end\n\n");


    fprintf(outp, "    always @(State) begin\n");
    fprintf(outp, "        case (State)\n");
    fprintf(outp, "            0: begin\n");
    fprintf(outp, "                Done <= 0;\n");
    fprintf(outp, "                if (Start == 0)\n");
    fprintf(outp, "                    NextState <= 0;\n");
    fprintf(outp, "                else\n");
    fprintf(outp, "                    NextState <= 1;\n");
    fprintf(outp, "            end\n");

    for (i = 0; i < mContainer->maxLatency; i++){
        tempNode = mContainer->scheduledNodes[i];
        fprintf(outp, "            %d: begin\n", i+1);
        while (tempNode!=NULL){
            tempOp = tempNode->element;
            if (strcmp(tempOp->operation, "?") == 0)
                fprintf(outp, "                %s <= %s ? %s : %s;\n",
                    tempOp->output->name, tempOp->input1->name, tempOp->input2->name, tempOp->input3->name);
            else if (strcmp(tempOp->operation, ">>") == 0)
                fprintf(outp, "                %s <= %s %s %s;\n",
                        tempOp->output->name, tempOp->input1->name, tempOp->input1->isSigned ? ">>>" : ">>", tempOp->input2->name);
            else
                fprintf(outp, "                %s <= %s %s %s;\n",
                    tempOp->output->name, tempOp->input1->name, tempOp->operation, tempOp->input2->name);

            tempNode = tempNode->next;
        }
        fprintf(outp, "                NextState <= %d;\n", i+2);
        fprintf(outp, "            end\n");
    }


    fprintf(outp, "            %d: begin\n",i+1);
    fprintf(outp, "                Done <= 1;\n");
    fprintf(outp, "                NextState <= 0;\n");
    fprintf(outp, "            end\n");
    fprintf(outp, "        endcase\n");
    fprintf(outp, "    end\n");
    fprintf(outp, "\nendmodule\n");
    fclose(outp);
}

int bitCount(unsigned int n){
    int counter = 0;
    while(n) {
        n = n/2;
        if(n)counter++;
    }
    return counter+1;
}

void printVars(mainContainer* mContainer, FILE* outp, variableType type){
    int i, j, printedVar = 0;
    int bitWidths[] = {1, 2, 8, 16, 32, 64};
    varNode* tempVar;
    //print inputs
    for (i = 0; i<6; i++){
        for (j = 0; j<2; j++) {
            tempVar = mContainer->variables;
            while (tempVar != NULL) {
                if (j == 0){
                    if (tempVar->isSigned == 1 && tempVar->varType == type && tempVar->width == bitWidths[i]) {
                        if (type == INPUT){
                            if (i == 0) fprintf(outp, "\n    input signed %s", tempVar->name);
                            else fprintf(outp, "\n    input signed [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        } else if (type == OUTPUT){
                            if (i == 0) fprintf(outp, "\n    output reg signed %s", tempVar->name);
                            else fprintf(outp, "\n    output reg signed [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        } else if (type == VARIABLE) {
                            if (i == 0) fprintf(outp, "\n    reg signed %s", tempVar->name);
                            else fprintf(outp, "\n    reg signed [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        }
                        tempVar = tempVar->next;
                        while (tempVar != NULL) {
                            if (tempVar->isSigned == 1 && tempVar->varType == type && tempVar->width == bitWidths[i]) {
                                fprintf(outp, ", %s", tempVar->name);
                            }
                            tempVar = tempVar->next;
                        }
                    }
                    tempVar = tempVar == NULL ? NULL : tempVar->next;
                    if (tempVar == NULL && printedVar == 1){
                        fprintf(outp, ";\n");
                        printedVar = 0;
                    }
                } else {
                    if (tempVar->isSigned == 0 && tempVar->varType == type && tempVar->width == bitWidths[i]) {
                        if (type == INPUT){
                            if (i == 0) fprintf(outp, "\n    input %s", tempVar->name);
                            else fprintf(outp, "\n    input [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        } else if (type == OUTPUT){
                            if (i == 0) fprintf(outp, "\n    output %s", tempVar->name);
                            else fprintf(outp, "\n    output reg [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        } else if (type == VARIABLE) {
                            if (i == 0) fprintf(outp, "\n    reg %s", tempVar->name);
                            else fprintf(outp, "\n    reg [0:%d] %s", bitWidths[i] - 1, tempVar->name);
                            printedVar = 1;
                        }
                        tempVar = tempVar->next;
                        while (tempVar != NULL) {
                            if (tempVar->isSigned == 0 && tempVar->varType == type && tempVar->width == bitWidths[i]) {
                                fprintf(outp, ", %s", tempVar->name);
                            }
                            tempVar = tempVar->next;
                        }
                    }
                    tempVar = tempVar == NULL ? NULL : tempVar->next;
                    if (tempVar == NULL && printedVar == 1){
                        fprintf(outp, ";\n");
                        printedVar = 0;
                    }
                }
            }
        }
    }
}