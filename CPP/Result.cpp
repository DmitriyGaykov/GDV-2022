#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


void process(int& a_process)
{
	a_process = sum((int)a_process, (int)a_process);
	
}
char func(int& a_func)
{
	a_func = 23;
	return a_func;
	
}
int a = 2111;
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	For((int)1, (int)func((int&)a), 2, [&](int el_For1_l1_main)
	{
		cout << el_For1_l1_main << ' ';
		return false;
;
		
	
		return false;
	});
	return 0;
	
}