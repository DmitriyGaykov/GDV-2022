#include "Polish.h"

namespace PN
{
	typedef char operation;

	bool Polish(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		auto expr = new char[100];
		auto cls = (char*)"";
		int size;
		int index;
		char* polExprs;
		char el;
		map<char, IT::Entry> dict;
		IT::Entry* ide;
		LT::Entry* lte;
		LT::LexTable lex = LT::Create(lextable.maxsize);
		
		for (int i = 0; i < lextable.size; i++)
		{
			dict.clear();
			strcpy_s(expr, cls);
			size = 0;
			el = 'a';
			
			if (
				lextable.table[i].lexema == '=' && 
				(lextable.table[i + 1].lexema != i && lextable.table[i + 2].lexema != '('))
			{
				for (int j = i + 1; lextable.table[j].lexema != ';'; j++)
				{
					if (isOperation(lextable.table[j].view))
					{
						expr[size++] = lextable.table[j].view;
					}
					else if (isBrackets(lextable.table[j].lexema))
					{
						expr[size++] = lextable.table[j].lexema;
					}
					else
					{	
						ide = new IT::Entry();

						index = IT::GetIndexByLTIndex(idtable, j);
						*ide = IT::GetEntry(idtable, index);
						expr[size++] = el;
						dict[el++] = *ide;

						delete ide;
					}
				}
				
				expr[size] = '\0';
				
				polExprs = PolishNotation(expr);
				
				LT::Add(lex, lextable.table[i]);

				for (int k = 0; k < strlen(polExprs); k++)
				{
					if (!isOperation(polExprs[k]))
					{
						
						if (strcmp(dict[polExprs[k]].id, "literal") != -1)
						{
							index = IT::IsId(idtable, dict[polExprs[k]].id);
							lextable.table[dict[polExprs[k]].idxfirstLE].idxTI = index;
							LT::Add(lex, lextable.table[dict[polExprs[k]].idxfirstLE]);
							idtable.table[index].idxfirstLE = lex.size - 1;
						}
						else
						{
							index = IT::GetIndexByLTIndex(idtable, dict[polExprs[k]].idxfirstLE);
							lextable.table[dict[polExprs[k]].idxfirstLE].idxTI = index;
							LT::Add(lex, lextable.table[dict[polExprs[k]].idxfirstLE]);
							idtable.table[index].idxfirstLE = lex.size - 1;
						}
					}
					else
					{
						lte = new LT::Entry();
						
						lte->idxTI = -1;
						lte->lexema = polExprs[k];
						lte->sn = lextable.table[i].sn;
						lte->view = polExprs[k];
						
						LT::Add(lex, *lte);
						
						delete lte;
					}
				}
				
				i += size;
			}
			else
			{
				LT::Add(lex, lextable.table[i]);
			}
		}
		
		lextable = lex;
		
		delete[] expr;

		return true;
	}

	map<operation, int> priorities =
	{
		{'(', 1},
		{')', 1},
		{'|', 2},
		{'&', 3},
		{'~', 4}
	};

	char* PolishNotation(char* expression)
	{
		/*if (!checkExpression(expression))
		{
			return false;
		}*/

		char* polishExpression = new char[strlen(expression)];
		strcpy_s(polishExpression, strlen(expression), "");

		goThrowStr(expression, polishExpression);

		return polishExpression;
	}

	bool checkExpression(char* expression)
	{
		if (
			!checkBrackets(expression) ||
			!checkOperations(expression)
			)
		{
			return false;
		}
		return true;
	}

	bool checkBrackets(char* expression)
	{
		stack<char> brackets;

		for (int i = 0; i < strlen(expression); i++)
		{
			if (expression[i] == '(')
			{
				brackets.push(expression[i]);
			}
			else if (expression[i] == ')')
			{
				if (brackets.empty())
				{
					return false;
				}
				else
				{
					brackets.pop();
				}
			}
		}

		if (brackets.empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool checkOperations(char* expression)
	{
		for (int i = 0; i < strlen(expression); i++)
		{
			if (isOperation(expression[i]) && !isBrackets(expression[i]))
			{
				if (i == 0)
				{
					return false;
				}
				else if (i == strlen(expression) - 1 && !isBrackets(expression[i]))
				{
					return false;
				}
				else if (isOperation(expression[i - 1]) && !isBrackets(expression[i - 1]))
				{
					return false;
				}
				else if (isOperation(expression[i + 1]) && !isBrackets(expression[i + 1]))
				{
					return false;
				}
			}
		}
		return true;
	}

	void goThrowStr(char* expr, char* polExprs)
	{
		stack<operation> _stack;
		int	polExprIndex = 0;

		for (auto i = 0; i < strlen(expr); i++)
		{
			if (isOperand(expr[i]))
			{
				polExprs[polExprIndex++] = expr[i];
			}
			else if (isOperation(expr[i]))
			{
				if (_stack.empty() ||
					expr[i] == '(' ||
					priorities[expr[i]] > priorities[_stack.top()])
				{
					_stack.push(expr[i]);
				}
				else
				{
					while (
						!_stack.empty() &&
						priorities[expr[i]] <= priorities[_stack.top()])
					{
						if (!isBrackets(_stack.top()))
						{
							polExprs[polExprIndex++] = _stack.top();
						}

						_stack.pop();
					}
					_stack.push(expr[i]);
				}
			}
		}

		polExprs[polExprIndex] = '\0';

		if (!_stack.empty())
		{
			writeTo(polExprs, _stack);
		}

	}

	void writeTo(char* expr, stack<operation> _stack)
	{
		int exprIndex = strlen(expr);

		while (!_stack.empty())
		{
			if (!isBrackets(_stack.top()))
			{
				expr[exprIndex++] = _stack.top();
			}

			_stack.pop();
		}

		expr[exprIndex] = '\0';
	}

	bool isOperand(char symb)
	{
		return
			(symb >= '0' && symb <= '9') ||
			(symb >= 'a' && symb <= 'z') ||
			(symb >= 'A' && symb <= 'Z');
	}

	bool isOperation(char symb)
	{
		return
			symb == '&' ||
			symb == '|' ||
			symb == '~' ||
			isBrackets(symb);
	}

	bool isBrackets(char symb)
	{
		return symb == '(' ||
			symb == ')';
	}
}