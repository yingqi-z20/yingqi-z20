//
//  main.cpp
//  cube
//
//  Created by 张英奇 on 2020/9/20.
//  Copyright © 2020 张英奇 & 张程皓. All rights reserved.
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

bool success = 0;
int k = 0; //枚举的最大步数
short color[6][9] = {0};
short backup[6][9] = {0};
const short sur_trans[6] = {2, 3, 0, 1, 4, 5};
const int color_trans[6][9] = {{8, 7, 6, 1, 0, 5, 2, 3, 4},
                               {6, 7, 8, 5, 0, 1, 4, 3, 2},
                               {4, 3, 2, 5, 0, 1, 6, 7, 8},
                               {2, 3, 4, 1, 0, 5, 8, 7, 6},
                               {8, 7, 6, 1, 0, 5, 2, 3, 4},
                               {2, 3, 4, 1, 0, 5, 8, 7, 6}};
const char color_name[6] = {'R', 'B', 'Y', 'G', 'O', 'W'};
//short *clr[6][9];
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

/*void initialize()
{
    for (short i = 0; i < 6; i++)
    {
        for (short j = 0; j < 9; j++)
        {
            clr[i][j] = &color[sur_trans[i]][color_trans[i][j]];
        }
    }
}*/

bool CanUP = 1;

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
        for (short j = 1; j < 9; j++)
        {
            r += (color[i][0] != color[i][j]);
        }
    }
    return r;
}

short distant1()
{
    short r = 0;
    for (short j = 1; j < 9; j++)
    {
        r += (color[4][0] != color[4][j] && color[5][0] != color[4][j]);
        r += (color[4][0] != color[5][j] && color[5][0] != color[5][j]);
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
        printf("F ");
    else
        F();
}

void m7(bool b)
{
    if (b)
        printf("Fi ");
    else
        Fi();
}

void m8(bool b)
{
    if (b)
        printf("U ");
    else
        U();
}

void m9(bool b)
{
    if (b)
        printf("Ui ");
    else
        Ui();
}

void m10(bool b)
{
    if (b)
        printf("D ");
    else
        D();
}

void m11(bool b)
{
    if (b)
        printf("Di ");
    else
        Di();
}

void m12(bool b)
{
    if (b)
        printf("R R ");
    else
    {
        R();
        R();
    }
}

void m13(bool b)
{
    if (b)
        printf("F F ");
    else
    {
        F();
        F();
    }
}

void m14(bool b)
{
    if (b)
        printf("L L ");
    else
    {
        L();
        L();
    }
}

void m15(bool b)
{
    if (b)
        printf("B B ");
    else
    {
        B();
        B();
    }
}

void m16(bool b)
{
    if (b)
        printf("U U ");
    else
    {
        U();
        U();
    }
}

void m17(bool b)
{
    if (b)
        printf("D D ");
    else
    {
        D();
        D();
    }
}

const short fm_n = 18, fm_n1 = 12;

void (*formula[fm_n])(bool) = {m0, m1, m2, m3, m4, m5,
                               m6, m7, m8, m9, m10, m11,
                               m12, m13, m14, m15, m16, m17};

void (*formula1[fm_n1])(bool) = {m0, m1, m2, m3, m4, m5,
                                 m6, m7, m12, m13, m14, m15};

bool input()
{
    //initialize();
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
                        color[sur_trans[i]][color_trans[i][j]] = k;
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

bool output()
{
    FILE *fp;
    fp = stdout;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            putc(color_name[color[sur_trans[i]][color_trans[i][j]]], fp);
        }
        putc('\n', fp);
    }
    return 0;
}

bool test(int step)
{
    int cnt = 0;
    for (int i = 0; i < 6; ++i)
        for (int j = 1; j < 9; ++j)
            if (color[i][j] != color[i][0])
            {
                if (0.15 * (++cnt) > k - step) //当前步数(step)+估价函数值(cnt)>枚举的最大步数
                    return 0;
            }
    return 1;
}

bool test1(int step)
{
    int cnt = 0;
    for (int i = 4; i < 6; ++i)
        for (int j = 1; j < 9; ++j)
            if (color[i][j] != color[4][0] && color[i][j] != color[5][0])
            {
                if ((++cnt) * 0.29 > k - step) //当前步数(step)+估价函数值(cnt)>枚举的最大步数
                    return 0;
            }
    return 1;
}

