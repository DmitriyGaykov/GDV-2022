#pragma once
#include "stdafx.h"

#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'

void LexAnalize( // функция для выполнения лексического анализа
	In::IN in, // входной файл
	LT::LexTable& lextable, // таблица лексем
	IT::IdTable& idtable); // таблица идентификаторов