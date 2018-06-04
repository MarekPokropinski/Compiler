#include "instruction_parser.h"
#include <string.h>

const Symbol_s* parse_single_instruction(FILE*f,const Symbol_s* s,int type)
{
    static int ifCnt = 0;
    static int whileCnt=0;

    int start = 0;
    int end = 0;
    while (s[end].symbol != Endline)
    {
        end++;
    }
    end--;

    if(end<0)
    {
        return s+1;
    }


    if(s[start].symbol == Name)
    {
        if(strcmp(s[start].text,"if")==0)
        {
            parse_single_instruction(f,s+1,value);
            fprintf(f,"test %%eax, %%eax\njz _if_end_%d\n",ifCnt);
            s=parse_instruction_block(f,s+end+2);
            if(strcmp(s[1].text,"else")==0)
            {
                fprintf(f,"jmp _else_end_%d\n_if_end_%d:\n",ifCnt,ifCnt);
                s=parse_instruction_block(f,s+3);
                fprintf(f,"_else_end_%d:\n",ifCnt);
            }
            else
            {
                fprintf(f,"_if_end_%d:\n",ifCnt,ifCnt);
            }
            ifCnt++;
            return s;
        }
		if(strcmp(s[start].text,"while")==0)
		{
			fprintf(f,"_while_start_%d:\n",whileCnt);
			parse_single_instruction(f,s+1,value);
            fprintf(f,"test %%eax, %%eax\njz _while_end_%d\n",whileCnt);
            s=parse_instruction_block(f,s+end+2);
            fprintf(f,"jmp _while_start_%d\n_while_end_%d:\n",whileCnt,whileCnt);
			whileCnt++;
            return s;
		}
		if(strcmp(s[start].text,"print")==0)
		{
			s=parse_single_instruction(f,s+1,value);
            fprintf(f,"push %%eax\npush $format\ncall _printf\nadd $8,%%esp\n");
            return s;
		}
		if(strcmp(s[start].text,"read")==0)
		{
			s=parse_single_instruction(f,s+1,pointer);
            fprintf(f,"push %%eax\npush $scanf_format\ncall _scanf\nadd $8,%%esp\n");
            return s;
		}
		if(strcmp(s[start].text,"for")==0)
        {
            s = parse_single_instruction(f,s+1,value);
            fprintf(f,"_for_start_%d:\n",whileCnt);
			s = parse_single_instruction(f,s,value);
            fprintf(f,"test %%eax, %%eax\njz _for_end_%d\njmp _for_iter_operation_end_%d\n_for_iter_operation_%d:\n",whileCnt,whileCnt,whileCnt);
            s = parse_single_instruction(f,s,value);
            fprintf(f,"jmp _for_start_%d\n_for_iter_operation_end_%d:\n",whileCnt,whileCnt);

            end = 0;
            while (s[end].symbol != Endline)
            {
                end++;
            }
            end--;

            s=parse_instruction_block(f,s);
            fprintf(f,"jmp _for_iter_operation_%d\n_for_end_%d:\n",whileCnt,whileCnt);
			whileCnt++;
            return s;
        }
    }
	AST*tree = generateSyntaxTree(s, start, end);
	if (tree)
	{
		GenerateCode(tree, f, type);
		return s+end+2;
	}
	return NULL;
}

const Symbol_s* parse_instruction_block(FILE*f, const Symbol_s* s)
{
	if (s[0].symbol != OpenBlock)
	{
		return 0; // It's not an instruction block!
	}
	s++;

	while (s[0].symbol!=CloseBlock)
	{
	    s=parse_single_instruction(f,s,value);
	}
	return s+1;
}

