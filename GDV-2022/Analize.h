#pragma once
#include "stdafx.h"

#define PARAMETR_OF_LYMBDA 'o'
#define FOR 'p'

void LexAnalize( // ������� ��� ���������� ������������ �������
	In::IN in, // ������� ����
	LT::LexTable& lextable, // ������� ������
	IT::IdTable& idtable); // ������� ���������������