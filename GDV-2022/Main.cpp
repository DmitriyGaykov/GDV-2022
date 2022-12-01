#include "stdafx.h"
#include "MFST.h"
#include "Gen.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	
	Log::LOG log = Log::INITLOG; // ������������� ����
	Parm::PARM parm; // ������������� ����������
	try
	{
		parm = Parm::getparm(argc, argv); // ��������� ����������
		log = Log::getlog(parm.log); // ��������� ����
		auto in = In::getin(parm.in); // ��������� ������� ������

		LT::LexTable lextable = LT::Create(in.size); // �������� ������� ������
		IT::IdTable idtable = IT::Create(in.size); // �������� ������� ���������������
		
		LexAnalize(in, lextable, idtable);
		
		cout << endl;

		for (int i = 0; i < lextable.size; i++)
		{
			cout << lextable.table[i].lexema;
		}

		cout << endl;
		
		ofstream fout("trace.txt");
		
		MFST_TRACE_START

			fout.close();
			MFST::Mfst mfst(lextable, GRB::getGreibach());
		bool resultSint = mfst.start();
		mfst.savededucation();
		
		fout.open("trace.txt", ios::app);
		mfst.printrules(fout);
		fout.close();
		
		if (!resultSint)
		{
			throw ERROR_THROW(121);
		}
		
		cout << endl << endl;
		
		GEN::Generate(lextable, idtable);

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