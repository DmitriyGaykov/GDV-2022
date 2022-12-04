#include "Analize.h"
using namespace fst;

#define BLOCK_SLASH_N
#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'

void LexAnalize(
	In::IN in,
	LT::LexTable& lextable,
	IT::IdTable& idtable)
{
	map<string, string> funcsGDV =
	{
		{"pow",
		" pow is foo(number is num, power is num) is num { result => 1; For(1, power, 1, i => { result = mult(result, number); }); return result;\}"},

		{"symb_to_num",
		"symb_to_num is foo(s is symb) is num{(s == '0') ?Truth{return 0;}(s == '1') ?Truth{return 1;}(s == '2') ?Truth{return 2;}(s == '3') ?Truth{return 3;}(s == '4') ?Truth{return 4;}(s == '5') ?Truth{return 5;}(s == '6') ?Truth{return 6;}(s == '7') ?Truth{return 7;}(s == '8') ?Truth{return 8;}(s == '9') ?Truth{return 9;}return s;}"},
		
		{"abs",
		"abs is foo(number is num) is num{(number < 0) ? Truth { return mult(number, -1);}return number;}"}
	};

	char* text = (char*)in.text;
	vector<string> words;
	string word = "";
	int line = 1;
	char symb;
	bool isComment = false;
	
	for (int i = 0; i < strlen(text); i++)
	{
		symb = text[i];
		
		if (symb == '\n')
		{
			line++;
			
			
			if (isComment)
			{
				words.push_back("\n");
				isComment = false;
				continue;
			}
		}

		if (!isComment)
		{
			if (symb == '*')
			{
				isComment = true;
				continue;
			}

			if (isStopSymbol(symb))
			{
				if (
					word != "" &&
					word != " " &&
					word != "\t"
#if !defined(BLOCK_SLASH_N)
					&& word != "\n"
#endif
					)
				{
					words.push_back(word);
					word = "";
				}

				if (
					symb == '=' &&
					text[i + 1] == '='
					)
				{
					word += symb;
					word += text[i + 1];

					i++;

					words.push_back(word);
					word = "";
				}
				else if (symb == '!')
				{
					word += symb;
					word += text[i + 1];

					i++;

					words.push_back(word);
					
					word = "";
				}
				else if (
					symb == '=' &&
					text[i + 1] == '>'
					)
				{
					word += symb;
					word += text[i + 1];
					
					i++;

					words.push_back(word);
				}
				else if (symb == '\'')
				{
					if (text[i + 2] != '\'')
					{
						throw ERROR_THROW_IN(600, line, 0);
					}

					word += symb;
					word += text[i + 1];
					word += text[i + 2];

					i += 2;

					words.push_back(word);
				}
				else if (
					symb != ' ' &&
					symb != '\t'
#if !defined(BLOCK_SLASH_N)
					&& symb != "\n"
#endif
					)
				{
					word += symb;
					words.push_back(word);
				}

				word = "";

				if (words.size() >= 3 && words[words.size() - 3] == "@import")
				{
					if (funcsGDV.find(words[words.size() - 2]) != funcsGDV.end())
					{
						insertToStr(text, funcsGDV[words[words.size() - 2]], i);
					}
					else
					{
						throw ERROR_THROW_IN(119, line, 0);
					}
					words.pop_back();
					words.pop_back();
					words.pop_back();
					i--;
				}
			}
			else
			{
				word += symb;
			}
		}
	}

	/*for (auto i : words)
	{
		cout << i << endl;
	}*/

	setLexemsAndIds(words, lextable, idtable);
}

