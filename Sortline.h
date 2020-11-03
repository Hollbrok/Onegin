#ifndef SORTLINE_H_INCLUDED
#define SORTLINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>
//#include <locale.h>         //Библиотеки для работы с русским языком
//#include <windows.h>        // -//-
//#include <winuser.h>        // -//-

const int LIM_SIZE = 500;
const int BRED_SIZE = 100; //кол-во четверостиший.
const int TRUE = 1;
const int FALSE = 0;

struct line_t //структура строки( содержит саму строку и ее длину).
{
    char *line;
    int length;
};

void LineProcessing(FILE *onegin, FILE *res, char *mode, char* isbred); //основная функция в main, в ней происходят все преобразования.

void Sorting(line_t *lines, int n, char *mode); //сортировка строк.

void Quicksort(line_t *lines, int l_0, int r_0, char *mode); //сортировка.

void Insertion(line_t *lines, int n, char *mode);

void Switcher(line_t *lines, int l, int r);//меняет строки местами.

void Bred_Gen(line_t *lines, char *bred_ind, int n);//бредогенератор.

int strcomp_decrease(line_t *lines, int a, int b);//по убыванию.

int strcomp_decrease_end(line_t *lines, int a, int b);//по убыванию c конца строки.

int strcomp_increase(line_t *lines, int a, int b);//по возрастанию.

int strcomp_increase_end(line_t *lines, int a, int b);//по возрастанию с конца строки.

#endif // SORTLINE_H_INCLUDED
