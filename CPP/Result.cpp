#include <iostream>
#include <windows.h>
using namespace std;


#pragma comment(lib, "..\\GDV-2022\\Debug\\LIB.lib")
#include "..\\LIB\\framework.h"


int Fact_(int a_Fact)
{
	int fact_Fact = 1;
	For(1, a_Fact, 1, [&](int i_Fact)
	{
		fact_Fact = mult(fact_Fact, i_Fact);
		
	}
	);
	return fact_Fact;
	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto pow_l1main = [&](int number_powl1main, int power_powl1main)
	{
		int result_powl1main = 1;
		For(1, power_powl1main, 1, [&](int i_powl1main)
		{
			result_powl1main = mult(result_powl1main, number_powl1main);
			
		}
		);
		return result_powl1main;
		
	}	;
	cout << pow_l1main(2, 3);
	return 0;
	
}