void setLexemsAndIds(
	vector<string>& words,
	LT::LexTable& lextable,
	IT::IdTable& idtable)
{
	string word;
	LT::Entry lexe;
	IT::Entry* ide;
	ushort line = 1;
	ushort countScopes = 0;

	ushort countActionScopes = 0;
	bool isAction = false;
	
	string* fullWord = new string;
	stack<bool> isFor;
	isFor.push(false);

	stack<string> scope;
	stack<string> tempScope;
	IT::Entry* tempId;
	
	bool* willBeTrue = nullptr;

	int index;
	int indexOfScope = 0;
	scope.push("");

	// TODO: доделать с идентификаторам в =
	// TODO: доделать с вычислением идентификатора

	for (int i = 0; i < words.size(); i++)
	{
		lexe.view = 0;
		word = words[i];

		if (
			i + 2 < words.size() &&
			words[i + 1] == "=>" &&
			nextIs("{", words, i + 2)
			)
		{
			bool in_table = is_id_in_table(idtable, words[i], scope);
			bool in_scope = is_id_in_this_scope(idtable, words[i], scope);
			ide = new IT::Entry();
			if (isLiteral(words[i]))
			{
				throw ERROR_THROW_IN(612,line, 0);
			}
			else if (isId(words[i]))
			{
				tempId = new IT::Entry();

				word = getNewWord(word, scope);
				
				strcpy_s(ide->id, word.c_str());
				ide->hasValue = true;
				ide->idtype = IT::P;
				ide->idxfirstLE = lextable.size;
				
				int j = i - 2;

				while (j >= 0 && words[j] != ",")
				{
					if (j == 0 && words[j] != ",")
					{
						throw ERROR_THROW_IN(613, line, 0);
					}
					j--;
				}

				j++;

				if (isId(words[j]))
				{
					if ((index = IT::IsId(idtable, (char*)words[j].c_str()) != TI_NULLIDX))
					{
						*tempId = IT::GetEntry(idtable, index);
						ide->iddatatype = tempId->iddatatype;
					}
				}
				else if (isLiteral(words[j]))
				{
					ide->iddatatype = words[j][0] == '\'' ? IT::SYMB 
									  : isFloat(words[j]) ? IT::FLT : 
															IT::NUM;
				}
				else
				{
					throw ERROR_THROW_IN(614, line, 0);
				}

				j = i;

				delete tempId;
			}
			
			i++;
			IT::Add(idtable, *ide);

			lexe.idxTI = idtable.size - 1;
			lexe.lexema = 'i';
			lexe.sn = line;
			lexe.view = 'v';

			LT::Add(lextable, lexe);
			
			delete ide;
			continue;
			
		}
		else if (
			i + 1 < words.size() &&
			words[i + 1] == "=>")
		{
			*fullWord = words[i + 2];
			words[i + 1] = "is";
			
			if (isLiteral(*fullWord))
			{
				if (fullWord->c_str()[0] == '\'')
				{
					words.insert(words.begin() + i + 2, "symb");
				}
				else if (isFloat(*fullWord))
				{
					words.insert(words.begin() + i + 2, "float");
				}
				else
				{
					words.insert(words.begin() + i + 2, "num");
				}
			}
			else if (is_id_in_table(idtable, *fullWord, scope, fullWord))
			{
				index = IT::IsId(idtable, (char*)fullWord->c_str());
				
				tempId = new IT::Entry();
				
				*tempId = IT::GetEntry(idtable, index);
				
				if (tempId->iddatatype == IT::NUM)
				{
					words.insert(words.begin() + i + 2, "num");
				}
				else if (tempId->iddatatype == IT::FLT)
				{
					words.insert(words.begin() + i + 2, "float");
				}
				else
				{
					words.insert(words.begin() + i + 2, "symb");
				}
				
				delete tempId;
			}
			else
			{
				throw ERROR_THROW_IN(608, line, 0);
			}
			
			words.insert(words.begin() + i + 3, "=");
		}

		if (word == "is")
		{
			lexe.idxTI = -1;
			lexe.lexema = 's';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "\n")
		{
			line++;
			if (i - 1 > 0 && words[i - 1] != "\n")
			{
				/*lexe.idxTI = -1;
				lexe.lexema = '\n';
				lexe.sn = line;

				LT::Add(lextable, lexe);*/
			}
		}

		else if (word == "num")
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "break")
		{
			if (!isFor.top())
			{
				throw ERROR_THROW_IN(616, line, 0);
			}
			lexe.idxTI = -1;
			lexe.lexema = 'b';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "For")
		{
			scope.push("For" + to_string(countScopes++));
			lexe.lexema = FOR;
			lexe.sn = line;
			lexe.idxTI = -1;

			int j = i + 1;
			isFor.push(true);
			while (words[j] != "=>")
			{
				if (j == words.size())
				{
					throw ERROR_THROW_IN(615, line, 0);
				}
				j++;
			}
			
			j--; // i/l,el 
			j--; // i/l,
			j--; // i/l
			
			if (isLiteral(words[j]))
			{
				lexe.view = words[j][0] == '\'' ? 's' : isFloat(words[j]) ? 'f' : 'n';
			}
			else if ((index = IT::IsId(idtable, (char*)words[j].c_str()) != TI_NULLIDX))
			{
				tempId = new IT::Entry();
				
				*tempId = IT::GetEntry(idtable, index);
				lexe.view = tempId->iddatatype == IT::NUM ? 'n' : tempId->iddatatype == IT::FLT ? 'f' : 's';
				
				delete tempId;
			}
			
			LT::Add(lextable, lexe);
		}
		
		else if (word == "symb")
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "float")
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "action")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'a';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}
		
		else if (word == "=" )
		{
			lexe.idxTI = -1;
			lexe.lexema = '=';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			//////////////////////////////

			if (words[i - 1] == "num" || words[i - 1] == "symb" || words[i - 1] == "float")
			{
				fullWord = &words[i - 3];
			}
			else if (isId(words[i - 1]))
			{
				fullWord = &words[i - 1];
			}
			else
			{
				throw ERROR_THROW_IN(607, line, 0);
			}

			if (!is_id_in_table(idtable, *fullWord, scope, fullWord))
			{
				throw ERROR_THROW_IN(602, line, 0);
			}
			
			index = IT::IsId(idtable, (char*)fullWord->c_str());
			
			willBeTrue = &idtable.table[index].hasValue;
		}

		else if (word == ";")
		{
			lexe.idxTI = -1;
			lexe.lexema = ';';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if(willBeTrue != nullptr)
			{
				*willBeTrue = true;
				willBeTrue = nullptr;
			}
		}

		else if (word == "console")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'c';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "foo")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'f';
			lexe.sn = line;

			LT::Add(lextable, lexe);
			
			scope.push(words[i - 2]);
			
			ide = new IT::Entry();

			*ide = IT::GetEntry(idtable, IT::GetIndexByLTIndex(idtable, lextable.size - 1 - 1 - 1));

			if (ide->iddatatype == IT::ACTION)
			{
				isAction = true;
				countActionScopes = 0;
			}
			
			delete ide;
		}

		else if (word == "(")
		{
			lexe.idxTI = -1;
			lexe.lexema = '(';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == ")")
		{
			lexe.idxTI = -1;
			lexe.lexema = ')';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "{")
		{
			lexe.idxTI = -1;
			lexe.lexema = '{';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			if (lextable.table[lextable.size - 2].lexema != 't')
			{
				scope.push("l" + to_string(indexOfScope++));
			}

			countScopes++;

			if (isAction)
			{
				countActionScopes++;
			}
		}

		else if (word == "}")
		{
			lexe.idxTI = -1;
			lexe.lexema = '}';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			scope.pop(); // хз, пока как указать =)

			countScopes--;

			if (
				i + 1 < words.size() &&
				words[i + 1] == ")"
				)
			{
				isFor.pop();
				scope.pop();
			}

			if (isAction)
			{
				countActionScopes--;
				
				if (countActionScopes == 0)
				{
					scope.pop();
					isAction = false;
				}
			}
		}

		else if (word == ",")
		{
			lexe.idxTI = -1;
			lexe.lexema = ',';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (
			word == ">" ||
			word == "<" ||
			word == "==" ||
			word == "!=")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'V';
			lexe.sn = line;
			lexe.view = word.c_str()[0];

			LT::Add(lextable, lexe);
		}

		else if (
			word == "&" ||
			word == "|"
			)
		{
			lexe.idxTI = -1;
			lexe.lexema = 'v';
			lexe.sn = line;
			lexe.view = word[0];

			LT::Add(lextable, lexe);
		}

		else if (
			word == "-"
		)
		{
			if (
				i + 1 < words.size() &&
				isLiteral(words[i + 1]) &&
				words[i + 1][0] != '\'')
			{
				words[i + 1] = "-" + words[i + 1];
			}
			else
			{
				throw ERROR_THROW_IN(120, line, 0);
			}
		}

		else if (word == "~")
		{
			lexe.idxTI = -1;
			lexe.lexema = '~';
			lexe.sn = line;
			lexe.view = word[0];

			LT::Add(lextable, lexe);
		}

		else if (word == "main")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'm';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			scope.push("main");
		}

		else if (word == "return")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'r';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "?")
		{
			lexe.idxTI = -1;
			lexe.lexema = '?';
			lexe.sn = 0;

			LT::Add(lextable, lexe);
		}

		else if (word == "Truth")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'T';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (word == "Lie")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'L';
			lexe.sn = line;

			LT::Add(lextable, lexe);
		}

		else if (isLiteral(word))
		{
			lexe.idxTI = -1;
			lexe.lexema = 'l';
			lexe.sn = line;
			lexe.idxTI = idtable.size;

			LT::Add(lextable, lexe);

			ide = new IT::Entry();

			ide->idxfirstLE = lextable.size - 1;
			ide->idtype = IT::L;
			ide->iddatatype = word[0] == '\'' ? IT::SYMB 
							  : isFloat(word) ? IT::FLT : IT::NUM;
			ide->hasValue = true;
			
			if (ide->iddatatype == IT::NUM)
			{
				ide->value.vint = stoi(word);
			}
			else if (ide->iddatatype == IT::FLT)
			{
				for (auto& i : word)
				{
					if (i == '.')
					{
						i = ',';
					}
				}
				ide->value.vflt = stof(word);
			}
			else
			{
				ide->value.vsymb = word[1];
			}

			strcpy_s(ide->id, "literal");

			IT::Add(idtable, *ide);
			
			delete ide;
		}

		else if (isId(word))
		{
			lexe.idxTI = idtable.size;
			lexe.lexema = 'i';
			lexe.sn = line;
			lexe.idxTI = idtable.size;
			

			bool in_scope = is_id_in_this_scope(idtable, word, scope);
			bool in_table = is_id_in_table(idtable, word, scope);
			
			/////////////////////////////////////////
			
			/*if (
				nextIs("is", words, i + 1) &&
				(nextIs("num", words, i + 2) || nextIs("symb", words, i + 2) || nextIs("float", words, i + 2) || nextIs("foo", words, i + 2) ||
					(nextIs("ref", words, i + 2) && (nextIs("num", words, i + 3) || nextIs("symb", words, i + 3) || nextIs("symb", words, i + 3)))) &&
				words[i + 4] != "?" &&
				!in_scope
				)*/
			
			if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float" || words[i + 2] == "foo" || 
				(words[i + 2] == "ref" && (words[i + 3] == "num" || words[i + 3] == "symb" || words[i + 3] == "float"))) &&
				words[i + 4] != "?" &&
				!in_scope
				)
			{
				ide = new IT::Entry();
				
				word = getNewWord(word, scope);

				strcpy_s(ide->id, word.c_str());
				ide->idxfirstLE = lextable.size;

				if (words[i + 2] == "ref")
				{
					ide->isRef = true;
					words.erase(words.begin() + i + 2);
				}
				
				
				switch (words[i + 2][0])
				{
				case 'n':
					ide->iddatatype = IT::NUM;
					break;
				case 's':
					ide->iddatatype = IT::SYMB;
					break;
				case 'f':
					if (words[i + 2][1] != 'l')
					{
						
					}
					else
					{
						ide->iddatatype = IT::FLT;
						break;
					}
				case '.':
					ide->idtype = IT::F;
					ide->countParams = 0;
					
					if (words[i + 3] == "(")
					{
						int j = i + 3;

						while (words[j] != ")")
						{
							j++;
							
							if (words[j] == "is")
							{
								ide->countParams++;
								ushort itmp = 1;

								if (j + 1 < words.size() && words[j + 1] == "ref")
								{
									itmp++;
								}

								if (
									j + itmp < words.size() &&
									(words[j + itmp] == "num" || words[j + itmp] == "symb" || words[j + itmp] == "float"))
								{
									if (words[j + itmp] == "num")
									{
										ide->params.push_back(itmp == 1 ? "int" : "int&");
									}
									else if (words[j + itmp] == "symb")
									{
										ide->params.push_back(itmp == 1 ? "char" : "char&");
									}
									else if (words[j + itmp] == "float")
									{
										ide->params.push_back(itmp == 1 ? "float" : "float&");
									}
								}
							}
							
							if (
								j + 1 == words.size() &&
								words[j] != ")"
								)
							{
								throw ERROR_THROW_IN(604, line, i);
							}
						}

						if (
							words[j + 1] == "is" &&
							(words[j + 2] == "num" || words[j + 2] == "symb" || words[j + 2] == "action" || words[j + 2] == "float")
							)
						{
							if (words[j + 2] == "action")
							{
								ide->iddatatype = IT::ACTION;
							}
							else
							{
								ide->iddatatype = words[j + 2] == "num" ? IT::NUM :
												  isFloat(words[j + 2]) ? IT::FLT : IT::SYMB;
							}

						}
						else
						{
							throw ERROR_THROW_IN(605, line, 0);
						}
					}
					else
					{
						throw ERROR_THROW_IN(603, line, 0);
					}
					
					break;
				}
				
				if (
					ide->idtype != IT::F &&
					i > 0 &&
					(words[i - 1] == "," || words[i - 1] == "(") &&
					words[i + 1] == "is")
				{
					ide->idtype = IT::P;
				}
				else if(ide->idtype != IT::F)
				{
					ide->idtype = IT::V;
				}
				
				if (ide->idtype == IT::P)
				{
					ide->hasValue = true;
				}

				if (ide->isRef && ide->idtype != IT::P)
				{
					throw ERROR_THROW_IN(618, line, 0);
				}
				
				IT::Add(idtable, *ide);

				delete ide;
			}	
			else if (
				words[i - 1] == "(" &&
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float") &&
				words[i + 3] == ")" &&
				words[i + 4] == "?" &&
				is_id_in_table(idtable, word, scope, fullWord)
				)
			{
				ide = new IT::Entry;

				index = IT::IsId(idtable, (char*)fullWord->c_str());
				
				if (index != TI_NULLIDX)
				{
					*ide = idtable.table[index];
					
					if (
						(ide->iddatatype == IT::NUM && words[i + 2] == "num") ||
						(ide->iddatatype == IT::SYMB && words[i + 2] == "symb") ||
						(ide->iddatatype == IT::FLT && words[i + 2] == "float")
						)
					{
						words[i] = "1";
						words[i + 1] = ">";
						words[i + 2] = "0";
					}
					else
					{
						words[i] = "0";
						words[i + 1] = ">";
						words[i + 2] = "1";
					}

					i--;
					continue;
				}
				
				delete ide;
			}
			else if (!in_table)
			{
				throw ERROR_THROW_IN(609, line, 0);
			}
			else if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "float" || words[i + 2] == "foo") &&
				words[i + 3] != ")" &&
				in_scope
				)
			{
				throw ERROR_THROW_IN(606, line, 0);
			}
			else if (is_id_in_table(idtable, word, scope, fullWord))
			{
				ide = new IT::Entry();
				
				index = IT::IsId(idtable, (char*)fullWord->c_str());

				*ide = IT::GetEntry(idtable, index);

				ide->idxfirstLE = lextable.size;

				IT::Add(idtable, *ide);

				if (
					ide->idtype == IT::F &&
					i + 1 < words.size() &&
					words[i + 1] != "("
					)
				{
					throw ERROR_THROW_IN(617, line, 0);
				}
		
				delete ide;
			}
			LT::Add(lextable, lexe);
		}

		else
		{
			throw ERROR_THROW_IN(601, line, 0);
		}
	}
}


