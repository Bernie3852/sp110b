#include <stdio.h>
#include "list_in _C_v3.0.h"

int main ()
{
    list *A = create();
    list *C = create();
    list *D = create();
    append(A, INT, 123);
    append(A, DOUBLE, 123.5);
    
    int B[] = {123, 456, 789};
    append(A, INT_PTR, B);

    char *words = "I love you";
    append(A, CHAR_PTR, words);
    print(A);
    printf("\n%d", sizeof(*A));
    printf("\n");
    
    append(C, LIST_PTR, A);
    print(C);
    printf("\n");
    print(C);
    printf("\n");
    print(A);
    append(D, LIST_PTR, C);
    printf("\n");
    append(D, LIST_PTR, A);
    append(D, INT, 123);
    printf("\n");
    print(D);
    printf("\n");
    reverse(D);
    print(D);
    printf("\n");
    delete(D, LIST_PTR, A);
    print(D);
    delete(D, LIST_PTR, C);
    printf("\n");
    print(D);
    clear(D);
    print(D);
}

int main ()
{
    printf("%d", '1' - 1);
}