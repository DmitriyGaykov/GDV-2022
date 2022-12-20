#include <iostream>
#include <windows.h>

#pragma comment(lib, "C:\\Users\\dimag\\OneDrive\\Рабочий стол\\Курсач КПО\\GDV-2022\\Debug\\LIB.lib")
#include "C:\\Users\\dimag\\OneDrive\\Рабочий стол\\Курсач КПО\\LIB\\framework.h"


int Fact_$(int a_Fact_$)
{
	int res_Fact_$ = 1; 
	For((int)1, (int)a_Fact_$, 1, [&](int el_$For1_Fact_$)
	{
		res_Fact_$ = mult((int)res_Fact_$, (int)el_$For1_Fact_$); 
		
	
		return false;
	}); 
	return (int)(res_Fact_$); 
	
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int a_$l1_main_$ = Fact_$((int)3); 
	if (a_$l1_main_$ == 6)
	{
		std::cout << "a равно 6"; 
		std::cout << '\n'; 
		
	}
	else	
	{
		std::cout << "a не равно 6"; 
		std::cout << '\n'; 
		
	}	
	{
		return (int)(0); 
		
	}
	auto round_$l1_main_$ = [&](float a_round_$l1_main_$)
	{
		int numa_round_$l1_main_$ = a_round_$l1_main_$; 
		if (minus((float)a_round_$l1_main_$, (float)numa_round_$l1_main_$) > 0.5)
		{
			return (float)(sum((int)numa_round_$l1_main_$, (int)1)); 
			
		}
		return (float)(numa_round_$l1_main_$); 
		
	}; 
	float half_$l1_main_$ = round_$l1_main_$((float)78.67); 
	std::cout << "rounded half = " << half_$l1_main_$; 
	return (int)(0); 
	
}