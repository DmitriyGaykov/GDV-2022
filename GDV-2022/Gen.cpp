#include "Gen.h"

namespace GEN
{
	bool Generate(
		LT::LexTable& lextable, 
		IT::IdTable& idtable 
	)
	{
		const string PATH = "..\\CPP\\Result.cpp";
		ofstream fout(PATH);
		IT::Entry* edi;
		IT::Entry* tedi;
		IT::Entry* t2edi;
		int size = 0;
		const int MAX_SIZE = 100;
		char* expr = new char[MAX_SIZE];
		stack<string> tabs;
		auto t = tabs;
		bool isEndOfIf = false;
		
		int countScopes = 0;
		int countParams = 0;
		bool needAdd = false;

		int countScopesForConsole = 0;
		bool isConsole = false;
		
		tabs.push("");
		
		fout << HEADER;
		
		for (int i = 0; i < lextable.size; i++)
		{
			switch (lextable.table[i].lexema)
			{
			case 'i':
			{
				if (
					i + 2 < lextable.size &&
					lextable.table[i + 1].lexema == 's' &&
					(lextable.table[i + 2].lexema == 't' || lextable.table[i + 2].lexema == 'f'))
				{
					edi = new IT::Entry();

					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);
					if (tabs.size() > 1 && lextable.table[i + 2].lexema == 'f')
					{
						fout << "auto " << edi->id << " = [&]";
					}
					else if (edi->iddatatype == IT::NUM)
					{
						fout << "int " << edi->id;
					}
					else
					{
						fout << "char " << edi->id;
					}
					
					i += 2;

					delete edi;
				}

				else if (
					i + 1 < lextable.size &&
					lextable.table[i + 1].lexema == '{')
				{
					edi = new IT::Entry();

					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);

					fout << "[&](" << (edi->iddatatype == IT::NUM ? "int " : "char ") << edi->id << ")";

					delete edi;
				}

				else
				{
					edi = new IT::Entry();
				
					*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);
					changeDoth(edi->id);

					countParams = 0;
					int j = i + 1;
					int countScopes = 0;

					if (edi->idtype == IT::F)
					{
						if (lextable.table[j].lexema == '(')
						{
							do
							{
								if (
									lextable.table[j].lexema != ')' && 
									lextable.table[j].lexema != '(' && 
									lextable.table[j].lexema != ',' && 
									countScopes == 1)
								{
									countParams++;
								}
								if (lextable.table[j].lexema == '(')
								{
									countScopes++;
								}
								if (lextable.table[j].lexema == ')')
								{
									countScopes--;
								}
								j++;
							} while (countScopes != 0);

							if (countParams != edi->countParams)
							{
								throw ERROR_THROW(610);
							}
						}
					}
					
					fout << edi->id;
					
					delete edi;
				}
				break; 
			}

			case 'l':
			{
				edi = new IT::Entry();

				*edi = IT::GetEntry(idtable, lextable.table[i].idxTI);

				if (edi->iddatatype == IT::NUM)
				{
					fout << edi->value.vint;
				}
				else
				{
					fout << '\'' << edi->value.vsymb << '\'';
				}
				
				delete edi;
				break;
			}

			case 'r':
			{
				fout << "return ";
				break;
			}
			
			case FOR:
			{
				fout << "For";
				break;
			}

			case '=':
			{
				size = 0;

				fout << " = ";
				
				
				break;
			}
			
			case 'm':
			{
				fout << STARTMAIN;

				tabs.push("\t");
				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}

				i++;
				break;
			}

			case 'v':
			{
				fout << " " << lextable.table[i].view << " ";
				break;
			}

			case 'V':
			{
				if (lextable.table[i].view != '=')
				{
					fout << " " << lextable.table[i].view << " ";
				}
				else
				{
					fout << " == ";
				}
				break;
			}

			case '{': case '}':
			{	
				
				fout << "\n";
				
				t = tabs;
				
				if (lextable.table[i].lexema == '}')
				{
					t.pop();
				}

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}

				fout << lextable.table[i].lexema << (i + 1 < lextable.size && (lextable.table[i + 1].lexema != ';' && lextable.table[i + 1].lexema != '}' && lextable.table[i + 1].lexema != '{')  ? "\n" : "");

				if (lextable.table[i].lexema == '{')
				{
					tabs.push("\t");
					t = tabs;
					
					while (!t.empty())
					{
						fout << t.top();
						t.pop();
					}
				}
				else
				{
					tabs.pop();
					
					t = tabs;

					while (!t.empty())
					{
						fout << t.top();
						t.pop();
					}
				}
				
				break;
			}

			case '(':
			{
				if (isConsole && lextable.table[i - 1].lexema != 'i')
				{
					countScopesForConsole++;
				}
				else
				{
					if (isConsole) countScopesForConsole++;
					if (needAdd)
					{
						countScopes++;
					}
					size = 0;
					isEndOfIf = false;
					if (lextable.table[i - 1].lexema != 'i')
					{
						for (int j = i; lextable.table[j].lexema != '?'; j++)
						{
							if (
								lextable.table[j].lexema == ';' ||
								lextable.table[j].lexema == '{' ||
								lextable.table[j].lexema == '}')
							{
								isEndOfIf = true;
								break;
							}
							size++;
						}
					}
					else
					{
						fout << "(";
						break;
					}

					if (isEndOfIf)
					{
						fout << "(";
						break;
					}

					fout << "if (";

					if (lextable.table[i + size + 1].lexema != 'T')
					{
						needAdd = true;
						countScopes++;
					}

				}
				break;
			}

			case 'T':
			{
				break;
			}

			case 'L':
			{
				fout << "else";
				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}
				break;
			}
			
			case ')':
			{
				if (isConsole && lextable.table[i + 1].lexema != ',' && lextable.table[i + 1].lexema != ')')
				{
					countScopesForConsole--;
					
					if (countScopesForConsole == 0)
					{
						isConsole = false;
					}
				}
				else
				{
					if (isConsole) countScopesForConsole--;
					
					fout << ")";
					if (needAdd)
					{
						countScopes--;

						if (countScopes == 0)
						{
							fout << "\t{}\n";
							t = tabs;

							while (!t.empty())
							{
								fout << t.top();
								t.pop();
							}
							needAdd = false;
						}


					}

					if (lextable.table[i - 1].lexema == 't')
					{
						i += 2;
					}
				}
				
				break;
			}

			case ';':
			{
				fout << ";" << (i + 1 < lextable.size && lextable.table[i].lexema != '}' ? "\n" : "");

				t = tabs;

				while (!t.empty())
				{
					fout << t.top();
					t.pop();
				}
				
				break;
			}
			
			case ',':
			{
				fout << (isConsole && countScopesForConsole == 1 ? " << " : ", ");
				break;
			}
			
			case 'c':
			{
				if (lextable.table[i + 1].lexema == '(' && lextable.table[i + 2].lexema == ')')
				{
					fout << "cout << '\\n'";
					i += 2;
				}
				else
				{
					fout << "cout << ";
					countScopesForConsole = 0;
					isConsole = true;
				}
				break;
			}
			}
		}
		
		delete[] expr;
		fout.close();

		return true;
	}

	void changeDoth(char* str)
	{
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] == '.')
			{
				str[i] = '_';
			}
		}
	}
}