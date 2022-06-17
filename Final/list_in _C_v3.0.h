//鍊狀資料結構函式庫 in C  by Bernie Lee
//最近一次更新於2020/03/12
//本函式庫主要功能參考自python之列表相關functions，並額外新增了應對語言間差異所替代用function，也擁有實驗性gc功能
//目前支援的基本資料型態及其對應formate定義如下 : (list *) -> LIST_PTR, (int) -> INT, (int *) -> INT_PTR, (double) -> DOUBLE, (double *) -> DOUBLE_PTR, (char *) -> CHAR_PTR
//INT_PTR, DOUBLE_PTR類型僅記錄記憶體位置而不會複製其內容物
//Sort()函式目前尚未支援

//目前將列表插入列表使用深複製，將在之後的版本進行後續處裡

// 2021/03/12 新增print()函式，可列印出列表的所有內容
// 2021/03/12 修正delete(), count(), search()函樹之錯誤
// 2021/03/12 更新compare()函式可比較兩列表之內容是否相同


#define LIST_PTR 1
#define INT 10
#define INT_PTR 11
#define DOUBLE 20
#define DOUBLE_PTR 21
#define CHAR_PTR 31

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

#undef delete

typedef struct list_struct list_struct;
struct list_struct
{
    void *element;
    int formate;
    list_struct *previous;
    list_struct *next;
};

typedef struct list list;
struct list
{
    list_struct *head;
    list_struct *tail;
    int count;   //列表項數計數器
    void **gbs;  //廢棄記憶體位置儲存陣列
    int gbcount; //廢棄記憶體計數器
    int gbsize;  //廢棄記憶體陣列大小
};

list *create();
//void copy(list );
void append(list *list_name, const int formate, ...);
void insert(list *list_name, int index, const int formate, ...);
void *pop(list *list_name, int index);
int delete (list *list_name, int formate, ...);
int search(list *list_name, int formate, ...);
int count(list *list_name, int formate, ...);
int reverse(list *list_name);
int get_formate(list *list_name, int index);
void *address(list *list_name, int index);
void gbc(list *list_name, void *ptr);
void gc(list *list_name);
int print(list *list_name);
int compare(list *list1, list *list2);
//void sort();
//void sorted();
void clear();

//建立一個列表並回傳該列表的記憶體位置
list *create()
{
    list *newlist = (list *)calloc(1, sizeof(list));
    newlist->gbs = (void **)calloc(1000, sizeof(void *));
    newlist->gbcount = 0;
    newlist->gbsize = 1000;
    return newlist;
}

