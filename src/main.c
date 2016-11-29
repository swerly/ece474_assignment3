//
// Created by sethw on 11/17/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lists.h"

int main(int argc, char** argv){
    mainContainer container;
    container.variables = NULL;
    container.operations = NULL;

    //check for correct argument count
    if (argc < 4){
        printf("\nUsage:\n\nhlsyn cFile latency verilogFile\n\n");
        return 0;
    }

    strcpy(container.inputFilename, argv[1]);

    printf("Starting Parsing...");
    beginParsing(&container);

    //TODO: start to write verilog file definition

    //TODO: parse inputs / outputs

    //TODO: add inputs / outputs as wires/variables (I think?)

    //TODO: parse program code

    //TODO: append program code to verilog file

    //TODO: compute scheduling using LIST_R
    //printList(container.variables);
    return 0;
}