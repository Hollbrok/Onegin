#ifndef SORTLINE_H_INCLUDED
#define SORTLINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


const int LIM_SIZE = 500;

const int TRUE = 1;
const int FALSE = 0;

struct line_t //структура строки( содержит саму строку и ее длину).
    {
    char *line;
    int length;
    };

//-----------------------------------------------------------------------------

void LineProcessing(FILE *onegin, FILE *res, char *mode); //основная функция в main, в ней происходят все преобразования

void Sorting(line_t *lines, int n, char *mode); //сортировка строк

void quicksort(line_t *lines, int l_0, int r_0, char *mode); //сортировка

void insertion(line_t *lines, int n, char *mode);

void switcher(line_t *lines, int l, int r);//меняет строки местами

int strcomp_decrease(line_t *lines, int a, int b);//по убыванию

int strcomp_decrease_end(line_t *lines, int a, int b);//по убыванию c конца строки

int strcomp_increase(line_t *lines, int a, int b);//по возрастанию

int strcomp_increase_end(line_t *lines, int a, int b);//по возрастанию с конца строки

//-----------------------------------------------------------------------------

void LineProcessing (FILE *onegin, FILE *res, char *mode)
{
    assert(onegin != NULL);
    assert(res != NULL);
    assert(mode != NULL);

    fseek(onegin, 0, SEEK_END);
    long file_length = ftell(onegin);

    char *stream = (char *) calloc(file_length, sizeof(char));              // массив хранения символов файла
    assert(stream);

    fseek(onegin, 0, SEEK_SET);

    int file_lines = 0;                                                     //кол-во строк в файле
    file_length = 0;                                                        //размер файла

    while((stream[file_length] = fgetc(onegin)) != EOF)
    {
        if(stream[file_length] == '\n')                                     // если "\n", то + 1 к кол-ву строк
            file_lines++;

        file_length++;                                                      //шаг итерации(+ 1 к номеру проверяемого символа)
    }

    //printf("%d", file_length);

    free(stream + file_length);                                                 //освобождаем остатки памяти

    struct line_t *lines = (line_t*)calloc(file_lines, sizeof(line_t));         // инициализируем структуры по кол-ву строк, рассчитанных выше
    assert(lines);

    int k = 0;                                                                  // счётчик символов(пробелы, табуляции, \n)
    int i = 0;                                                                  // счётчик структур
    while(TRUE)                                                                 // читаем строки из массива stream в структуры
    {
        while(TRUE)
        {
            if(k >= file_length)
                break;
            if((stream[k] == ' ') || (stream[k] == '\t') || (stream[k] == '\n'))//если один из первых символов(пробел, табуляция, \n), то +1 к счетчику этих символов
                k++;
            else
                break;
        }

        if(k >= file_length)
            break;

        lines[i].line = &stream[k];
        lines[i].length = k;
        while(stream[k] != '\n')//пока не конец строки
        {
            if(k >= file_length)
                break;
            k++;
        }
        k++;

        lines[i].length = k - lines[i].length; //отнять кол-во символов вначале(пробел \t \n).
        i++;
    }

    Sorting(lines, i, mode);//сортировка строк


    for(int t = 0; t < i; ++t)
        for(int j = 0; j < lines[t].length; j++)
            fprintf (res, "%c", lines[t].line[j]);//печать каждого символа отсортировынных строк в файл(res)

    free(stream);//освобождение выделенной памяти

}

//-----------------------------------------------------------------------------

void Sorting(line_t *lines, int n, char *mode)
{
    quicksort(lines, 0, n - 1, mode);
    insertion(lines, n, mode);
}

//-----------------------------------------------------------------------------

void quicksort(line_t *lines, int l_0, int r_0, char *mode)
{
    assert(lines);
    assert(mode);

    int piv = (l_0 + r_0)/2;
    int l = l_0;
    int r = r_0;

    int (*str_comp)(line_t*, int, int);

    if (!strcmp(mode, "decrease_end"))
        str_comp = strcomp_decrease_end;
    else if (!strcmp(mode, "increase_end"))
        str_comp = strcomp_increase_end;
    else if(!strcmp(mode, "decrease"))
        str_comp = strcomp_decrease;
    else if(!strcmp(mode, "increase"))
        str_comp = strcomp_increase;
    else exit(1);

    do
    {
        while(str_comp(lines, piv, l) > 0)
            l++;
        while(str_comp(lines, r, piv) > 0)
            r--;

        if(l <= r)
            switcher(lines, l++, r--);

    }while(l <= r);// пока l и r не совпали

    if(l_0 < r)
        quicksort(lines, l_0, r, mode);
    if(r_0 > l)
        quicksort(lines, l, r_0, mode);
}

