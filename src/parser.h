//
// Created by sethw on 11/27/2016.
//

#ifndef ECE474_ASSIGNMENT3_PARSER_H
#define ECE474_ASSIGNMENT3_PARSER_H

#include "lists.h"

#define INPUT_STRING "input"
#define OUTPUT_STRING "output"
#define VARIABLE_STRING "variable"

void beginParsing(mainContainer* container);
void parseVariables(varNode** variableList, variableType type);
int isDataType(char* token);

#endif //ECE474_ASSIGNMENT3_PARSER_H
