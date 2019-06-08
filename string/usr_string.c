#include "usr_string.h"

#ifdef DEBUG
#include <stdio.h>
#endif

#define BUFFER_SIZE  ( 8 )

/* 書式指定子構造体 */
typedef struct{
    int len;    /* 書式指定子の長さ */
    int sign;   /* 符号文字の有無 */
    char padding;   /* パディング文字 */
    int min_width;  /* 最小フィールド幅 */
    char conv;  /* 変換指定子 */
} stFmtSpec;

const char *ParseFormatModifier(const char *format, stFmtSpec *spec);

/* 文字列を連結する */
char *usr_strncat(char *str1, const char *str2, size_t n)
{
    int i;
    char *src = str1;

    while(*str1 != '\0'){
        str1++;
    }

    for( i = 0; *str2 != '\0' && i < n; i++){
        *str1 = *str2;
        str1++;
        str2++;
    }

    *str1 = '\0';

    return src;
}

/* 文字列の長さを返す */
int usr_strlen(const char *str)
{
    int len;

    for( len = 0; *str != '\0'; len++){
        str++;
    }

    return len;
}

/* 書式文字列に従って文字列配列に書き込む */
int usr_sprintf(char *str, const char *format, void *argv[], size_t n)
{
    int len;
    int i;
    int tlen;
    char tmp[BUFFER_SIZE];

    stFmtSpec spec;

    for (len = 0, i = 0; *format != '\0' && i < n; len++){
        switch(*format){
            case '%':  /* 書式指定子 */
                format++;
                ParseFormatModifier(format, &spec);
                format += spec.len;
                switch(spec.conv){
                    case 'd':   /* 10進整数 */
                        if(spec.sign == 1){
                            if( *(int *)*argv >= 0 ){
                                *str = '+';
                                spec.min_width--;
                                str++;
                            }
                        }
                        itoa(*(int *)*argv, tmp, 10);
                        //itoa(*(int *)*argv, tmp, spec.min_width, spec.padding);
                        #if 0
                        for(int j = 0; usr_strlen(tmp) + j < spec.min_width; j++){
                            *str = spec.padding;
                            str++;
                        }
                        #endif
                        usr_strncat(str, tmp, BUFFER_SIZE);
                        str += usr_strlen(tmp);
                        argv++;
                        break;
                    case 'c':   /* 文字 */
                        *str = *(char *)*argv;
                        str++;
                        argv++;
                        break;
                    case 's':   /* 文字列 */
                        tlen = usr_strlen((char *)*argv);
                        usr_strncat(str, (char *)*argv, tlen);
                        str += tlen;
                        argv++;
                        break;
                    case '%':   /* '%' */
                        *str = '%';
                        str++;
                        break;
                    default:
                        break;
                }
                break;
            default:    /* そのまま出力 */
                *str = *format;
                str++;
                format++;
                break;
        }
        *str = '\0';
    }

    return len;
}

/* 書式指定子解析 */
const char *ParseFormatModifier(const char *format, stFmtSpec *spec)
{
    int loop;
    int i;
    char tmp[BUFFER_SIZE];

    /* 指定子情報の初期化 */
    spec->len = 0;
    spec->sign = 0;
    spec->min_width = 0;
    spec->padding = ' ';
    spec->conv = 'c';

    /* フラグ解析 */
    loop = 1;
    while(loop){
        switch (*format)
        {
            case '+':   /* 符号付き */
                spec->sign = 1;
                spec->len++;
                format++;
                break;
            case '0':   /* 0埋め */
                spec->padding = '0';
                spec->len++;
                format++;
                break;
            default:
                loop = 0;
                break;
        }
    }

    /* 最小フィールド幅解析 */
    for(i = 0, loop = 1; i < BUFFER_SIZE && loop ; i++){
        if(*format >= '0' && *format <= '9'){
            tmp[i] = *format;
            spec->len++;
            format++;
        }else{
            loop = 0;
        }
    }
    spec->min_width = atoi(tmp);

    /* 変換指定子解析 */
    spec->conv = *format;
    spec->len++;

#ifdef DEBUG
    printf("\n[%c]\n", spec->conv);
#endif

    return format;
}
