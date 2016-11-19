//
// Created by sethw on 11/17/2016.
//

#include <stdio.h>
#include <stdlib.h>

int main(int charc, char** argv){


    //check for correct argument count
    if (argc < 3){
        printf("\nUsage:\n\nhlsyn cFile latency verilogFile\n\n");
        return 0;
    }

    //TODO: check to make sure input file exists

    //TODO: start to write verilog file definition

    //TODO: parse inputs / outputs

    //TODO: add inputs / outputs as wires/variables (I think?)

    //TODO: parse program code

    //TODO: append program code to verilog file

    //TODO: compute scheduling using LIST_R

    return 0;
}