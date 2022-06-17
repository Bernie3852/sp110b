#ifndef STDIO_EXTEND_H
#define STDIO_EXTEND_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

char INPUT_BUFF = 0; //如果讀數字時有多讀到字元時用的暫存區

char *_gets(char buffer[])
{
    int index = 0;
    char input = 0;
    while ((input = getc(stdin)) != '\n')
    { //直到讀入enter鍵之前持續讀取
        buffer[index++] = input;
    }
    buffer[index] = '\0';
    return buffer;
}

int _gets_skip_blank(char buffer[]) //用於%s讀取
{
    int index = 0;
    char input = 0;

    //若暫存區中還有文字，將其讀入並清除
    if (INPUT_BUFF)
    {
        if (INPUT_BUFF != '\n' && INPUT_BUFF != '\t' && INPUT_BUFF != ' ')
        {
            input = INPUT_BUFF;
            INPUT_BUFF = 0;
        }
        else
        {
            INPUT_BUFF = 0;
            do
                input = getc(stdin);
            while (input == ' ' || input == '\t' || input == '\n'); //省略字串前面的任何空格, Tab鍵, enter鍵
        }
    }
    else
        do
            input = getc(stdin);
        while (input == ' ' || input == '\t' || input == '\n'); //省略字串前面的任何空格, Tab鍵, enter鍵

    while (1)
    {
        //直到讀入enter, Tab, 空格鍵之前持續讀取
        if (input != '\n' && input != '\t' && input != ' ')
        {
            buffer[index++] = input;
            input = getc(stdin);
        }
        else
            break;
    }
    buffer[index] = '\0';
}

int is_number(char input)   //檢查該文字是否屬於數字or負號
{
    return '0' <= input && input <= '9' || input == '-';
}

int _gets_number(char buffer[]) //用於%d讀取
{
    int index = 0;
    char input = 0;

    if (INPUT_BUFF && INPUT_BUFF != ' ' && INPUT_BUFF != '\t' && INPUT_BUFF != '\n' && INPUT_BUFF != '-') //若暫存區中還有文字，回傳-1表示錯誤
        return -1;

    if (INPUT_BUFF == '-') //若暫存區中是負號
    {
        input = INPUT_BUFF;
        INPUT_BUFF = 0;
    }
    else
        do
        {
            input = getc(stdin);
            if (!is_number(input) && input != ' ' && input != '\t' && input != '\n') //若讀到的首字為文字則將其保存並回傳-1表示錯誤
            {
                INPUT_BUFF = input;
                return -1;
            }
        } while (input == ' ' || input == '\t' || input == '\n'); //省略字串前面的任何空格, Tab, enter鍵

    while (1)
    { //直到讀入enter, Tab, 空格鍵, 非數字之前持續讀取
        if (input != '\n' && input != '\t' && input != ' ')
        {
            buffer[index++] = input;
            input = getc(stdin);
        }
        else
            break;
        if (!is_number(input) || input == '-') //若讀到的字為文字或負號則將其保存並停止讀取
        {
            INPUT_BUFF = input;
            break;
        }
    }
    buffer[index] = '\0';
    return index - 1;
}

int _puts(char buffer[])
{
    int index = 0;
    while (putc(buffer[index++], stdout) != '\0')
    {
    } //直到讀入字串串末'0'之前持續輸出
    return 0;
}

int words_to_int(char *number, int index) //將char *型態的數字轉為int
{
    int output = 0;
    for (int i = index; i != 0; --i)
        output += (number[i] - '0') * pow(10, index - i);
    if (number[0] == '-')
        output = -output;
    else
        output += (number[0] - '0') * pow(10, index);
    return output;
}