//在list最後端新增一個元素
void append(list *list_name, const int formate, ...)
{
    list *arg_list_ptr;
    int arg_int, *arg_int_ptr;
    char *arg_char_ptr;
    double arg_double, *arg_double_ptr;

    va_list args;
    va_start(args, formate);
    switch (formate)
    {
    case LIST_PTR:
        arg_list_ptr = va_arg(args, list *);
        break;
    case INT:
        arg_int = va_arg(args, int);
        break;
    case INT_PTR:
        arg_int_ptr = va_arg(args, int *);
        break;
    case DOUBLE:
        arg_double = va_arg(args, double);
        break;
    case DOUBLE_PTR:
        arg_double_ptr = va_arg(args, double *);
        break;
    case CHAR_PTR:
        arg_char_ptr = va_arg(args, char *);
    }
    va_end(args);

    list_struct *ptr, *new_ptr, *list_ptr;
    if (!(list_name->count)) //list_name->count == 0
    {
        new_ptr = list_name->head = list_name->tail = (list_struct *)calloc(1, sizeof(list_struct));
        switch (formate)
        {
        case LIST_PTR:
            new_ptr->element = create();
            list_ptr = arg_list_ptr->head;
            while (list_ptr) //list_ptr != 0
            {
                switch (list_ptr->formate)
                {
                case INT:
                    append((list *)new_ptr->element, INT, *(int *)list_ptr->element);
                    break;
                case DOUBLE:
                    append((list *)new_ptr->element, DOUBLE, *(double *)list_ptr->element);
                    break;
                case LIST_PTR:
                case INT_PTR:
                case DOUBLE_PTR:
                case CHAR_PTR:
                    append((list *)new_ptr->element, list_ptr->formate, list_ptr->element);
                    break;
                }
                list_ptr = list_ptr->next;
            }
            new_ptr->formate = LIST_PTR;
            break;
        case INT:
            new_ptr->element = calloc(1, sizeof(int));
            *(int *)new_ptr->element = arg_int;
            new_ptr->formate = INT;
            break;
        case INT_PTR:
            new_ptr->element = arg_int_ptr;
            new_ptr->formate = INT_PTR;
            break;
        case DOUBLE:
            new_ptr->element = calloc(1, sizeof(double));
            *(double *)new_ptr->element = arg_double;
            new_ptr->formate = DOUBLE;
            break;
        case DOUBLE_PTR:
            new_ptr->element = arg_double_ptr;
            new_ptr->formate = DOUBLE_PTR;
            break;
        case CHAR_PTR:
            new_ptr->element = calloc(strlen(arg_char_ptr), sizeof(char));
            strcpy((char *)new_ptr->element, (char *)arg_char_ptr);
            new_ptr->formate = CHAR_PTR;
        }
    }
    else
    {
        ptr = list_name->tail;
        new_ptr = (list_struct *)calloc(1, sizeof(list_struct));
        switch (formate)
        {
        case LIST_PTR:
            new_ptr->element = create();
            list_ptr = arg_list_ptr->head;
            while (list_ptr) //list_ptr != 0
            {
                switch (list_ptr->formate)
                {
                case INT:
                    append((list *)new_ptr->element, INT, *(int *)list_ptr->element);
                    break;
                case DOUBLE:
                    append((list *)new_ptr->element, DOUBLE, *(double *)list_ptr->element);
                    break;
                case LIST_PTR:
                case INT_PTR:
                case DOUBLE_PTR:
                case CHAR_PTR:
                    append((list *)new_ptr->element, list_ptr->formate, list_ptr->element);
                    break;
                }
                list_ptr = list_ptr->next;
            }
            new_ptr->formate = LIST_PTR;
            break;
        case INT:
            new_ptr->element = calloc(1, sizeof(int));
            *(int *)new_ptr->element = arg_int;
            new_ptr->formate = INT;
            break;
        case INT_PTR:
            new_ptr->element = arg_int_ptr;
            new_ptr->formate = INT_PTR;
            break;
        case DOUBLE:
            new_ptr->element = calloc(1, sizeof(double));
            *(double *)new_ptr->element = arg_double;
            new_ptr->formate = DOUBLE;
            break;
        case DOUBLE_PTR:
            new_ptr->element = arg_double_ptr;
            new_ptr->formate = DOUBLE_PTR;
            break;
        case CHAR_PTR:
            new_ptr->element = calloc(strlen(arg_char_ptr), sizeof(char));
            strcpy((char *)new_ptr->element, (char *)arg_char_ptr);
            new_ptr->formate = CHAR_PTR;
        }
        new_ptr->previous = ptr;
        ptr->next = new_ptr;
        list_name->tail = new_ptr;
    }

    list_name->count++;
}

