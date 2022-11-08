#pragma once
#include "stdafx.h"

void LexAnalize( // функция для выполнения лексического анализа
	In::IN in, // входной файл
	LT::LexTable& lextable, // таблица лексем
	IT::IdTable& idtable); // таблица идентификаторов