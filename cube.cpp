//
//  main.cpp
//  cube
//
//  Created by 张英奇 on 2020/9/20.
//

#include <cstdio>

#define R() spin(1, 0)
#define Ri() spin(1, 1)
#define L() spin(0, 1)
#define Li() spin(0, 0)
#define B() spin(3, 0)
#define Bi() spin(3, 1)
#define D() spin(5, 0)
#define Di() spin(5, 1)
#define F() spin(2, 1)
#define Fi() spin(2, 0)
#define U() spin(4, 1)
#define Ui() spin(4, 0)

using namespace std;

short color[6][9] = {0};
short backup[6][9] = {0};
const short sur_trans[6] = {2, 3, 0, 1, 4, 5};
const short color_trans[3][9] = {{8, 7, 6, 1, 0, 5, 2, 3, 4},
                                 {6, 7, 8, 5, 0, 1, 4, 3, 2},
                                 {2, 3, 4, 1, 0, 5, 8, 7, 6}};
const char color_name[6] = {'R', 'B', 'Y', 'G', 'O', 'W'};
short *clr[6][9];
short *sur_next[6][12] = {
    {&color[2][2], &color[2][1], &color[2][8], &color[4][2], &color[4][1],
     &color[4][8], &color[3][8], &color[3][1], &color[3][2], &color[5][8],
     &color[5][1], &color[5][2]},
    {&color[2][4], &color[2][5], &color[2][6], &color[4][4], &color[4][5],
     &color[4][6], &color[3][6], &color[3][5], &color[3][4], &color[5][6],
     &color[5][5], &color[5][4]},
    {&color[0][2], &color[0][1], &color[0][8], &color[5][2], &color[5][3],
     &color[5][4], &color[1][8], &color[1][1], &color[1][2], &color[4][4],
     &color[4][3], &color[4][2]},
    {&color[0][4], &color[0][5], &color[0][6], &color[5][8], &color[5][7],
     &color[5][6], &color[1][6], &color[1][5], &color[1][4], &color[4][6],
     &color[4][7], &color[4][8]},
    {&color[0][4], &color[0][3], &color[0][2], &color[2][8], &color[2][7],
     &color[2][6], &color[1][2], &color[1][3], &color[1][4], &color[3][6],
     &color[3][7], &color[3][8]},
    {&color[0][6], &color[0][7], &color[0][8], &color[2][2], &color[2][3],
     &color[2][4], &color[1][8], &color[1][7], &color[1][6], &color[3][4],
     &color[3][3], &color[3][2]},
};
short trans(short i, short j)
{
    switch (i)
    {
    case 0:
    case 2:
    case 4:
        return color_trans[0][j];
    case 1:
    case 3:
        return color_trans[1][j];
    case 5:
        return color_trans[2][j];
    }
    return 1024;
}
void spin(short s, bool b)
{
    short temp[3];
    if (b)
    {
        temp[0] = *sur_next[s][0];
        temp[1] = *sur_next[s][1];
        temp[2] = *sur_next[s][2];
        for (short i = 0; i < 9; i++)
        {
            *sur_next[s][i] = *sur_next[s][i + 3];
        }
        *sur_next[s][9] = temp[0];
        *sur_next[s][10] = temp[1];
        *sur_next[s][11] = temp[2];
        temp[0] = color[s][3];
        temp[1] = color[s][2];
        for (short i = 3; i < 9; i++)
        {
            color[s][i - 1] = color[s][i % 8 + 1];
        }
        color[s][1] = temp[0];
        color[s][8] = temp[1];
    }
    else
    {
        temp[0] = *sur_next[s][9];
        temp[1] = *sur_next[s][10];
        temp[2] = *sur_next[s][11];
        for (short i = 11; i >= 3; i--)
        {
            *sur_next[s][i] = *sur_next[s][i - 3];
        }
        *sur_next[s][0] = temp[0];
        *sur_next[s][1] = temp[1];
        *sur_next[s][2] = temp[2];
        temp[0] = color[s][1];
        temp[1] = color[s][8];
        for (short i = 8; i > 2; i--)
        {
            color[s][i % 8 + 1] = color[s][i - 1];
        }
        color[s][3] = temp[0];
        color[s][2] = temp[1];
    }
}
void initialize()
{
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            clr[i][j] = &color[sur_trans[i]][trans(i, j)];
        }
    }
}
void upload()
{
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            backup[i][j] = color[i][j];
        }
    }
}
void download()
{
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            color[i][j] = backup[i][j];
        }
    }
}
short d = 256;
short distant()
{
    short r = 0;
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            r += (color[i][j] != color[i][0]);
        }
    }
    return r;
}
void m0(bool b)
{
    if (b)
        printf("R ");
    else
        R();
}
void m1(bool b)
{
    if (b)
        printf("Ri ");
    else
        Ri();
}
void m2(bool b)
{
    if (b)
        printf("L ");
    else
        L();
}
void m3(bool b)
{
    if (b)
        printf("Li ");
    else
        Li();
}
void m4(bool b)
{
    if (b)
        printf("B ");
    else
        B();
}
void m5(bool b)
{
    if (b)
        printf("Bi ");
    else
        Bi();
}
void m6(bool b)
{
    if (b)
        printf("D ");
    else
        D();
}
void m7(bool b)
{
    if (b)
        printf("Di ");
    else
        Di();
}
void m8(bool b)
{
    if (b)
        printf("F ");
    else
        F();
}
void m9(bool b)
{
    if (b)
        printf("Fi ");
    else
        Fi();
}
void m10(bool b)
{
    if (b)
        printf("U ");
    else
        U();
}
void m11(bool b)
{
    if (b)
        printf("Ui ");
    else
        Ui();
}
const short fm_n = 12;
void (*formula[12])(bool) = {m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11};

bool input()
{
    initialize();
    char c;
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9;)
        {
            c = getchar();
            if (c != '\n' && c != ' ')
            {
                for (short k = 0; k < 6; k++)
                {
                    if (color_name[k] == c)
                    {
                        *clr[i][j] = k;
                        j++;
                    }
                }
            }
        }
    }
    upload();
    d = distant();
    return 0;
}
void restore()
{
    while (d != 0)
    {
        short d_min = 1024, i_min = 1024;
        for (short i = 0; i < fm_n; i++)
        {
            formula[i](0);
            if (distant() < d_min)
            {
                d_min = distant();
                i_min = i;
            }
            download();
        }
        if (d_min < d)
        {
            formula[i_min](0);
            formula[i_min](1);
            upload();
            d = distant();
            goto end;
        }
        formula[i_min](0);
        formula[i_min](1);
        upload();
        d = distant();
    end:;
    }
}
int main(int argc, const char *argv[])
{
    if (input())
        return 0;
    restore();
    return 0;
}
