int hstrlen(const char *str)
{
	// 文字列の長さをチェック。最大で1024文字にしておく
	for (int i = 0; i < 1024; i++) {
		if (str[i] == 0) {
			return i;
        }
	}
	
	return -1;
}

int hstrncmp(const char *s1, const char *s2, int len)
{
    for (int i = 0; i < len; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }

    return 0;
}

int hstrcmp(const char *s1, const char *s2)
{
    int len = hstrlen(s1);
    int len2 = hstrlen(s2);

    if (len != len2) {
        return len - len2;
    }

    return hstrncmp(s1, s2, len);
}

int hstrindexof(const char *s, char c)
{
    int len = hstrlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] == c)
            return i;
    }

    return -1;
}