bool test2(int step)
{
    int cnt = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 1; j < 9; ++j)
            if (color[i][j] != color[i][0])
            {
                if (0.3 * (++cnt) > k - step) //当前步数(step)+估价函数值(cnt)>枚举的最大步数
                    return 0;
            }
    return 1;
}

short op[64];

void A_star(int step, int pre)
{
    if (step == k)
    {
        if (!distant())
            success = 1;
        return;
    }
    //达到当前限制的最大深度
    if (success)
        return;
    for (int i = 0; i < fm_n; ++i)
    {
        formula[i](0);
        //output();
        int re = i < 12 ? i + 1 - 2 * (i & 1) : i;
        if (pre == re)
        {
            formula[re](0);
            continue; //加入了上述最优性剪枝
        }
        if (test(step) && !success)
            A_star(step + 1, i); //A*估价合法再向下搜索
        if (success)
        {
            for (int j = 0; j < 64; j++)
            {
                if (op[j] == -1)
                {
                    op[j] = i;
                    break;
                }
            }
            return;
        }
        formula[re](0);
    }
}

void A_star1(int step, int pre)
{
    //output();
    if (!distant1())
        success = 1;
    if (step == k || success)
    {
        return;
    }
    //达到当前限制的最大深度
    for (int i = 0; i < fm_n1; ++i)
    {
        formula[i](0);
        //output();
        int re = i < 12 ? i + 1 - 2 * (i & 1) : i;
        if (pre == re)
        {
            formula[re](0);
            continue; //加入了上述最优性剪枝
        }
        if (test1(step))
            A_star1(step + 1, i); //A*估价合法再向下搜索
        if (success)
        {
            //if(CanUP)
            upload();
            CanUP = 0;
            for (int j = 0; j < 64; j++)
            {
                if (op[j] == -1)
                {
                    op[j] = i;
                    break;
                }
            }
            return;
        }
        formula[re](0);
    }
}

void A_star2(int step, int pre)
{
    if (step == k)
    {
        if (!distant())
            success = 1;
        return;
    }
    //达到当前限制的最大深度
    if (success)
        return;
    for (int i = 8; i < fm_n; ++i)
    {
        formula[i](0);
        //output();
        int re = i < 12 ? i + 1 - 2 * (i & 1) : i;
        if (pre == re)
        {
            formula[re](0);
            continue; //加入了上述最优性剪枝
        }
        if (test2(step) && !success)
            A_star2(step + 1, i); //A*估价合法再向下搜索
        if (success)
        {
            for (int j = 0; j < 64; j++)
            {
                if (op[j] == -1)
                {
                    op[j] = i;
                    break;
                }
            }
            return;
        }
        formula[re](0);
    }
}

int main(int argc, const char *argv[])
{
    if (input())
        return 0;
    for (int i = 0; i < 64; i++)
    {
        op[i] = -1;
    }
    upload();
    /*while (++k < 9) //枚举最大深度
    {
        download();
        A_star(0, -1);
        if (success)
        {
            for(int i = 63;i >= 0;i--) {
                if(op[i] != -1) formula[op[i]](1);
            }
            fprintf(stderr, "%d\n", k);
            return 0;
        }
        if (k == 8)
            fprintf(stderr, "-1");
    }
    for(int i=0;i<64;i++) {
        op[i]=-1;
    }*/
    k = 0;
    while (++k) //枚举最大深度
    {
        download();
        A_star1(0, -1);
        if (success)
        {
            success = 0;
            for (int i = 63; i >= 0; i--)
            {
                if (op[i] != -1)
                    formula[op[i]](1);
            }
            fprintf(stderr, "%d\n", k);
            break;
        }
        if (k == 8)
            fprintf(stderr, "-1");
    }
    for (int i = 0; i < 64; i++)
    {
        op[i] = -1;
    }
    download();
    output();
    k = 0;
    success = 0;
    while (++k) //枚举最大深度
    {
        download();
        A_star2(0, -1);
        if (success)
        {
            for (int i = 63; i >= 0; i--)
            {
                if (op[i] != -1)
                    formula[op[i]](1);
            }
            fprintf(stderr, "%d\n", k);
            break;
        }
        if (k == 8)
            fprintf(stderr, "-1");
    }
    return 0;
}
