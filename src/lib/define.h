#pragma once

// 共通
#define     NULL            0
#define     TRUE            1
#define     FALSE           0
typedef     unsigned char   bool;
typedef     unsigned char   byte;
typedef     unsigned short  ushort;
typedef     unsigned int    uint;

// カラーコード
#define     COL_000000      0
#define     COL_0000A8      1
#define     COL_00A800      2
#define     COL_00A8A8      3
#define     COL_A80000      4
#define     COL_A800A8      5
#define     COL_A85400      6
#define     COL_A8A8A8      7
#define     COL_545454      8
#define     COL_5454FC      9
#define     COL_54FC54      10
#define     COL_54FCFC      11
#define     COL_FC5454      12
#define     COL_FC54FC      13
#define     COL_FCFC54      14
#define     COL_FCFCFC      15
#define     COL_BLACK       COL_000000
#define     COL_DARKBLUE    COL_0000A8
#define     COL_DARKGREEN   COL_00A800
#define     COL_DARKCYAN    COL_00A8A8
#define     COL_DARKRED     COL_A80000
#define     COL_DARKPURPLE  COL_A800A8
#define     COL_BROWN       COL_A85400
#define     COL_GREY        COL_A8A8A8
#define     COL_DARKGREY    COL_545454
#define     COL_BLUE        COL_5454FC
#define     COL_GREEN       COL_54FC54
#define     COL_CYAN        COL_54FCFC
#define     COL_RED         COL_FC5454
#define     COL_PURPLE      COL_FC54FC
#define     COL_YELLOW      COL_FCFC54
#define     COL_WHITE       COL_FCFCFC
#define     COL_NONE        255

typedef struct
{
    int x, y;
    int w, h;
} RECT;

// string.c
int hstrlen(const char *str);
int hstrncmp(const char *s1, const char *s2, int len);
int hstrcmp(const char *s1, const char *s2);
int hstrindexof(const char *s, char c);
char htouppercase(const char c);
char htolowercase(const char c);

// sprintf.c
int hsprintf (char *str, const char *fmt, ...);
