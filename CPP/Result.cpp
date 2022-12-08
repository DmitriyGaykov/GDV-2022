#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


void dasha(float& a_dasha)
{
	if (a_dasha > 2)
	{
		a_dasha = 2;
		
	}
	if (a_dasha == 23)
	{
		a_dasha = sum((int)23, (int)1);
		
	}
	cout << "A теперь имеет значение: " << a_dasha;
	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	float a_l3_main = -2.22222e+33;
	char char_l3_main = '$';
	float b_l3_main = (int)a_l3_main & (int)2.3 | 1 | ~23;
	return 0;
	
}