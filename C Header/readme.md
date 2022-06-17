# 以getc & putc為基礎，設計gets, puts, scanf, printf等輸入輸出函式之函式庫
### 本函式庫所有內容皆為100%原創

test.c為測試用範例程式

本專案目標在於重現以上四種函式提供之功能，目前已經達成之目標如下 :
* gets() :
    * 完成最基礎讀取單行輸入之gets()函式
* puts() :
    * 完成最基礎輸出完整字串之puts()函式
* scanf() :
    * 一次讀取一種輸入類型之輸入，並將其正確儲存 :
        * char型態之輸入讀取 :
            ```C
            scanf("%c", &char_1);
            ```
            實作 :
            ```C
            temp = (char *)va_arg(args, char *);    //要儲存的記憶體位置
            *(char *)temp = getc(stdin);
            ```
        * char *型態輸入之讀取 :
            ```C
            scanf("%s", &chars_1);
            ```
            實作 :
            ```C
            temp = (char *)va_arg(args, char *);    //要儲存的記憶體位置
            _gets_skip_blank((char *)temp);     //會省略字串前空格且以空格截斷版本之gets()
            ```
        * int型態輸入之讀取 :
            ```C
            scanf("%d", &int_1);
            ```
            實作 :
            ```C
            temp = (int *)va_arg(args, int *);      //要儲存的記憶體位置
            char inputs[13] = {};               //數字暫存
            int index = _gets_number(inputs);       //會省略字串前空格且以非數字輸入截斷版本之gets()
            if (index == -1)                    //錯誤的輸入格式處理, 以免記憶體錯誤
                return 1;
            *(int *)temp = words_to_int(inputs, index); //將char *型態數字轉為int型態之函式
            break;
            ```
        * char型態之省略空白輸入版本讀取 :
            ```C
            scanf(" %c", &char_1);
            ```
            實作 :
            ```C
            //其中temp為欲儲存的記憶體位置，requires為使用者輸入之讀取格式之字串
            if (requires[i - 2] == ' ') //若%c前面有空格，省略字串前面的任何空格, Tab鍵, enter鍵
            {
                while (input == ' ' || input == '\t' || input == '\n')
                    input = getc(stdin);
                *(char *)temp = input;
            }
            else
                *(char *)temp = getc(stdin);
            ```
        * char *型態 & int型態輸入之省略空白輸入讀取 :
            實作方式類似char型態
    * 一次讀取多種輸入類型之輸入 :
        * 使用stdarg達成不定參數輸入:
            ```C
            scanf("%c%d%s", &char_1, &int_1, chars_1);
            scanf("%d", &int_2);
            ```
            實作:
            ```C
            int _scanf(const char requires[], ...)
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
                            //char實作
                        case 's':
                            //char * 實作
                        case 'd':
                            int 實作
                        default:        //輸入格式有誤
                            va_end(args);   //清理已使用完畢的參數
                            return 1;       //若不符合輸入規則則回傳1
                        }
                    }
                }
                va_end(args);   //清理已使用完畢的參數
                return 0;  
            }
            ```
        * 解決int型態輸入之特性
            在int型態輸入有一特性，如果讀取數字時讀到非數字，會自動截斷
            例如 :
            ```C
            _scanf("%d%s", &int_1, chars_1);
            //輸入 "123test"
            printf("%d%s", int_1, chars_1):
            //輸出為 "123est"
            ```
            由於%d預先讀到了't'  
            輸入流剩餘之內容為"est"  
            造成後續輸入錯誤  
            * 解決方法 :  
                在header裡面新增一全域變數INPUT_BUFF  
                並在每次讀取前先檢查該全域變數是否有內容  
                %c範例實作 :  
                ```C
                if (INPUT_BUFF) //若暫存區內有已讀取的字元
                {
                    input = INPUT_BUFF;
                    INPUT_BUFF = 0;
                }
                ```
                在%s, %d也都有對應檢查之實作  
* printf():
    * 僅有單純字串之輸出 :  
        例如 :
        ```C
        printf("input words");
        ```
        基本上與puts()幾乎相同  
        實作 :
        ```C
        va_list args; //宣告參數列表
        va_start(args, requires); //讀入第一個參數
        for (int i = 0; requires[i] != '\0'; ++i)
            putc(requires[i], stdout);
        va_end(args); //清理已使用完畢的參數
        ```
    * 包含插入變數之輸出 :  
        例如 :
        ```C
        _scanf("%s %d %c %d", chars_1, &int_1, &char_1, &int_2);
        //輸入 "is 10 : 30"
        printf("It %s %d%c%d AM", chars_1, int_1, char_1, int_2);
        //輸出為 "It is 10:30 AM"
        ```
        實作 :
        ```C
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
                    _puts(temp);    //直接沿用_puts()輸出字串
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
            else
                putc(requires[i], stdout);
        }
    * 包含跳脫字元之輸出 :
        例如 :
        ```C
        _scanf("%d %c %d", &int_1, &char_1, &int_2);
        //輸入 "10 : 30"
        printf("It\' %d%c%d AM \n Cool", int_1, char_1, int_2);
        /*
        輸出為
        "It is 10:30 AM
         Cool"
        */
        ```
        實作 :
        ```C
        for (int i = 0; requires[i] != '\0'; ++i)
        {
            if (requires[i] == '%')
            {
                //%實作
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
        ```
