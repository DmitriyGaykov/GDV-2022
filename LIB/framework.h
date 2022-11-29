#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#include <functional>

int sum(int a, int b);

int minus(int a, int b);

int mult(int a, int b);

int division(int a, int b);

void For(int, int, int, std::function<void(int)> foo);

void For(char, char, int, std::function<void(char)> foo);
