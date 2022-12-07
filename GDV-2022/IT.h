#pragma once
#include <vector>
#include <string>

#define ID_MAXSIZE 450 // максимальное количество символов в идентификаторе
#define TI_MAXSIZE 4096 // максимальное количество записей в таблице идентификаторов
#define TI_INT_DEFAULT 0x00000000 // значение по умолчанию дл€ типа integer
#define TT_STR_DEFAULT 0x00 // значение по умолчанию дл€ типа string
#define TI_NULLIDX 0xffffffff // нет элемента в таблицы идентификаторов
#define TI_STR_MAXSIZE 255 

namespace IT
{
	enum IDDATATYPE // типы данных идентификаторов
	{
		NUM = 1,
		SYMB = 2,
		FLT = 3,
		ACTION,
	};

	enum IDTYPE // типы идентификаторов
	{
		V = 1, // переменна€
		F = 2, // функци€
		P = 3, // параметр
		L = 4, // литерал
	};

	struct Entry // запись в таблице идентификаторов
	{
		int idxfirstLE; // индекс первой строки в таблице лексем
		short countParams; // кол-во параметров, если это функци€ 
		char id[ID_MAXSIZE]; // идентификатор
		IDDATATYPE iddatatype; // тип данных идентификатора
		IDTYPE idtype; // тип идентификатора
		bool hasValue;
		bool isRef;
		bool isFromStatic;

		std::vector<std::string> params;

		bool needToInt;
		
		union
		{
			int vint; // значение дл€ типа num

			char vsymb; // значение типа symb

			float vflt; // значение типа float

		
		} value;

		Entry()
		{
			hasValue = false;
			countParams = -1;
			needToInt = false;
			isRef = false;
			isFromStatic = false;
		}
	};

	struct IdTable // экземпл€р таблицы идентификаторов
	{
		int maxsize; // максимальное количество записей в таблице идентификаторов < TI_MAXSIZE
		int size; // текущий размер таблицы идентификаторов < maxsize
		Entry* table; // массив строк таблицы идентификаторов
	};

	IdTable Create(int size); // создание таблицы идентификаторов

	void Add( // добавление записи в таблицу идентификаторов
		IdTable& idtable, // таблица идентификаторов
		Entry entry // строка таблицы идентификаторов
	);

	Entry GetEntry( // получение записи из таблицы идентификаторов
		IdTable& idtable, // таблица идентификаторов
		int n // номер записи в таблице идентификаторов
	);

	int IsId( // поиск идентификатора в таблице идентификаторов вернуть индекс или TI_NULLIDX
		IdTable& idtable, // таблица идентификаторов
		char id[ID_MAXSIZE] // идентификатор
	);

	void Delete(IdTable& idtable); // удаление таблицы идентификаторов

	int GetIndexByLTIndex( // найти индекс элемента в таблице идентификаторов по индексу в таблице лексем
		IdTable& idtable, // таблица идентификаторов
		int index); // индекс в таблице лексем
}