void insert(list *list_name, int index, const int formate, ...)
{
    list *arg_list_ptr;
    int arg_int, *arg_int_ptr;
    char *arg_char_ptr;
    double arg_double, *arg_double_ptr;

    va_list args;
    va_start(args, formate);
    switch (formate)
    {
    case LIST_PTR:
        arg_list_ptr = va_arg(args, list *);
        break;
    case INT:
        arg_int = va_arg(args, int);
        break;
    case INT_PTR:
        arg_int_ptr = va_arg(args, int *);
        break;
    case DOUBLE:
        arg_double = va_arg(args, double);
        break;
    case DOUBLE_PTR:
        arg_double_ptr = va_arg(args, double *);
        break;
    case CHAR_PTR:
        arg_char_ptr = va_arg(args, char *);
    }
    va_end(args);

    list_struct *ptr, *new_ptr, *list_ptr;

    if (!(list_name->count)) //list_name->count == 0
    {
        new_ptr = list_name->head = list_name->tail = (list_struct *)calloc(1, sizeof(list_struct));
        switch (formate)
        {
        case LIST_PTR:
            new_ptr->element = create();
            list_ptr = arg_list_ptr->head;
            while (list_ptr) //list_ptr != 0
            {
                switch (list_ptr->formate)
                {
                case INT:
                    append((list *)new_ptr->element, INT, *(int *)list_ptr->element);
                    break;
                case DOUBLE:
                    append((list *)new_ptr->element, DOUBLE, *(double *)list_ptr->element);
                    break;
                case LIST_PTR:
                case INT_PTR:
                case DOUBLE_PTR:
                case CHAR_PTR:
                    append((list *)new_ptr->element, list_ptr->formate, list_ptr->element);
                    break;
                }
                list_ptr = list_ptr->next;
            }
            new_ptr->formate = LIST_PTR;
            break;
        case INT:
            new_ptr->element = calloc(1, sizeof(int));
            *(int *)new_ptr->element = arg_int;
            new_ptr->formate = INT;
            break;
        case INT_PTR:
            new_ptr->element = arg_int_ptr;
            new_ptr->formate = INT_PTR;
            break;
        case DOUBLE:
            new_ptr->element = calloc(1, sizeof(double));
            *(double *)new_ptr->element = arg_double;
            new_ptr->formate = DOUBLE;
            break;
        case DOUBLE_PTR:
            new_ptr->element = arg_double_ptr;
            new_ptr->formate = DOUBLE_PTR;
            break;
        case CHAR_PTR:
            new_ptr->element = calloc(strlen(arg_char_ptr), sizeof(char));
            strcpy((char *)new_ptr->element, (char *)arg_char_ptr);
            new_ptr->formate = CHAR_PTR;
        }
        list_name->count++;
    }
    else
    {
        new_ptr = (list_struct *)calloc(1, sizeof(list_struct));
        switch (formate)
        {
        case LIST_PTR:
            new_ptr->element = create();
            list_ptr = arg_list_ptr->head;
            while (list_ptr) //list_ptr != 0
            {
                switch (list_ptr->formate)
                {
                case INT:
                    append((list *)new_ptr->element, INT, *(int *)list_ptr->element);
                    break;
                case DOUBLE:
                    append((list *)new_ptr->element, DOUBLE, *(double *)list_ptr->element);
                    break;
                case LIST_PTR:
                case INT_PTR:
                case DOUBLE_PTR:
                case CHAR_PTR:
                    append((list *)new_ptr->element, list_ptr->formate, list_ptr->element);
                    break;
                }
                list_ptr = list_ptr->next;
            }
            new_ptr->formate = LIST_PTR;
            break;
        case INT:
            new_ptr->element = calloc(1, sizeof(int));
            *(int *)new_ptr->element = arg_int;
            new_ptr->formate = INT;
            break;
        case INT_PTR:
            new_ptr->element = arg_int_ptr;
            new_ptr->formate = INT_PTR;
            break;
        case DOUBLE:
            new_ptr->element = calloc(1, sizeof(double));
            *(double *)new_ptr->element = arg_double;
            new_ptr->formate = DOUBLE;
            break;
        case DOUBLE_PTR:
            new_ptr->element = arg_double_ptr;
            new_ptr->formate = DOUBLE_PTR;
            break;
        case CHAR_PTR:
            new_ptr->element = calloc(strlen(arg_char_ptr), sizeof(char));
            strcpy((char *)new_ptr->element, (char *)arg_char_ptr);
            new_ptr->formate = CHAR_PTR;
        }

        if (index < list_name->count)
        {

            if (index > -1)
            {
                ptr = list_name->head;
                for (int i = 0; i != index && ptr->next != 0; ++i)
                    ptr = ptr->next;
            }
            else
            {
                ptr = list_name->tail;
                for (int i = -1; i != index && ptr->previous != 0; i--)
                    ptr = ptr->previous;
            }

            if (ptr == list_name->head)
            {
                new_ptr->next = ptr;
                ptr->previous = new_ptr;
                list_name->head = new_ptr;
            }
            else
            {
                new_ptr->next = ptr;
                ptr = ptr->previous;
                ptr->next = new_ptr;
                new_ptr->previous = ptr;
                ptr = new_ptr->next;
                ptr->previous = new_ptr;
            }
            list_name->count++;
        }
        else
        {
            switch (formate)
            {
            case LIST_PTR:
                append(list_name, LIST_PTR);
                break;
            case INT:
                append(list_name, INT, arg_int);
                break;
            case INT_PTR:
                append(list_name, INT_PTR, arg_int_ptr);
                break;
            case DOUBLE:
                append(list_name, DOUBLE, arg_double);
                break;
            case DOUBLE_PTR:
                append(list_name, DOUBLE_PTR, arg_double_ptr);
                break;
            case CHAR_PTR:
                append(list_name, CHAR_PTR, arg_char_ptr);
            }
        }
    }
}

