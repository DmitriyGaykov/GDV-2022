#include "stdafx.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	Log::LOG log = Log::INITLOG; // ������������� ����
	Parm::PARM parm; // ������������� ����������
	try
	{
		parm = Parm::getparm(argc, argv); // ��������� ����������
		log = Log::getlog(parm.log); // ��������� ����
		In::IN in = In::getin(parm.in); // ��������� ������� ������

		LT::LexTable lextable = LT::Create(in.size); // �������� ������� ������
		IT::IdTable idtable = IT::Create(in.size); // �������� ������� ���������������
		
		LexAnalize(in, lextable, idtable);

		for (int i = 0; i < lextable.size; i++)
		{
			cout << lextable.table[i].lexema;
		}

		cout << endl << endl << endl;

		for (int i = 0; i < idtable.size; i++)
		{
			cout << idtable.table[i].id << endl;
		}


		//	Log::WriteLine(log, (wchar_t*)L"�����:", (wchar_t*)L" ��� ������\n", (wchar_t*)L""); // ������ � ��� �� ������
		//	Log::WriteLine(log, (char*)"�����: ", (char*)" ��� ������\n", (char*)""); // ������ � ��� �� ������

		Log::WriteLog(log); // ������ � ���

		Log::WriteParm(log, parm); // ������ �������� �������� ����������

		Log::WriteIn(log, in); // ������ �������� ��������� ������� ������

		Log::Close(log); // �������� ����

		Out::OUT out(parm.out);
		out.Write((char*)in.text);
	}
	catch (Error::ERROR e)
	{
		std::cout << "������: " << e.id << " : " << e.message << std::endl; // ����� ������
		if (e.inext.line != -1)
		{
			std::cout << "������: " << e.inext.line << "  ������: " << e.inext.col << std::endl;
		}
		Log::WriteError(log, e); // ������ ������ � ���
		Log::Close(log); // �������� ����
	}
	return 0;
}