all: parsers Kompilator

Kompilator: Kompilator.c
	gcc  -g -std=c11  -c "D:\Programming\c++\ak2 gcc\Compiler\Kompilator.c" -o obj\Debug\Kompilator.o
	gcc -o bin\Debug\Compiler.exe obj\Debug\expression_parser.o obj\Debug\instruction_parser.o obj\Debug\Kompilator.o 

parsers: expression_parser.c instruction_parser.c
	gcc  -g -std=c11  -c "D:\Programming\c++\ak2 gcc\Compiler\instruction_parser.c" -o obj\Debug\instruction_parser.o
	gcc  -g -std=c11  -c "D:\Programming\c++\ak2 gcc\Compiler\expression_parser.c" -o obj\Debug\expression_parser.o