#include "IT.h"
#include "stdafx.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}

		IdTable idtable;

		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];


		Entry* pEn = new Entry();

		pEn->hasValue = true;
		strcpy_s(pEn->id, "sum");
		pEn->iddatatype = IT::FLT;
		pEn->idtype = IT::F;
		pEn->countParams = 2;
		pEn->isFromStatic = true;
		
		Add(idtable, *pEn);
		
		strcpy_s(pEn->id, "minus");
		Add(idtable, *pEn);

		strcpy_s(pEn->id, "mult");
		Add(idtable, *pEn);
		
		strcpy_s(pEn->id, "division");
		Add(idtable, *pEn);
		
		delete pEn;
			
		return idtable;
	}

	void Add(
		IdTable& idtable,
		Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
		{
			throw ERROR_THROW(117);
		}

		idtable.table[idtable.size] = entry;
		idtable.size++;
	}

	Entry GetEntry(
		IdTable& idtable,
		int n)
	{
		if (
			n >= idtable.maxsize ||
			n < 0)
		{
			throw ERROR_THROW(118);
		}

		return idtable.table[n];
	}

	int IsId(
		IdTable& idtable,
		char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		idtable.size = 0;
		idtable.maxsize = 0;
	}

	int GetIndexByLTIndex(IdTable& idtable, int index)
	{
		int ind = -1;

		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idxfirstLE == index)
			{
				ind = i;
				break;
			}
		}

		return ind;
	}

	void Output(IdTable& idtable)
	{
		fstream fout("IT.txt", fstream::out);
		Entry e;
		fout << "Таблица идентификаторов";
		fout << endl;
		
		for (ushort i = 0; i < idtable.size; i++)
		{
			e = idtable.table[i];
			fout << "Идентификатор " << e.id << "\n";
			
			fout << "Тип данных: ";
			if (e.isRef)
			{
				fout << "ref ";
			}
			switch (e.iddatatype)
			{
			case IT::NUM:
				fout << "num";
				break;
			case IT::STR:
				fout << "str";
				break;
			case IT::SYMB:
				fout << "symb";
				break;
			case IT::ACTION:
				fout << "action";
				break;
			case IT::FLT:
				fout << "float";
				break;
			}
			fout << "\n";
			
			fout << "Тип идентификатора: ";
			switch (e.idtype)
			{
			case IT::F:
				fout << "Функция";
				break;
			case IT::P:
				fout << "Параметр";
				break;
			case IT::V:
				fout << "Переменная";
				break;
			case IT::L:
				fout << "Литерал";
				break;
			}
			
			if(!e.isFromStatic) fout << "\nИндекс в таблице лексем: " << e.idxfirstLE;
			
			if (e.isFromStatic)
			{
				fout << "\nИз статической библиотеки!";
			}
			if (e.idtype == IT::L)
			{
				fout << "\nЗначение: ";
				
				switch (e.iddatatype)
				{
				case IT::NUM:
					fout << e.value.vint;
					break;
				case IT::STR:
					fout << e.value.vstr;
					break;
				case IT::SYMB:
					fout << e.value.vsymb;
					break;
				}
			}
			fout << endl << endl;
		}
		
		fout << endl;
		fout.close();
	}
}