int hstrlen(const char *str)
{
	// 文字列の長さをチェック。最大で1024文字にしておく
	for (int i = 0; i < 1024; i++) {
		if (str[i] == 0)
			return i;
	}
	
	return -1;
}
