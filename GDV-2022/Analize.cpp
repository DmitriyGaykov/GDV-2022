#include "Analize.h"
using namespace fst;

#define BLOCK_SLASH_N

string toString(char* str); // перевод строки char в строку string

bool isStopSymbol(char symbol); // проверка на символ конца строки. Возвращает N, если это не стоп-символ, иначе возвращает переданный символ

void setLexemsAndIds( // разбиение строки на лексемы и идентификаторы
	vector<string>& words, // все слова в тектовом документе
	LT::LexTable& lexems, // таблица лексем
	IT::IdTable& idtable); // таблица идентификаторов

bool isLiteral(string word); // проверяет слово, является ли оно литералом

bool isId(string word); // проверяет слово, является ли оно идентификатором

bool is_id_in_table( // проверяет, есть ли идентификатор в таблице идентификаторов
	IT::IdTable& idtable, // таблица идентификаторов
	string word, // слово, которое нужно проверить
	stack<string> scope,
	string* fullWord = nullptr); // стек областей видимости

bool is_id_in_this_scope( // проверить переменную в данной области видимости
	IT::IdTable& idtable, // таблица идентификаторов
	string word, // слово
	stack<string> scope // область видимости
);

string getNewWord( // получить новое слово учитывая область видимости
	string word, // имя переменной
	stack<string> scope // область видимости
);

void LexAnalize(
	In::IN in,
	LT::LexTable& lextable,
	IT::IdTable& idtable)
{
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
			}
			else
			{
				word += symb;
			}
		}
	}
	
	// for (auto i : words) cout << i << endl;
	
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
	
	string* fullWord = new string;

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
		word = words[i];

		if (
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
			if (words[i - 1] != "\n")
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

		else if (word == "symb")
		{
			lexe.idxTI = -1;
			lexe.lexema = 't';
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

			if (words[i - 1] == "num" || words[i - 1] == "symb")
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

			scope.push("l" + to_string(indexOfScope++));
		}

		else if (word == "}")
		{
			lexe.idxTI = -1;
			lexe.lexema = '}';
			lexe.sn = line;

			LT::Add(lextable, lexe);

			scope.pop(); // хз, пока как указать =)
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
			word == "==")
		{
			lexe.idxTI = -1;
			lexe.lexema = 'V';
			lexe.sn = line;

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

			LT::Add(lextable, lexe);
		}

		else if (word == "~")
		{
			lexe.idxTI = -1;
			lexe.lexema = '~';
			lexe.sn = line;

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

			scope.pop();
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

			LT::Add(lextable, lexe);
		}

		else if (isId(word))
		{
			lexe.idxTI = idtable.size;
			lexe.lexema = 'i';
			lexe.sn = line;

			LT::Add(lextable, lexe);
			
			/////////////////////////////////////////
			
			if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "foo") &&
				words[i + 3] != "?" &&
				!is_id_in_this_scope(idtable, word, scope)
				)
			{
				ide = new IT::Entry();
				
				word = getNewWord(word, scope);

				strcpy_s(ide->id, word.c_str());
				ide->idxfirstLE = lextable.size - 1;
				
				switch (words[i + 2][0])
				{
				case 'n':
					ide->iddatatype = IT::NUM;
					break;
				case 's':
					ide->iddatatype = IT::SYMB;
					break;
				case 'f':
					ide->idtype = IT::F;
					
					if (words[i + 3] == "(")
					{
						int j = i + 3;

						while (words[j] != ")")
						{
							j++;

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
							(words[j + 2] == "num" || words[j + 2] == "symb")
							)
						{
							ide->iddatatype = words[j + 2] == "num" ? IT::NUM : IT::SYMB;
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

				IT::Add(idtable, *ide);

				delete ide;
			}	
			else if (!is_id_in_table(idtable, word, scope))
			{
				throw ERROR_THROW_IN(609, line, 0);
			}
			else if (
				words[i + 1] == "is" &&
				(words[i + 2] == "num" || words[i + 2] == "symb" || words[i + 2] == "foo") &&
				words[i + 3] != "?" &&
				is_id_in_this_scope(idtable, word, scope)
				)
			{
				throw ERROR_THROW_IN(606, line, 0);
			}
			else if (is_id_in_table(idtable, word, scope, fullWord))
			{
				ide = new IT::Entry();
				
				index = IT::IsId(idtable, (char*)fullWord->c_str());

				*ide = IT::GetEntry(idtable, index);

				/*if (
					!ide->hasValue &&
					words[i + 1] != "=" && 
					ide->idtype != IT::F && 
					words[i + 1] != "=>")
				{
					throw ERROR_THROW_IN(602, line, 0);
				}*/

				ide->idxfirstLE = lextable.size - 1;

				IT::Add(idtable, *ide);
		
				delete ide;
			}
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
	const char* stopSymbols = " \t\n;,{}()?><=,&|~\'";

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
	
	return execute(regNum) || regex_match(word.begin(), word.end(), regSymb);
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
	word += ".";
	while (!scope.empty())
	{
		word +=  scope.top();
		scope.pop();
	}

	return word;
}