#include "expression_parser.h"



AST* generateSyntaxTree(const Symbol_s* symbols, int start, int end)
{
	if (start == end)
	{
		if (symbols[start].symbol != Number && symbols[start].symbol != Name)
		{
			printf("Error while parsing! Expected variable instead got %s\n", symbols[start].text);
			return NULL;
		}
		else
		{
			AST* ast = (AST*)malloc(sizeof(AST));
			ast->symbol = symbols[start];
			ast->left = NULL;
			ast->right = NULL;
			return ast;
		}
	}

	if (symbols[start].symbol == OpenBracket && symbols[end].symbol == CloseBracket)
	{
		int i = 1;
		int bracketCnt = 1;
		while (start + i <= end)
		{
			if (symbols[start + i].symbol == OpenBracket)
			{
				bracketCnt++;
			}
			else if (symbols[start + i].symbol == CloseBracket)
			{
				bracketCnt--;
			}
			if (bracketCnt == 0)
			{
				if (start + i == end)
				{
					return generateSyntaxTree(symbols, start + 1, end - 1);
				}
				break;
			}
			i++;
		}

	}

	int bracketCnt = 0;

	int j = 0;
	while (operators[j] != NULL)
	{
		int i = end - start;
		while (i >= 0)
		{
			if (bracketCnt == 0 && symbols[start + i].symbol == Operator)
			{
				int k = 0;
				while (operators[j][k] != NULL)
				{
					if (strcmp(operators[j][k], symbols[start + i].text) == 0)
					{
						AST* tree = (AST*)malloc(sizeof(AST));
						tree->symbol.text = symbols[start + i].text;
						tree->symbol.symbol = Operator;

						tree->left = generateSyntaxTree(symbols, start, start + i - 1);
						tree->right = generateSyntaxTree(symbols, start + i + 1, end);
						return tree;
					}
					k++;
				}
			}
			else if (symbols[start + i].symbol == OpenBracket)
			{
				bracketCnt++;
			}
			else if (symbols[start + i].symbol == CloseBracket)
			{
				bracketCnt--;
			}
			i--;
		}
		j++;
	}

	return NULL;
}

char* readline(char* input, char* output) // Zwraca wska�nik na reszt� bufora wej�ciowego
{
	if (*(input - 1) == '\0')
		return NULL;
	while (*input != '\n'&&*input != '\0')
	{
		*(output++) = *(input++);
	}
	*output = '\0';
	return input + 1;
}

bool czyLitera(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z'))
		return true;
	return false;
}
bool czyCyfra(char c)
{
	if (c >= '0'&&c <= '9')
		return true;
	return false;
}

bool czyOperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '='|| c == '>'|| c == '<'|| c == '!')
		return true;
	return false;
}
