int hsqrt(int s)
{
    if (s < 1) {
        return 0;
    } else if (s == 1) {
        return 1;
    }

    int x = s / 2;
    int last_x = 0;
    int last_x_2 = 0;

    for (int i = 0; x != 0 && x != last_x && x != last_x_2 && i < 100; i++) {
        last_x_2 = last_x;
        last_x = x;
        x = (x + s / x) / 2;
    }

    return x;
}