//回傳list中第index項元素之記憶體位置並將其從列表刪除 ,若不存在該項則回傳NULL且不動作,
void *pop(list *list_name, int index)
{
    if (abs(index) > list_name->count - (index > -1) || !(list_name->count)) //abs(index) > list_name->count - (index > -1) || list_name->count == 0
    {
        return NULL;
    }

    list_struct *ptr, *old_ptr;

    if (index > -1)
    {
        ptr = list_name->head;
        for (int i = 0; i != index; ++i)
            ptr = ptr->next;
    }
    else
    {
        ptr = list_name->tail;
        for (int i = -1; i != index; i--)
            ptr = ptr->previous;
    }

    void *output = ptr->element;

    if (list_name->count == 1)
    {
        old_ptr = ptr;
        list_name->head = 0;
        list_name->tail = 0;
    }
    else
    {

        if (ptr == list_name->head)
        {
            old_ptr = ptr;
            ptr = ptr->next;
            ptr->previous = 0;
            list_name->head = ptr;
        }
        else if (ptr == list_name->tail)
        {
            old_ptr = ptr;
            ptr = ptr->previous;
            ptr->next = 0;
            list_name->tail = ptr;
        }
        else
        {
            old_ptr = ptr;
            ptr = ptr->previous;
            ptr->next = old_ptr->next;
            ptr = ptr->next;
            ptr->previous = old_ptr->previous;
        }
    }

    switch (old_ptr->formate)
    {
    case INT:
    case DOUBLE:
    case CHAR_PTR:
        gbc(list_name, old_ptr->element);
        gbc(list_name, old_ptr);
        break;
    case INT_PTR:
    case DOUBLE_PTR:
        gbc(list_name, old_ptr);
        break;
    case LIST_PTR:
        clear((list *)old_ptr->element);
        gbc(list_name, old_ptr);
        break;
    }

    list_name->count--;
    return output;
}

