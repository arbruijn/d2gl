#ifndef __WATCOMC__
void itoa(int n, char *ret, int base) {
    char buf[10], *p = buf;
    if (n < 0) {
        *ret++ = '-';
        n = -n;
    }
    do {
        int nn = n / base;
        int c = n - nn * base;
        *p++ = c < 10 ? c + '0' : c - 10 + 'a';
        n = nn;
    } while (n);
    while (p > buf)
        *ret++ = *--p;
    *ret = 0;
}
#endif