void generateCodeRec(AST* ast, FILE*f, int type)
{
    static int unique_val = 0;
	switch (ast->symbol.symbol)
	{
	case Number:
		fprintf(f, "pushl $%s\n", ast->symbol.text);
		break;
	case Name:
		if (type == pointer)
		{
			fprintf(f, "pushl $%s\n", ast->symbol.text);
		}
		else
		{
			fprintf(f, "movl (%s),%%eax\n", ast->symbol.text);
			fprintf(f, "pushl %%eax\n");
		}
		break;
	case Operator:
	{
		const char * t = ast->symbol.text;
		if (strcmp(t, "=") == 0)
		{
			generateCodeRec(ast->left, f, pointer);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%eax\n");
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "movl %%eax,(%%ebx)\n");
			fprintf(f, "pushl %%eax\n");
		}
		else if (strcmp(t, "+") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%eax\n");
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "addl %%ebx,%%eax\n");
			fprintf(f, "pushl %%eax\n");
		}
		else if (strcmp(t, "-") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "subl %%ebx,%%eax\n");
			fprintf(f, "pushl %%eax\n");
		}
		else if (strcmp(t, "*") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%eax\n");
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "imul %%ebx,%%eax\n");
			fprintf(f, "pushl %%eax\n");
		}
		else if (strcmp(t, "/") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "movl $0,%%edx\n");
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "divl %%ebx\n");
			fprintf(f, "pushl %%eax\n");
		}
		else if (strcmp(t, "%") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "movl $0,%%edx\n");
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "divl %%ebx\n");
			fprintf(f, "pushl %%edx\n");
		}
		else if (strcmp(t, ">") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "jg j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
		else if (strcmp(t, "<") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "jl j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
		else if (strcmp(t, ">=") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "jge j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
		else if (strcmp(t, "<=") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "jle j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
		else if (strcmp(t, "==") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "je j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
		else if (strcmp(t, "!=") == 0)
		{
			generateCodeRec(ast->left, f, value);
			generateCodeRec(ast->right, f, value);
			fprintf(f, "popl %%ebx\n");
			fprintf(f, "popl %%eax\n");
			fprintf(f, "movl $1,%%ecx\n");
			fprintf(f, "sub %%ebx,%%eax\n");
			fprintf(f, "jne j%d\n",unique_val);
			fprintf(f, "mov $0,%%ecx\n");
			fprintf(f, "j%d:\n",unique_val++);
			fprintf(f, "pushl %%ecx\n");
		}
	}
	break;
	default:
		break;
	}
}

int variableOut(char** vars, FILE*f)
{
	if (f == NULL)
	{
		printf("Error, file not found");
		return 0;
	}
	fprintf(f, ".data \n");
	while (*vars)
	{
		fprintf(f, "%s: .long 0\n", *vars);
		vars++;
	}
	fprintf(f, "format: .string \"%%d\\n\"\n");
	fprintf(f, "scanf_format: .string \"%%d\"\n");

	return 1;
}

void Compile(const char * input, const char * output)
{
	FILE* f = fopen(input, "r");
	FILE* out = fopen(output, "w");


	size_t size;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	char* program = (char*)malloc(size);

	fseek(f, 0, SEEK_SET);
	size_t bytes_read = fread(program, 1, size, f);

	if (bytes_read != 0)
	{
		program[bytes_read] = 0;
	}

	//printf("Program:\n%s\n", program);


	char** vars;
	const Symbol_s* s = pass1(program, &vars);

	const Symbol_s* s2 = s;
	/*while (s2->symbol != Eof)
	{
		printf("%d: %s\n", s2->symbol, s2->text);
		s2++;
	}*/

	variableOut(vars, out);
	fprintf(out, ".global _main\n.text\n_main:\n");

	if (!parse_instruction_block(out, s))
	{
		printf("Error! Instruction block expected!\n");
	}
	fprintf(out, "ret\n");


	fclose(f);
	fclose(out);

}
void GenerateCode(AST* ast, FILE*f, int type)
{
	if (f)
	{
		generateCodeRec(ast, f, type);
		fprintf(f, "pop %%eax\n");
	}
	else
	{
		printf("Output file couldn't be created");
	}
}

Symbol_s* pass1(char* code,/*OUT*/char***vars)
{
	int vars_count = 0;
	char* variables[1024];
	char* line = (char*)malloc(1024);

	char* buf = (char*)malloc(128);
	Symbol_s* symbols = (Symbol_s*)malloc(1024);
	int n = 0;

	while (code = readline(code, line))
	{
		//printf("Parse line 1\n");
		char* s = line;
		int len = 0;
		while (*s != '\0')
		{
			len = 0;
			if (czyLitera(s[0])) // To jest nazwa zmiennej
			{
				symbols[n].symbol = Name;
				while (czyCyfra(*s) || czyLitera(*s))
				{
					buf[len++] = *(s++);
				}
				buf[len] = '\0';

				bool defined = false;

				for (int i = 0; i < vars_count; i++)
				{
					if (strcmp(variables[i], buf) == 0)defined = true;
				}
				for(int i=0;compiler_names[i]!=NULL;i++)
				{
					if (strcmp(compiler_names[i], buf) == 0)defined = true;
				}
				if (!defined)
				{
					variables[vars_count] = (char*)malloc(len + 1);
					strcpy(variables[vars_count], buf);
					vars_count++;
				}
			}
			else if (czyCyfra(s[0]))
			{
				symbols[n].symbol = Number;
				while (czyCyfra(*s))
				{
					buf[len++] = *(s++);
				}
				buf[len] = '\0';
			}
			else if (czyOperator(s[0]))
			{
				symbols[n].symbol = Operator;
				while (czyOperator(*s))
				{
					buf[len++] = *(s++);
				}
				buf[len] = '\0';
			}
			else if (s[0] == '(')
			{
				symbols[n].symbol = OpenBracket;
				buf[0] = '(';
				buf[1] = '\0';
				s++;
			}
			else if (s[0] == ')')
			{
				symbols[n].symbol = CloseBracket;
				buf[0] = ')';
				buf[1] = '\0';
				s++;
			}
			else if (s[0] == '{')
			{
				symbols[n].symbol = OpenBlock;
				buf[0] = '{';
				buf[1] = '\0';
				s++;
			}
			else if (s[0] == '}')
			{
				symbols[n].symbol = CloseBlock;
				buf[0] = '}';
				buf[1] = '\0';
				s++;
			}
			else if(s[0]==';')
            {
                symbols[n].symbol = Endline;
                buf[0]=';';
                buf[1]='\0';
                s++;
            }
			else
			{
				s++;
				continue;
			}
			symbols[n].text = (const char*)malloc(len + 1);
			strcpy((char*)symbols[n].text, buf);
			n++;
		}
		symbols[n].text = (char*)"";
		symbols[n++].symbol = Endline;
	}
	symbols[n].symbol = Eof;
	//printf("%d symbols\n", n);

	*vars = (char**)malloc(sizeof(char*)*(vars_count + 1));
	for (int i = 0; i < vars_count; i++)
	{
		(*vars)[i] = variables[i];
	}
	(*vars)[vars_count] = NULL;
	return symbols;
}
