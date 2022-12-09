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
			else if (ite->iddatatype == IT::ACTION)
			{
				throw ERROR_THROW_IN(628, lextable.table[i - 1].sn, 0);
			}
			else  if ((ite + 1)->iddatatype == IT::ACTION)
			{
				throw ERROR_THROW_IN(628, lextable.table[i].sn, 0);
			}
		}
		else if (lextable.table[i].lexema == '~')
		{
			ite = &idtable.table[IT::GetIndexByLTIndex(idtable, i + 1)];

			if (ite->iddatatype == IT::FLT)
			{
				ite->needToInt = true;
			}
			else if (ite->iddatatype == IT::ACTION)
			{
				throw ERROR_THROW_IN(628, lextable.table[i + 1].sn, 0);
			}
		}
		else if (lextable.table[i].lexema == '=')
		{
			int j = i + 1;
			
			while (lextable.table[j].lexema == '(')
			{
				j++;
			}

			ite = &idtable.table[IT::GetIndexByLTIndex(idtable, j)];

			if (ite->idtype == IT::F && ite->iddatatype == IT::ACTION)
			{
				throw ERROR_THROW_IN(628, lextable.table[j].sn, 0);
			}
		}
	}
}