//-----------------------------------------------------------------------------

void insertion(line_t *lines, int n, char *mode)
{
    assert(lines);
    assert(mode);

    int (*str_comp)(line_t*, int, int);

    if (!strcmp(mode, "decrease_end"))
        str_comp = strcomp_decrease_end;
    else if (!strcmp(mode, "increase_end"))
        str_comp = strcomp_increase_end;
    else if(!strcmp(mode, "decrease"))
        str_comp = strcomp_decrease;
    else if(!strcmp(mode, "increase"))
        str_comp = strcomp_increase;
    else exit(1);

    for(int i = 0; i < n - 1; i++)
        for(int j = i + 1; j < n; j++)
			if(str_comp(lines, i, j) > 0)
                switcher(lines, i, j);//если нужное условие str_comp
}

//-----------------------------------------------------------------------------

int strcomp_decrease(line_t *lines, int a, int b)
{
    assert(lines);

    int i = 0;
    int j = 0;

    while(!isalpha(lines[a].line[i]) || lines[a].line[i] == '\'' )//  Макрос isalpha() возвращает ненулевое значение, если его аргумент является буквой алфавита (верхнего или нижнего регистра). В противном случае возвращается 0.
        i++;                                                      //  все цифры и апострофы пропускаем
    while(!isalpha(lines[b].line[j]) || lines[a].line[i] == '\'' )//  аналогично
        j++;

    while(i < lines[a].length && j < lines[b].length)            //  Пока индексы номера символа меньше длины строки
    {
        if(lines[a].line[i] == lines[b].line[j])                 // Пока символы равны увеличиваем индесы на 1
        {
            i++;
            j++;
        }
        else                                                      // Как только они не равны
            return -(lines[a].line[i] - lines[b].line[j]);        // делаем возращение для компаратора.
    }
    return -(lines[a].line[i] - lines[b].line[j]);                // Если достигнут конец строки, то делаем аналогично.
}

//-----------------------------------------------------------------------------

int strcomp_increase(line_t *lines, int a, int b)                   //все аналогично Strcomp_decrease, но return противоположный.
{
    assert(lines);

    int i = 0;
    int j = 0;

    while(!isalpha(lines[a].line[i]) || lines[a].line[i] == '\'' )
        i++;
    while(!isalpha(lines[b].line[j]) || lines[a].line[i] == '\'' )
        j++;

    while(i < lines[a].length && j < lines[b].length)
    {
        if(lines[a].line[i] == lines[b].line[j])
        {
            i++;
            j++;
        }
        else
            return (lines[a].line[i] - lines[b].line[j]);
    }
    return (lines[a].line[i] - lines[b].line[j]);
}

//-----------------------------------------------------------------------------

void switcher(line_t *lines, int l, int r)//меняем строки местами
{
    assert(lines);

    char *strtmp = lines[l].line;  //временная строка
    lines[l].line = lines[r].line; //меняем строки
    lines[r].line = strtmp;        //местами

    int tmp = lines[l].length;        //временная переменная длины
    lines[l].length = lines[r].length;//меняем длины
    lines[r].length = tmp;            //местами
}

//-----------------------------------------------------------------------------

int strcomp_increase_end(line_t *lines, int a, int b)
{
    assert(lines);

    int i = lines[a].length;
    int j = lines[b].length;

    while(!isalpha(lines[a].line[i]) || lines[a].line[i] == '\'' )
        i--;
    while(!isalpha(lines[b].line[j]) || lines[a].line[i] == '\'' )
        j--;

    while(i > 0 && j > 0)
    {
        if(lines[a].line[i] == lines[b].line[j])
        {
            i--;
            j--;
        }
        else
            return (lines[a].line[i] - lines[b].line[j]);
    }
    return (lines[a].line[i] - lines[b].line[j]);

}

//-----------------------------------------------------------------------------

int strcomp_decrease_end(line_t *lines, int a, int b)
{
    assert(lines);

    int i = lines[a].length;
    int j = lines[b].length;

    while(!isalpha(lines[a].line[i]) || lines[a].line[i] == '\'' )
        i--;                                                      
    while(!isalpha(lines[b].line[j]) || lines[a].line[i] == '\'' )
        j--;

    while(i > 0 && j > 0)            
    {
        if(lines[a].line[i] == lines[b].line[j])                
        {
            i--;
            j--;
        }
        else                                                    
            return -(lines[a].line[i] - lines[b].line[j]);      
    }
    return -(lines[a].line[i] - lines[b].line[j]);              
}
//-----------------------------------------------------------------------------


#endif // SORTLINE_H_INCLUDED
