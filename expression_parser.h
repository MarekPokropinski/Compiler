#pragma once
#include "structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


AST* generateSyntaxTree(const Symbol_s* symbols, int start, int end);


bool czyOperator(char c);
bool czyCyfra(char c);
bool czyLitera(char c);
char* readline(char* input, char* output);