//類似python之remove，刪除列表中符合的首項元素，若無法找到則回傳1且不動作
int delete (list *list_name, int formate, ...)
{
    if (!(list_name->count)) //list_name->count == 0
    {
        return 1;
    }

    list *arg_list_ptr;
    int arg_int, *arg_int_ptr, output;
    char *arg_char_ptr;
    double arg_double, *arg_double_ptr;

    va_list args;
    va_start(args, formate);

    list_struct *ptr, *new_ptr;
    ptr = list_name->head;

    switch (formate)
    {
    case LIST_PTR:
        arg_list_ptr = va_arg(args, list *);
        break;
    case INT:
        arg_int = va_arg(args, int);
        break;
    case INT_PTR:
        arg_int_ptr = va_arg(args, int *);
        break;
    case DOUBLE:
        arg_double = va_arg(args, double);
        break;
    case DOUBLE_PTR:
        arg_double = va_arg(args, double);
        break;
    case CHAR_PTR:
        arg_char_ptr = (va_arg(args, char *));
        break;
    }

    while (1)
    {
        if (ptr != 0)
        {
            if (ptr->formate == formate)
            {
                if (formate == LIST_PTR && compare((list *)ptr->element, arg_list_ptr))
                    break;
                if (formate == INT && *(int *)ptr->element == arg_int)
                    break;
                if (formate == INT_PTR && (int *)ptr->element == arg_int_ptr)
                    break;
                if (formate == DOUBLE && *(double *)ptr->element == arg_double)
                    break;
                if (formate == DOUBLE_PTR && (double *)ptr->element != arg_double_ptr)
                    break;
                if (formate == CHAR_PTR && strlen((char *)ptr->element) == strlen(arg_char_ptr))
                    if (!(strncmp((char *)ptr->element, arg_char_ptr, strlen(arg_char_ptr))))
                        break;
            }

            ptr = ptr->next;
        }
        else
            return 1;
    }

    if (list_name->count == 1)
    {
        list_name->head = 0;
        list_name->tail = 0;
    }
    else
    {

        if (ptr == list_name->head)
        {
            new_ptr = ptr->next;
            new_ptr->previous = 0;
            list_name->head = new_ptr;
        }
        else if (ptr == list_name->tail)
        {
            new_ptr = ptr->previous;
            new_ptr->next = 0;
            list_name->tail = new_ptr;
        }
        else
        {
            new_ptr = ptr->previous;
            new_ptr->next = ptr->next;
            new_ptr = ptr->next;
            new_ptr->previous = ptr->previous;
        }
    }

    switch (ptr->formate)
    {
    case INT:
    case DOUBLE:
    case CHAR_PTR:
        gbc(list_name, ptr->element);
        gbc(list_name, ptr);
        break;
    case INT_PTR:
    case DOUBLE_PTR:
        gbc(list_name, ptr);
        break;
    case LIST_PTR:
        clear((list *)ptr->element);
        gbc(list_name, ptr);
        break;
    }

    list_name->count--;
    return 0;
}

//搜尋該元素在列表中第一個符合的項次，若不存在則回傳-1
int search(list *list_name, int formate, ...)
{
    if (!(list_name->count)) //list_name->count == 0
    {
        return -1;
    }

    list *arg_list_ptr;
    int arg_int, *arg_int_ptr, output;
    char *arg_char_ptr;
    double arg_double, *arg_double_ptr;

    va_list args;
    va_start(args, formate);

    list_struct *ptr;
    ptr = list_name->head;

    int counter = 0;

    switch (formate)
    {
    case LIST_PTR:
        arg_list_ptr = va_arg(args, list *);
        break;
    case INT:
        arg_int = va_arg(args, int);
        break;
    case INT_PTR:
        arg_int_ptr = va_arg(args, int *);
        break;
    case DOUBLE:
        arg_double = va_arg(args, double);
        break;
    case DOUBLE_PTR:
        arg_double = va_arg(args, double);
        break;
    case CHAR_PTR:
        arg_char_ptr = (va_arg(args, char *));
        break;
    }

    while (ptr)
    {

        if (ptr->formate == formate)
        {
            if (formate == LIST_PTR && compare((list *)ptr->element, arg_list_ptr))
                return counter;
            if (formate == INT && *(int *)ptr->element == arg_int)
                return counter;
            if (formate == INT_PTR && (int *)ptr->element == arg_int_ptr)
                return counter;
            if (formate == DOUBLE && *(double *)ptr->element == arg_double)
                return counter;
            if (formate == DOUBLE_PTR && (double *)ptr->element != arg_double_ptr)
                return counter;
            if (formate == CHAR_PTR && strlen((char *)ptr->element) == strlen(arg_char_ptr))
                if (!(strncmp((char *)ptr->element, arg_char_ptr, strlen(arg_char_ptr))))
                    return counter;
        }

        ptr = ptr->next;
        ++counter;
    }

    return -1;
}

