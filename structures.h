#pragma once

#ifndef NULL
#define NULL 0
#endif // NULL


typedef _Bool bool;
#define true 1
#define false 0

typedef enum Symbol_enum
{
	Number,
	Name,
	Operator,
	Endline,
	OpenBracket,
	CloseBracket,
	OpenBlock,
	CloseBlock,
	Eof
}Symbol;

typedef struct Symbol_s_stucture
{
	Symbol symbol;
	const char * text;
}Symbol_s;

typedef struct AST_struct
{
	Symbol_s symbol;
	struct AST_struct *left, *right;
}AST;

enum
{
	value = 0,
	pointer
};


static const char* operators1[] = { "=",NULL };
static const char* operators2[] = { "==","!=","<",">","<=",">=",NULL };
static const char* operators3[] = { "+","-",NULL };
static const char* operators4[] = { "*","/",NULL };
static const char** operators[] = { operators1,operators2,operators3,operators4, NULL };
//static const char* names[] = {"while","for","if",NULL};

