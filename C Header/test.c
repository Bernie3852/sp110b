#include "stdio.h"
#include "stdio_extend.h"

int main ()
{
    int a1,a2,a3,a4,a5;
    char b1,b2,b3,b4,b5;
    char* c1[100], c2[100], c3[100], c4[100], c5[100];

    //原生scanf
    //測試輸入 : "test 08-12A"
    _scanf("%s",c1);
    _scanf("%d%d %c", &a1, &a2, &b1);
    _printf("This is print by original printf\n%s date is :\n\"%d : %d\"\n%%%c%%\n\n", c1, a1, a2, b1); //原生printf輸出
    //自研scanf
    //測試輸入 : "test 08-12A"
    _scanf("%s",c1);
    _scanf("%d%d %c", &a1, &a2, &b1);
    _printf("This is print by my printf\n%s date is :\n\"%d : %d\"\n%%%c%%\n\n", c1, a1, a2, b1); //自研printf輸出

    //原生scanf
    //測試輸入 : "a b   c"
    scanf(" %c%c%c %c", &b1, &b2, &b3, &b4);
    printf("This is print by original printf\n1.\'%c\', 2.\'%c\', 3.\'%c\', 4.\'%c\'\n\n", b1, b2, b3 ,b4);   //原生printf輸出
    //自研scanf
    //測試輸入 : "a b   c"
    scanf(" %c%c%c %c", &b1, &b2, &b3, &b4);
    printf("This is print by my printf\n1.\'%c\', 2.\'%c\', 3.\'%c\', 4.\'%c\'\n\n", b1, b2, b3 ,b4);   //自研printf輸出
    
}

//測試結果

/*
$ ./test
test 08-12A
This is print by original printf
test date is :
"8 : -12"
%A%

test 08-12A
This is print by my printf
test date is :
"8 : -12"
%A%

a b   c
This is print by original printf
1.'a', 2.' ', 3.'b', 4.'c'

a b   c
This is print by my printf
1.'a', 2.' ', 3.'b', 4.'c'


*/