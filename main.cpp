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

int color[6][9] = {0};
const int sur_trans[6] = {2, 3, 0, 1, 4, 5};
const int color_trans[3][9] = {{8, 7, 6, 1, 0, 5, 2, 3, 4},
                               {6, 7, 8, 5, 0, 1, 4, 3, 2},
                               {2, 3, 4, 1, 0, 5, 8, 7, 6}};
const char color_name[6] = {'R', 'B', 'Y', 'G', 'O', 'W'};
int *sur_next[6][12] = {
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
int *clr[6][9];
void spin(int s, bool b)
{
    int temp[3];
    temp[0] = *sur_next[s][0];
    temp[1] = *sur_next[s][1];
    temp[2] = *sur_next[s][2];
    for (int i = 0; i < 9; i++)
    {
        *sur_next[s][i] = *sur_next[s][i + 3];
    }
    *sur_next[s][9] = temp[0];
    *sur_next[s][10] = temp[1];
    *sur_next[s][11] = temp[2];
    for (int j = 0; j < 3; j++)
    {
        temp[0] = color[s][1];
        temp[1] = color[s][8];
        for (int i = 8; i > 2; i--)
        {
            color[s][i % 8 + 1] = color[s][(i + 6) % 8 + 1];
        }
        color[s][3] = temp[0];
        color[s][2] = temp[1];
    }
    if (b == 0)
    {
        spin(s, 1);
        spin(s, 1);
    }
}
int trans(int i, int j)
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
void initialize()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            clr[i][j] = &color[sur_trans[i]][trans(i, j)];
        }
    }
}
bool input()
{
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (fp == 0)
        return 1;
    char c;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9;)
        {
            c = getc(fp);
            if (c != '\n' && c != ' ')
            {
                for (int k = 0; k < 6; k++)
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
    fclose(fp);
    return 0;
}
bool output()
{
    FILE *fp;
    fp = fopen("output.txt", "w");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            putc(color_name[*clr[i][j]], fp);
        }
        putc('\n', fp);
    }
    fclose(fp);
    return 0;
}
int main(int argc, const char *argv[])
{
    initialize();
    if (input())
        return 0;
    output();
    return 0;
}