string toString(char* str)
{
	string s = "";

	for (int i = 0; str[i] != '\0'; i++)
	{
		s += str[i];
	}

	return s;
}

bool isStopSymbol(char symbol)
{
	const char* stopSymbols = " \t\n;,{}()?><=,&|~!\'";

	for (short i = 0; i < strlen(stopSymbols); i++)
	{
		if (symbol == stopSymbols[i])
		{
			return true;
		}
	}

	return false;
}

bool isLiteral(string word)
{
	FST regNum(
		(char*)word.c_str(),
		3,
		NODE(22, RELATION('+', 1), RELATION('-', 1), RELATION('0', 1), RELATION('0', 2), RELATION('1', 1), RELATION('1', 2), RELATION('2', 1), RELATION('2', 2), RELATION('3', 1), RELATION('3', 2), RELATION('4', 1), RELATION('4', 2), RELATION('5', 1), RELATION('5', 2), RELATION('6', 1), RELATION('6', 2), RELATION('7', 1), RELATION('7', 2), RELATION('8', 1), RELATION('8', 2), RELATION('9', 1), RELATION('9', 2)),
		NODE(20, RELATION('0', 1), RELATION('0', 2), RELATION('1', 1), RELATION('1', 2), RELATION('2', 1), RELATION('2', 2), RELATION('3', 1), RELATION('3', 2), RELATION('4', 1), RELATION('4', 2), RELATION('5', 1), RELATION('5', 2), RELATION('6', 1), RELATION('6', 2), RELATION('7', 1), RELATION('7', 2), RELATION('8', 1), RELATION('8', 2), RELATION('9', 1), RELATION('9', 2)),
		NODE()
		);

	regex regSymb("'[^☺]'");
	
	return execute(regNum) || regex_match(word.begin(), word.end(), regSymb) || isFloat(word);
}

