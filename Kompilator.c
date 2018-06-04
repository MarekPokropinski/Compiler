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
		printf("Kompilacja %s\n", argv[1]);
        Compile(argv[1], "out.s");
        system("as out.s -o out.o");
        system("as prolog.s -o prolog.o");
        char output_name[100];
        strcpy(output_name,"ld prolog.o out.o -L \"lib\" -lmsvcrt -lkernel32 -entry=_start_prolog -subsystem=console -o ");
        strcpy(output_name+strlen(output_name),argv[1]);
        strcpy(output_name+strlen(output_name),".exe");

        system(output_name);

        printf("Wcisnij dowoly klawisz aby zakonczyc");

        getchar();


	}
	else
	{
		Compile("abc.txt", "out.s");

		system("as out.s -o out.o");
		system("as prolog.s -o prolog.o");
		system("ld prolog.o out.o -L \"lib\" -lmsvcrt -lkernel32 -entry=_start_prolog -subsystem=console -o out.exe");

		printf("Press any key to exit");

		getchar();
	}

}
