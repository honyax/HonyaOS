#define SYSCALL_ID_EXAMPLE              0xFFFF
#define SYSCALL_ID_PUTCHAR              1
#define SYSCALL_ID_BG_DRAW_TEXT         2
#define SYSCALL_ID_GET_CURRENT_TIME     3

// asm_syscall.s
int _sc_example(int param1, int param2, int param3, int param4, int param5);
void _sc_putchar(int c);
void _sc_bg_draw_text(int x, int y, char *text, int color);
int _sc_get_current_time();
