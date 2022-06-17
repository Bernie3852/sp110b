# 以getc & putc為基礎，設計gets, puts, scanf, printf等輸入輸出函式之函式庫
### 本函式庫所有內容皆為100%原創
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
        * char *型態輸入之讀取 :
            ```C
            scanf("%s", &chars_1);
            ```
        * int型態輸入之讀取 :
            ```C
            scanf("%d", &int_1);
            ```
        * char型態之省略空白輸入讀取 :
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
                        default:            //輸入格式有誤
                            va_end(args);   //清理已使用完畢的參數
                            return 1;       //若不符合輸入規則則回傳1
                        }
                    }
                }
                va_end(args);   //清理已使用完畢的參數
                return 0;  
            }
            ```