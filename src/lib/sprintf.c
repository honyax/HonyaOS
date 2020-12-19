#include <stdarg.h>

//10進数からASCIIコードに変換
int dec2asc (char *str, int dec)
{
    int len = 0, len_buf; //桁数
    int minus = 0;
    int buf[10];
    if (dec < 0) {
        *(str++) = '-';
        dec = -dec;
        minus = 1;
    }
    while (1) { //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
        buf[len++] = dec % 10;
        if (dec < 10) break;
        dec /= 10;
    }
    len_buf = len;
    while (len) {
        *(str++) = buf[--len] + 0x30;
    }
    return len_buf + minus;
}
 
//16進数からASCIIコードに変換
int hex2asc (char *str, int dec, char alphabet_start)
{
    //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
    int len = 0, len_buf; //桁数
    int buf[10];
    while (1) {
        buf[len++] = dec % 16;
        if (dec < 16) break;
        dec /= 16;
    }
    len_buf = len;
    while (len) {
        len --;
        *(str++) = (buf[len]<10)?(buf[len] + 0x30):(buf[len] - 10 + alphabet_start);
    }
    return len_buf;
}

int str2asc (char *str, const char *arg)
{
    // argの文字列をstrにコピー
    int len = 0;
    for (len = 0; len < 64; len++) {
        if (*arg == 0x00)
            break;
        *(str++) = *(arg++);
    }
    return len;
}

int hsprintf (char *str, const char *fmt, ...)
{
    va_list list;
    int count = 0;
    int len;
    va_start (list, fmt);
 
    while (*fmt) {
        if(*fmt=='%') {
            fmt++;
            switch(*fmt){
                case 'd':
                    len = dec2asc(str, va_arg (list, int));
                    break;
                case 'x':
                    len = hex2asc(str, va_arg (list, int), 'a');
                    break;
                case 'X':
                    len = hex2asc(str, va_arg (list, int), 'A');
                    break;
                case 'c':
                    len = 1;
                    *str = va_arg(list, int);
                    break;
                case 's':
                    len = str2asc(str, va_arg (list, char*));
                    break;
                default:
                    len = 1;
                    *str = *fmt;
                    break;
            }
            str += len; fmt++;
            count += len;
        } else {
            *(str++) = *(fmt++);
            count++;
        }   
    }
    *str = 0x00; //最後にNULLを追加
    va_end (list);

    return count;
}