//計算該元素在列表中出現的次數
int count(list *list_name, int formate, ...)
{
    if (!(list_name->count)) //list_name->count == 0
    {
        return 0;
    }

    list *arg_list_ptr;
    int arg_int, *arg_int_ptr, output;
    char *arg_char_ptr;
    double arg_double, *arg_double_ptr;

    va_list args;
    va_start(args, formate);

    list_struct *ptr;
    ptr = list_name->head;

    int counter = 0;

    switch (formate)
    {
    case LIST_PTR:
        arg_list_ptr = va_arg(args, list *);
        break;
    case INT:
        arg_int = va_arg(args, int);
        break;
    case INT_PTR:
        arg_int_ptr = va_arg(args, int *);
        break;
    case DOUBLE:
        arg_double = va_arg(args, double);
        break;
    case DOUBLE_PTR:
        arg_double = va_arg(args, double);
        break;
    case CHAR_PTR:
        arg_char_ptr = (va_arg(args, char *));
        break;
    }

    while (ptr)
    {

        if (ptr->formate == formate)
        {
            if (formate == LIST_PTR && compare((list *)ptr->element, arg_list_ptr))
                ++counter;
            if (formate == INT && *(int *)ptr->element == arg_int)
                ++counter;
            if (formate == INT_PTR && (int *)ptr->element == arg_int_ptr)
                ++counter;
            if (formate == DOUBLE && *(double *)ptr->element == arg_double)
                ++counter;
            if (formate == DOUBLE_PTR && (double *)ptr->element != arg_double_ptr)
                ++counter;
            if (formate == CHAR_PTR && strlen((char *)ptr->element) == strlen(arg_char_ptr))
                if (!(strncmp((char *)ptr->element, arg_char_ptr, strlen(arg_char_ptr))))
                    ++counter;
        }

        ptr = ptr->next;
    }

    return counter;
}

//將整個列表反轉
int reverse(list *list_name)
{

    list_struct *ptr = list_name->head, *temp;

    while (ptr) //ptr != 0
    {
        temp = ptr->previous;
        ptr->previous = ptr->next;
        ptr->next = temp;
        ptr = ptr->previous;
    }

    temp = list_name->tail;
    list_name->tail = list_name->head;
    list_name->head = temp;
}

//取得該元素的資料型態
int get_formate(list *list_name, int index)
{
    if (abs(index) > list_name->count - (index > -1) || list_name->count == 0)
    {
        return -1;
    }

    list_struct *ptr;

    if (index > -1)
    {
        ptr = list_name->head;
        for (int i = 0; i != index; ++i)
            ptr = ptr->next;
    }
    else
    {
        ptr = list_name->tail;
        for (int i = -1; i != index; i--)
            ptr = ptr->previous;
    }

    return ptr->formate;
}

//輸出該項元素之記憶體位置，用以搭配取址符號直接存取元素，若不存在則回傳NULL
void *address(list *list_name, int index)
{
    if (abs(index) > list_name->count - (index > -1) || list_name->count == 0)
    {
        return NULL;
    }

    list_struct *ptr;

    if (index > -1)
    {
        ptr = list_name->head;
        for (int i = 0; i != index; ++i)
            ptr = ptr->next;
    }
    else
    {
        ptr = list_name->tail;
        for (int i = -1; i != index; i--)
            ptr = ptr->previous;
    }

    return ptr->element;
}

