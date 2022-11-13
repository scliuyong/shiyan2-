// 我们在刚开始上课的时候介绍过一个小学四则运算自动生成程序的例子，请实现它，要求：
// ①　能够自动生成四则运算练习题
// ②　可以定制题目数量
// ③　用户可以选择运算符
// ④　用户设置最大数（如十以内、百以内等）
// ⑤　用户选择是否有括号、是否有小数
// ⑥　用户选择输出方式（如输出到文件、打印机等）
// ⑦　最好能提供图形用户界面（根据自己能力选做，以完成上述功能为主）

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "math.h"

#define GetRandomNum(maxNum) rand() % 10001 / 10000.0 * maxNum
#define GetRandomOp(opChar) opChar[rand() % strlen(opChar)]
/**
 * @brief 生成运算练习题
 * @param n 生成运算练习题数量
 * @param m 练习题有几项
 * @param opChar 运算符
 * @param maxNum 参与运算的最大数
 * @param isFloat 最大结果
 * @param isBracket 是否有括号
 * @param printFlag 0:打印到屏幕 1:打印到文件
 * @return char** 返回生成的运算练习题
 */
char** Generate(int n, int m, char* opChar, int maxNum, int isFloat, int isBracket, int printFlag);
//  84 88 84 85 82 87 87 89 85 90
int main()
{

    printf("请设置生成练习题数量：");
    int n;
    scanf("%d", &n);
    printf("请设置练习题几项运算：");
    int m;
    scanf("%d", &m);
    printf("请设置运算符：");
    char opChar[10];
    scanf("%s", opChar);
    printf("请设置参与运算的最大数：");
    int maxNum;
    scanf("%d", &maxNum);
    printf("请设置最大结果是否为小数(1:有小数 0:无小数):");
    int isFloat;
    scanf("%d", &isFloat);
    printf("请设置是否有括号(1:有括号 0:无括号):");
    int isBracket;
    scanf("%d", &isBracket);
    printf("请设置输出方式(1:直接打印屏幕上 0:输出到result.txt中):");
    int printFlag;
    scanf("%d", &printFlag);
    char** result = Generate(n, m, opChar, maxNum, isFloat, isBracket, printFlag);
    //char opChar[10] = "*/";
    //char **result = Generate(10, 4, opChar, 10, 1, 1, 0);
    return 0;
}