char *int_to_words(int number) //將int型態的數字轉為char *, output第0項儲存字串的起始index
{
    int index = 12;
    char *output = (char *)calloc(13, sizeof(char));

    if (number == 0)
    {
        output[index] = '0';
        output[0] = index;
    }
    else if (number > 0)
    {
        for (; number != 0; --index, number /= 10)
            output[index] = number % 10 + '0';
        output[0] = index + 1;
    }
    else
    {
        number = -number;
        for (; number != 0; --index, number /= 10)
            output[index] = number % 10 + '0';
        output[index] = '-';
        output[0] = index;
    }
    return output;
}

int _scanf(const char requires[], ...) //目前僅支援%c, %s, %d的基本輸入，使用%d%s讀取，輸入時已不需要需要將數字與文字分開
{
    va_list args; //宣告參數列表

    va_start(args, requires); //讀入第一個參數

    for (int i = 0; requires[i] != '\0'; ++i)
    {
        if (requires[i] == '%')
        {
            void *temp = 0;
            switch (requires[++i])
            {
            case 'c':
                temp = (char *)va_arg(args, char *);
                char input = ' ';
                if (INPUT_BUFF) //若暫存區內有已讀取的字元
                {
                    input = INPUT_BUFF;
                    INPUT_BUFF = 0;
                }
                if (requires[i - 2] == ' ') //若%c前面有空格，省略字串前面的任何空格, Tab鍵, enter鍵
                {
                    while (input == ' ' || input == '\t' || input == '\n')
                        input = getc(stdin);
                    *(char *)temp = input;
                }
                else
                    *(char *)temp = getc(stdin);
                break;
            case 's':
                temp = (char *)va_arg(args, char *);
                _gets_skip_blank((char *)temp); //會省略字串前空格且以空格截斷版本之gets()
                break;
            case 'd':
                temp = (int *)va_arg(args, int *);
                char inputs[13] = {};   //數字暫存
                int index = _gets_number(inputs);   //會省略字串前空格且以非數字輸入截斷版本之gets()
                if (index == -1)        //錯誤的輸入格式處理, 以免記憶體錯誤
                    return 1;
                *(int *)temp = words_to_int(inputs, index);
                break;
            default:
                va_end(args); //清理已使用完畢的參數
                return 1;     //若不符合輸入規則則回傳1
            }
        }
    }

    va_end(args); //清理已使用完畢的參數
    return 0;
}

int _printf(const char requires[], ...) //目前僅支援%c, %s, %d的基本輸出，並支援常用跳脫字元，但不支援格式字元
{
    va_list args; //宣告參數列表

    va_start(args, requires); //讀入第一個參數

    for (int i = 0; requires[i] != '\0'; ++i)
    {
        if (requires[i] == '%')
        {
            char ch = 0;    //字元暫存
            int in = 0;     //數字暫存
            char *temp = 0; //字串暫存
            switch (requires[++i])
            {
            case 'c':
                ch = (char)va_arg(args, int);
                putc(ch, stdout);
                break;
            case 's':
                temp = (char *)va_arg(args, char *);
                _puts(emp);         //直接沿用_puts()輸出字串
                break;
            case 'd':
                char *words = 0;
                in = (int)va_arg(args, int);
                words = int_to_words(in);   //int_to_words為將數字轉換為字串的函式，會動態規劃一個空間所以記得要free
                _puts(&words[words[0]]);    //直接沿用_puts()，words第0項儲存該數字串之起始index
                free(words);        //釋放int_to_words中動態規劃的空間
                break;
            case '%':
                putc('%', stdout);
            }
        }
        else if (requires[i] == '\\')
        {
            switch (requires[++i])
            {
            case '\'':
                putc('\'', stdout);
                break;
            case '\"':
                putc('\"', stdout);
                break;
            case '\\':
                putc('\\', stdout);
                break;
            case '0':
                putc('\0', stdout);
                break;
            case 't':
                putc('\t', stdout);
                break;
            case 'n':
                putc('\n', stdout);
                break;
            case 'b':
                putc('\b', stdout);
                break;
            }
        }
        else
            putc(requires[i], stdout);
    }
    va_end(args); //清理已使用完畢的參數
    return 0;
}

#endif