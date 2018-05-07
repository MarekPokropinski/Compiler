/*
Kompilator w�asego j�zyka programowania

Marek Pokropi�ski
Hubert Wnukiewicz

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction_parser.h"



int main(int argc, char** argv)
{
    printf("Kompilator wykonany na projekt AK\nPolitechnika Wroclawska\n\n");
	if (argc == 2)
	{
		printf("opening file %s\n", argv[1]);
		FILE* f = fopen(argv[1], "r");
		if (f)
		{

		}
	}
	else
	{
		Compile("abc.txt", "out.s");

		system("as out.s -o out.o");
		system("as prolog.s -o prolog.o");
		system("ld prolog.o out.o -L \"D:\\Programs\\codeblocks\\MinGW\\lib\" -lmsvcrt -lkernel32 -entry=_start_prolog -subsystem=console -o out.exe");

		printf("Press any key to exit");

		getchar();
	}

}
