#include "Semantic.h"

void checkSemantic(
	LT::LexTable& lextable, // таблица лексем
	IT::IdTable& idtable // таблица идентификаторов
)
{
	IT::Entry* ite;
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == 'v')
		{
			ite = &idtable.table[IT::GetIndexByLTIndex(idtable, i - 1)];
			
			if (ite->iddatatype == IT::FLT)
			{
				ite->needToInt = (ite + 1)->needToInt = true;
			}
		}
		else if (lextable.table[i].lexema == '~')
		{
			ite = &idtable.table[IT::GetIndexByLTIndex(idtable, i + 1)];

			if (ite->iddatatype == IT::FLT)
			{
				ite->needToInt = true;
			}
		}
	}
}