bool isFloat(string word)
{
	regex regFloat("[0-9]{0,10}\\.[0-9]{1,23}");
	return regex_match(word.begin(), word.end(), regFloat);
}


bool isId(string word)
{
	regex reg("[A-Za-z_][A-Za-z0-9_]*");

	return regex_match(word.begin(), word.end(), reg);
}

bool is_id_in_table(
	IT::IdTable& idtable,
	string word,
	stack<string> scope,
	string* fullWord)
{
	string temp = word;
	int index;

	index = IT::IsId(idtable, (char*)word.c_str());

	if (index != TI_NULLIDX)
	{
		if (fullWord != nullptr)
		{
			*fullWord = word;
		}
		return true;
	}
	
	while (true)
	{
		temp = word;
		
		temp = getNewWord(temp, scope);
		
		index = IT::IsId(idtable, (char*)temp.c_str());

		if (index == TI_NULLIDX)
		{
			if (scope.empty())
			{
				return false;
			}
			else
			{
				scope.pop();
			}
		}
		else
		{
			if (fullWord != nullptr)
			{
				*fullWord = temp;
			}
			
			return true;
		}
	}
}

bool is_id_in_this_scope(
	IT::IdTable& idtable, 
	string word,
	stack<string> scope 
)
{
	word = getNewWord(word, scope);

	auto index = IT::IsId(idtable, (char*)word.c_str());

	if (index == TI_NULLIDX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

string getNewWord(string word, stack<string> scope)
{
	while (!scope.empty() && scope.top() != "")
	{
		word += "." + scope.top();
		scope.pop();
	}

	return word;
}

void insertToStr(
	char* str,
	string insertedStr,
	int index)
{
	int len = strlen(str);
	
	string after = "";
	string before = "";

	for (int i = 0; i < len; i++)
	{
		if (i < index)
		{
			before += str[i];
		}
		else
		{
			after += str[i];
		}
	}

	string res = before + insertedStr + after;
	strcpy_s(str, res.c_str());
}

bool nextIs(string word, vector<string>& words, int i)
{
	for (; i < words.size(); i++)
	{
		if (words[i] == word)
		{
			return true;
		}
		else if (words[i] == "\n")
		{
			continue;
		}
		else
		{
			return false;
		}
	}

	return false;
}