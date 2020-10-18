int kernel_main();

// カーネルの先頭に自動的に配置されるよう、ソースファイル名を _.c にした。
// 0x10300にジャンプした際に、以下の関数が呼ばれる
int os_main()
{
    return kernel_main();
}
