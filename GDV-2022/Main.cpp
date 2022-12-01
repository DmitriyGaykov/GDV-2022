#include "stdafx.h"
#include "MFST.h"
#include "Gen.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	
	Log::LOG log = Log::INITLOG; // инициализация лога
	Parm::PARM parm; // инициализация параметров
	try
	{
		parm = Parm::getparm(argc, argv); // получение параметров
		log = Log::getlog(parm.log); // получение лога
		auto in = In::getin(parm.in); // получение входных данных

		LT::LexTable lextable = LT::Create(in.size); // создание таблицы лексем
		IT::IdTable idtable = IT::Create(in.size); // создание таблицы идентификаторов
		
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

		Log::WriteLog(log); // запись в лог

		Log::WriteParm(log, parm); // запись процесса проверки параметров

		Log::WriteIn(log, in); // запись процесса обработки входных данных

		Log::Close(log); // закрытие лога

		Out::OUT out(parm.out);
		out.Write((char*)in.text);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка: " << e.id << " : " << e.message << std::endl; // вывод ошибки
		if (e.inext.line != -1)
		{
			std::cout << "Строка: " << e.inext.line << "  Символ: " << e.inext.col << std::endl;
		}
		Log::WriteError(log, e); // запись ошибки в лог
		Log::Close(log); // закрытие лога
	}
	return 0;
}