//收集不在列表中的廢棄記憶體
void gbc(list *list_name, void *ptr)
{
    list_name->gbs[list_name->gbcount] = ptr;
    list_name->gbcount++;

    //若廢棄記憶體位置儲存陣列滿了
    if (list_name->gbcount == list_name->gbsize)
    {
        list_name->gbs = (void **)realloc(list_name->gbs, ((list_name->gbsize) * 1.2) * sizeof(void *));
        list_name->gbsize *= 1.2;
    }
}

//回收所有廢棄記憶體，若列表中有指標類型的元素可能導致非預期的錯誤，請謹慎使用
void gc(list *list_name)
{
    for (int i = 0; i != list_name->gbcount; ++i)
        free(list_name->gbs[i]);
    free(list_name->gbs);
    list_name->gbs = (void **)calloc(1000, sizeof(void *));
    list_name->gbcount = 0;
    list_name->gbsize = 1000;
}

int print(list *list_name)
{
    if (list_name->count == 0)
    {
        printf("[]");
        return 0;
    }

    list_struct *ptr;
    ptr = list_name->head;

    printf("[");

    while (ptr) //ptr != NULL
    {
        switch (ptr->formate)
        {
        case LIST_PTR:
            print((list *)ptr->element);
            break;
        case INT:
            printf("%d", *(int *)ptr->element);
            break;
        case INT_PTR:
            printf("%d(INT_PTR address)", (int *)ptr->element);
            break;
        case DOUBLE:
            printf("%lf", *(double *)ptr->element);
            break;
        case DOUBLE_PTR:
            printf("%d(DOUBLE_PTR address)", (double *)ptr->element);
            break;
        case CHAR_PTR:
            printf("%s", (char *)ptr->element);
            break;
        }

        ptr = ptr->next;
        if (ptr) //ptr != NULL
            printf(", ");
    }

    printf("]");
    return 0;
}

//比較兩個列表內容是否相同，若相同回傳1，否則回傳0
int compare(list *list1, list *list2)
{
    list_struct *ptr1 = list1->head, *ptr2 = list2->head;
    while (ptr1 || ptr2) //ptr1 != 0 || ptr2 != 0
    {
        if (ptr1 && ptr2)
            if (ptr1->formate == ptr2->formate)
            {
                switch (ptr1->formate)
                {
                case INT:
                    if (*(int *)ptr1->element != *(int *)ptr2->element)
                        return 0;
                    break;
                case DOUBLE:
                    if (*(double *)ptr1->element != *(double *)ptr2->element)
                        return 0;
                    break;
                case INT_PTR:
                case DOUBLE_PTR:
                    if ((void *)ptr1->element != (void *)ptr2->element)
                        return 0;
                    break;
                case LIST_PTR:
                    if (!compare((list *)ptr1->element, (list *)ptr2->element))
                        return 0;
                    break;
                case CHAR_PTR:
                    if (strlen((char *)ptr1->element) == strlen((char *)ptr2->element))
                    {
                        if ((strncmp((char *)ptr1->element, (char *)ptr2->element, strlen((char *)ptr1->element))))
                            return 0;
                    }
                    else
                        return 0;
                    break;
                }
            }
            else
                return 0;
        else
            return 0;

        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    return 1;
}

//未完成功能
void sort()
{
}

void clear(list *list_name)
{
    list_struct *ptr = list_name->head;
    while (ptr) //ptr != 0
    {
        switch (ptr->formate)
        {
        case INT:
        case DOUBLE:
        case CHAR_PTR:
            gbc(list_name, ptr->element);
            gbc(list_name, ptr);
            break;
        case INT_PTR:
        case DOUBLE_PTR:
            gbc(list_name, ptr);
            break;
        case LIST_PTR:
            clear((list *)ptr->element);
            gbc(list_name, ptr);
            break;
        }

        ptr = ptr->next;
    }

    gc(list_name);
    free(list_name->gbs);
    free(list_name);
}