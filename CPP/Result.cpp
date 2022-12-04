#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


void process(int& a_process)
{
	a_process = sum((int)a_process, (int)a_process);
	
}
char func(int a_func)
{
	return a_func;
	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int a_l1_main = 2;
	char res_l1_main = func((int)sum((int)1, (int)mult((float)2.2f, (float)func((int)a_l1_main))));
	return 0;
	
}