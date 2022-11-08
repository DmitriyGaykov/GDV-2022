#include "stdafx.h"

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	Log::LOG log = Log::INITLOG; // инициализация лога
	Parm::PARM parm; // инициализация параметров
	try
	{
		parm = Parm::getparm(argc, argv); // получение параметров
		log = Log::getlog(parm.log); // получение лога
		In::IN in = In::getin(parm.in); // получение входных данных

		LT::LexTable lextable = LT::Create(in.size); // создание таблицы лексем
		IT::IdTable idtable = IT::Create(in.size); // создание таблицы идентификаторов
		
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


		//	Log::WriteLine(log, (wchar_t*)L"Текст:", (wchar_t*)L" без ошибок\n", (wchar_t*)L""); // запись в лог об ошибке
		//	Log::WriteLine(log, (char*)"Текст: ", (char*)" без ошибок\n", (char*)""); // запись в лог об ошибке

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