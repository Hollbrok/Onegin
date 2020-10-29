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

struct line_t //��������� ������( �������� ���� ������ � �� �����).
    {
    char *line;
    int length;
    };

//-----------------------------------------------------------------------------

void LineProcessing(FILE *onegin, FILE *res, char *mode); //�������� ������� � main, � ��� ���������� ��� ��������������

void Sorting(line_t *lines, int n, char *mode); //���������� �����

void quicksort(line_t *lines, int l_0, int r_0, char *mode); //����������

void insertion(line_t *lines, int n, char *mode);

void switcher(line_t *lines, int l, int r);//������ ������ �������

int strcomp_decrease(line_t *lines, int a, int b);//�� ��������

int strcomp_increase(line_t *lines, int a, int b);//�� �����������

//-----------------------------------------------------------------------------

void LineProcessing (FILE *onegin, FILE *res, char *mode)
{
    assert(onegin != NULL);
    assert(res != NULL);
    assert(mode != NULL);

    fseek(onegin, 0, SEEK_END);
    long file_length = ftell(onegin);

    char *stream = (char *) calloc(file_length, sizeof(char));              // ������ �������� �������� �����
    assert(stream);

    fseek(onegin, 0, SEEK_SET);

    int file_lines = 0;                                                     //���-�� ����� � �����
    file_length = 0;                                                        //������ �����

    while((stream[file_length] = fgetc(onegin)) != EOF)
    {
        if(stream[file_length] == '\n')                                     // ���� "\n", �� + 1 � ���-�� �����
            file_lines++;

        file_length++;                                                      //��� ��������(+ 1 � ������ ������������ �������)
    }

    //printf("%d", file_length);

    free(stream + file_length);                                                 //����������� ������� ������

    struct line_t *lines = (line_t*)calloc(file_lines, sizeof(line_t));         // �������������� ��������� �� ���-�� �����, ������������ ����
    assert(lines);

    int k = 0;                                                                  // ������� ��������(�������, ���������, \n)
    int i = 0;                                                                  // ������� ��������
    while(TRUE)                                                                 // ������ ������ �� ������� stream � ���������
    {
        while(TRUE)
        {
            if(k >= file_length)
                break;
            if((stream[k] == ' ') || (stream[k] == '\t') || (stream[k] == '\n'))//���� ���� �� ������ ��������(������, ���������, \n), �� +1 � �������� ���� ��������
                k++;
            else
                break;
        }

        if(k >= file_length)
            break;

        lines[i].line = &stream[k];
        lines[i].length = k;
        while(stream[k] != '\n')//���� �� ����� ������
        {
            if(k >= file_length)
                break;
            k++;
        }
        k++;

        lines[i].length = k - lines[i].length; //������ ���-�� �������� �������(������ \t \n).
        i++;
    }

    Sorting(lines, i, mode);//���������� �����


    for(int t = 0; t < i; ++t)
        for(int j = 0; j < lines[t].length; j++)
            fprintf (res, "%c", lines[t].line[j]);//������ ������� ������� ��������������� ����� � ����(res)

    free(stream);//������������ ���������� ������

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

    /*if (!strcmp(mode, "rhyme"))
        str_comp = strcomp_rhyme;
    else */if(!strcmp(mode, "decrease"))
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

    }while(l <= r);// ���� l � r �� �������

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
    /*if (!strcmp(mode, "rhymes"))
        str_comp = strcomp_rhymes;
    else*/ if(!strcmp(mode, "decrease"))
        str_comp = strcomp_decrease;
    else if(!strcmp(mode, "increase"))
        str_comp = strcomp_increase;
    else exit(1);

    for(int i = 0; i < n - 1; i++)
        for(int j = i + 1; j < n; j++)
			if(str_comp(lines, i, j) > 0)
                switcher(lines, i, j);//���� ������ ������� str_comp
}

//-----------------------------------------------------------------------------

int strcomp_decrease(line_t *lines, int a, int b)
{
    assert(lines);

    int i = 0;
    int j = 0;

    while(!isalpha(lines[a].line[i]) || lines[a].line[i] == '\'' )//  ������ isalpha() ���������� ��������� ��������, ���� ��� �������� �������� ������ �������� (�������� ��� ������� ��������). � ��������� ������ ������������ 0.
        i++;                                                      //  ��� ����� � ��������� ����������
    while(!isalpha(lines[b].line[j]) || lines[a].line[i] == '\'' )//  ����������
        j++;

    while(i < lines[a].length && j < lines[b].length)            //  ���� ������� ������ ������� ������ ����� ������
    {
        if(lines[a].line[i] == lines[b].line[j])                 // ���� ������� ����� ����������� ������ �� 1
        {
            i++;
            j++;
        }
        else                                                      // ��� ������ ��� �� �����
            return -(lines[a].line[i] - lines[b].line[j]);        // ������ ���������� ��� �����������.
    }
    return -(lines[a].line[i] - lines[b].line[j]);                // ���� ��������� ����� ������, �� ������ ����������.
}

//-----------------------------------------------------------------------------

int strcomp_increase(line_t *lines, int a, int b)                   //��� ���������� Strcomp_decrease, �� return ���������������.
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

void switcher(line_t *lines, int l, int r)//������ ������ �������
{
    assert(lines);

    char *strtmp = lines[l].line;  //��������� ������
    lines[l].line = lines[r].line; //������ ������
    lines[r].line = strtmp;        //�������

    int tmp = lines[l].length;        //��������� ���������� �����
    lines[l].length = lines[r].length;//������ �����
    lines[r].length = tmp;            //�������
}

//-----------------------------------------------------------------------------




#endif // SORTLINE_H_INCLUDED
