#pragma once


#include "structures.h"
#include "expression_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void generateCodeRec(AST* ast, FILE*f, int type);
int variableOut(char** vars, FILE*f);
void GenerateCode(AST* ast, FILE*f, int type);

const Symbol_s* parse_single_instruction(FILE*f,const Symbol_s* s, int type);
const Symbol_s* parse_instruction_block(FILE*f, const Symbol_s* s);


Symbol_s* pass1(char* code, char***vars);

void Compile(const char * input, const char * output);

static const char* compiler_names[]={"while","if","print","read","format","for",NULL};