char** Generate(int n, int m, char* opChar, int maxNum, int isFloat, int isBracket, int printFlag)
{
    //根据时间来产生随机数种子
    srand((unsigned)time(0));
    // 动态申请运算式内存
    char** result = (char**)malloc(sizeof(char*) * n);

    for (int i = 0; i < n; i++)
    {
        double firstItem = GetRandomNum(maxNum);
        // firstItem转换为字符串
        char* tempFormula = (char*)malloc(sizeof(char) * m * 10);
        isFloat ? sprintf(tempFormula, "%.2f", firstItem) : sprintf(tempFormula, "%d", (int)firstItem);
        for (int j = 0; j < m - 1; j++)
        {
            char* tempItem = (char*)malloc(sizeof(char) * 10);
            // 随机生成运算符，然后拼接
            sprintf(tempItem, " %c ", GetRandomOp(opChar));
            strcat(tempFormula, tempItem);

            // 生成一个随机数
            double nextItem = 0;
            if (tempItem[1] == '/')
            {
                while (nextItem == 0 || nextItem < 1)
                {
                    nextItem = GetRandomNum(maxNum);
                }
            }
            else
            {
                nextItem = GetRandomNum(maxNum);
            }

            isFloat ? sprintf(tempItem, "%.2f", nextItem) : sprintf(tempItem, "%d", (int)nextItem);
            strcat(tempFormula, tempItem);

            free(tempItem);
        }
        strcat(tempFormula, " =\0");
        result[i] = tempFormula;
    }
    // 判断是否需要添加括号
    if (isBracket)
    {
        for (int i = 0; i < n; i++)
        {
            // 提取出运算式中的运算符
            char* op = (char*)malloc(sizeof(char) * 10);
            int opIndex = 0;
            for (int j = 0; j < strlen(result[i]); j++)
            {
                if (strchr(opChar, result[i][j]))
                {
                    op[opIndex++] = result[i][j];
                }
            }
            op[opIndex] = '\0';

            // 如果运算符中只有加减法，则在减号后面添加括号
            // 如果运算符中只有乘除法，则在除法部分添加括号
            // 如果运算符中有加减法和乘除法，则在加减法部分添加括号
            if ((strchr(op, '+') || strchr(op, '-')) && !strchr(op, '*') && !strchr(op, '/'))
            {
                // 如果op前一个运算符是减号，并且后一个运算符不是\0，则在后一个运算符前面添加括号
                for (int j = 0; j < strlen(op); j++)
                {
                    if (op[j] == '-' && op[j + 1] != '\0')
                    {
                        // 找到减号的位置
                        int minusIndex = 0;
                        for (int k = 0; k < strlen(result[i]); k++)
                        {
                            if (result[i][k] == '-')
                            {
                                minusIndex = k;
                                break;
                            }
                        }
                        int nextOpIndex = 0;
                        int OpCount = 0;
                        for (int k = minusIndex + 1; k < strlen(result[i]); k++)
                        {
                            // 判断result[i][k]是不是+-*/=中的一个
                            // printf("%s\n", strchr(opChar, result[i][k]));
                            int isOp = strchr(opChar, result[i][k]) ? 1 : 0;
                            if (result[i][k] == '=' || isOp)
                            {
                                if (OpCount)
                                {
                                    nextOpIndex = k;
                                    break;
                                }
                                OpCount++;
                            }
                        }
                        // 在minusIndex后面添加括号
                        char* temp = (char*)malloc(sizeof(char) * m * 10);
                        memset(temp, 0, sizeof(char) * m * 10);
                        strncpy(temp, result[i], minusIndex + 1);
                        strcat(temp, " (");
                        strncat(temp, result[i] + minusIndex + 1, nextOpIndex - minusIndex - 1);
                        strcat(temp, ") ");
                        strncat(temp, result[i] + nextOpIndex, strlen(result[i]) - nextOpIndex);
                        // 释放原来的内存，防止内存泄漏
                        free(result[i]);
                        result[i] = temp;
                        // printf("1: %s\n", result[i]);

                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            if ((strchr(op, '*') || strchr(op, '/')) && !strchr(op, '+') && !strchr(op, '-'))
            {
                // 如果op前一个运算符是除号，并且后一个运算符不是\0，则在后一个运算符前面添加括号
                for (int j = 0; j < strlen(op); j++)
                {
                    if (op[j] == '/' && op[j + 1] != '\0')
                    {
                        // 找到除号的位置
                        int divideIndex = 0;
                        for (int k = 0; k < strlen(result[i]); k++)
                        {
                            if (result[i][k] == '/')
                            {
                                divideIndex = k;
                                break;
                            }
                        }
                        int nextOpIndex = 0;
                        int OpCount = 0;
                        for (int k = divideIndex + 1; k < strlen(result[i]); k++)
                        {
                            // 判断result[i][k]是不是+-*/=中的一个
                            // printf("%s\n", strchr(opChar, result[i][k]));
                            int isOp = strchr(opChar, result[i][k]) ? 1 : 0;
                            if (result[i][k] == '=' || isOp)
                            {
                                if (OpCount)
                                {
                                    nextOpIndex = k;
                                    break;
                                }
                                OpCount++;
                            }
                        }
                        // printf("\n*/ %d %d %d\n", i, divideIndex, nextOpIndex);
                        // 在divideIndex后面添加括号
                        char* temp = (char*)malloc(sizeof(char) * m * 10);
                        memset(temp, 0, sizeof(char) * m * 10);

                        strncpy(temp, result[i], divideIndex + 1);
                        strcat(temp, " (");
                        strncat(temp, result[i] + divideIndex + 1, nextOpIndex - divideIndex - 1);
                        strcat(temp, ") ");
                        strncat(temp, result[i] + nextOpIndex, strlen(result[i]) - nextOpIndex);
                        // 释放原来的内存，防止内存泄漏
                        free(result[i]);
                        result[i] = temp;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            if ((strchr(op, '+') || strchr(op, '-')) && (strchr(op, '*') || strchr(op, '/')))
            {
                // 如果op前一个运算符是减号，并且后一个运算符不是\0，则在后一个运算符前面添加括号
                for (int j = 0; j < strlen(op); j++)
                {
                    if ((op[j] == '*' || op[j] == '/') && op[j + 1] != '\0')
                    {
                        // 找到减号的位置
                        int minusIndex = 0;
                        for (int k = 0; k < strlen(result[i]); k++)
                        {
                            if (result[i][k] == '*' || result[i][k] == '/')
                            {
                                minusIndex = k;
                                // printf("%d %d\n", i, minusIndex);
                                break;
                            }
                        }
                        int nextOpIndex = -1;
                        int OpCount = 0;
                        for (int k = minusIndex + 1; k < strlen(result[i]); k++)
                        {
                            // 判断result[i][k]是不是+-*/=中的一个
                            int isOp = strchr("+-", result[i][k]) ? 1 : 0;
                            if (result[i][k] == '=' || isOp)
                            {
                                if (OpCount)
                                {
                                    nextOpIndex = k;
                                    OpCount++;
                                    break;
                                }
                                OpCount++;
                            }
                            else
                            {
                                OpCount = 1;
                                break;
                            }
                        }
                        if (OpCount == 1)
                        {
                            break;
                        }
                        // 在minusIndex后面添加括号
                        char* temp = (char*)malloc(sizeof(char) * m * 10);
                        memset(temp, 0, sizeof(char)* m * 10);
                        strncpy(temp, result[i], minusIndex + 1);
                        strcat(temp, " (");
                        strncat(temp, result[i] + minusIndex + 1, nextOpIndex - minusIndex - 1);
                        strcat(temp, ") ");
                        strncat(temp, result[i] + nextOpIndex, strlen(result[i]) - nextOpIndex);
                        free(result[i]);
                        result[i] = temp;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
    }
    // 输出结果
    FILE* fp = fopen("result.txt", "w");
    for (int i = 0; i < n; i++)
    {
        // 如果printFlag为1:直接打印屏幕上 0:输出到result.txt中
        if (printFlag)
        {
            printf("%s\n", result[i]);

        }
        else
        {
            fprintf(fp, "%s\n", result[i]);
        }
    }
    fclose(fp);
    return result;
}