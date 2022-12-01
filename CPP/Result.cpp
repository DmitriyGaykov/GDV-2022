#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto Fact__l0_main_ = [&](int a__Fact_l0_main_)
	{
		int fact__Fact_l0_main_ = 1;
		For(1, a__Fact_l0_main_, 1, [&](int i__Fact_l0_main_)
		{
			fact__Fact_l0_main_ = mult(fact__Fact_l0_main_, i__Fact_l0_main_);
			
		}
		);
		return fact__Fact_l0_main_;
		
	}	;
	return 0;
	
}