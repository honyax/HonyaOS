// string.c
int hstrlen(const char *str);
int hstrncmp(const char *s1, const char *s2, int len);
int hstrcmp(const char *s1, const char *s2);
int hstrindexof(const char *s, char c);
char htouppercase(const char c);
char htolowercase(const char c);

// sprintf.c
int hsprintf (char *str, const char *fmt, ...);
