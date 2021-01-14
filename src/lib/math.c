int hsqrt(int s)
{
    if (s < 1) {
        return 0;
    } else if (s == 1) {
        return 1;
    }

    int x = s / 2;
    int last_x = 0;

    while (x != 0 && x != last_x) {
        last_x = x;
        x = (x + s / x) / 2;
    }

    return x;
}
