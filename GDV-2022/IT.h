#pragma once
#include <vector>
#include <string>

#define ID_MAXSIZE 450 // ������������ ���������� �������� � ��������������
#define TI_MAXSIZE 4096 // ������������ ���������� ������� � ������� ���������������
#define TI_INT_DEFAULT 0x00000000 // �������� �� ��������� ��� ���� integer
#define TT_STR_DEFAULT 0x00 // �������� �� ��������� ��� ���� string
#define TI_NULLIDX 0xffffffff // ��� �������� � ������� ���������������
#define TI_STR_MAXSIZE 255 

namespace IT
{
	enum IDDATATYPE // ���� ������ ���������������
	{
		NUM = 1,
		SYMB = 2,
		FLT = 3,
		ACTION,
	};

	enum IDTYPE // ���� ���������������
	{
		V = 1, // ����������
		F = 2, // �������
		P = 3, // ��������
		L = 4, // �������
	};

	struct Entry // ������ � ������� ���������������
	{
		int idxfirstLE; // ������ ������ ������ � ������� ������
		short countParams; // ���-�� ����������, ���� ��� ������� 
		char id[ID_MAXSIZE]; // �������������
		IDDATATYPE iddatatype; // ��� ������ ��������������
		IDTYPE idtype; // ��� ��������������
		bool hasValue;
		bool isRef;
		bool isFromStatic;

		std::vector<std::string> params;

		bool needToInt;
		
		union
		{
			int vint; // �������� ��� ���� num

			char vsymb; // �������� ���� symb

			float vflt; // �������� ���� float

		
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

	struct IdTable // ��������� ������� ���������������
	{
		int maxsize; // ������������ ���������� ������� � ������� ��������������� < TI_MAXSIZE
		int size; // ������� ������ ������� ��������������� < maxsize
		Entry* table; // ������ ����� ������� ���������������
	};

	IdTable Create(int size); // �������� ������� ���������������

	void Add( // ���������� ������ � ������� ���������������
		IdTable& idtable, // ������� ���������������
		Entry entry // ������ ������� ���������������
	);

	Entry GetEntry( // ��������� ������ �� ������� ���������������
		IdTable& idtable, // ������� ���������������
		int n // ����� ������ � ������� ���������������
	);

	int IsId( // ����� �������������� � ������� ��������������� ������� ������ ��� TI_NULLIDX
		IdTable& idtable, // ������� ���������������
		char id[ID_MAXSIZE] // �������������
	);

	void Delete(IdTable& idtable); // �������� ������� ���������������

	int GetIndexByLTIndex( // ����� ������ �������� � ������� ��������������� �� ������� � ������� ������
		IdTable& idtable, // ������� ���������������
		int index); // ������ � ������� ������
}