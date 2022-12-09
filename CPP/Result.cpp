#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


char f(float& a_f)
{
	if (a_f > 2)	{}
	else	
	{
		a_f = 2;
		
	}
	if (a_f == 23)
	{
		a_f = sum((int)23, (int)1) & 23;
		
	}
	cout << "A теперь имеет значение: " << a_f;
	return (int)a_f & (int)a_f;
	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	float a_l2_main = 2.22222e+30;
	int cb_l2_main = sum((char)'1', (char)23);
	For((int)1, (int)23, 2, [&](int el_For1_l2_main)
	{
		el_For1_l2_main = el_For1_l2_main;
		
	
		return false;
	});
	float b_l2_main = (int)22.2 & 'g';
	f((float&)a_l2_main);
	char char_l2_main = '$';
	b_l2_main = (int)a_l2_main & (int)2.3 | 1 | ~23;
	return 0;
	
}