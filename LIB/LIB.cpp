// LIB.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

int sum(int a, int b)
{
	return a + b;
}

int minus(int a, int b)
{
	return a - b;
}

int mult(int a, int b)
{
	return a * b;
}

int division(int a, int b)
{
	return a / b;
}
void For(int start, int end, int step, std::function<void(int)> foo)
{
	if (start < end)
	{
		for (int i = start; i <= end; i += step)
		{
			foo(i);
		}
	}
	else
	{
		for (int i = start; i >= end; i -= step)
		{
			foo(i);
		}
	}
}

void For(char start, char end, int step, std::function<void(char)> foo)
{
	if (start < end)
	{
		for (int i = start; i <= end; i += step)
		{
			foo(i);
		}
	}
	else
	{
		for (int i = start; i >= end; i -= step)
		{
			foo(i);
		}
	}
}