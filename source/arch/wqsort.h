static void swap(char *a, char *b, size_t s) {
    int *ia = (void *)a, *ib = (void *)b;
    while (s >= 4) {
        int t = *ia;
        *ia++ = *ib;
        *ib++ = t;
        s -= 4;
    }
    a = (void *)ia; b = (void *)ib;
    while (s) {
        char t = *a;
        *a++ = *b;
        *b++ = t;
        s--;
    }
}

static void wqsort(void *arr, size_t num, size_t size, int (*fun)(const void *,const void *)) {
    char *base = arr;
    char *base_stack[32], *baserest;
    size_t num_stack[32], numrest;
    int i, j, jd;
    int sp = 0;
    for (;;) {
        if (num == 2) {
            if (fun(base, base + size) > 0)
                swap(base, base + size, size);
        } else if (num > 2) {
            swap(base, base + (num >> 1) * size, size);
            j = 0;
            jd = 0;
            for (i = 1; i < num; i++) {
                int c = fun(base + i * size, base);
                if (c <= 0) {
                    j++;
                    if (j != i)
                        swap(base + i * size, base + j * size, size);
                }
                if (c)
                    jd = j;
            }
            if (j != num - 1 || jd) {
                if (j)
                    swap(base, base + j * size, size);
                numrest = num - (j + 1);
                baserest = base + (j + 1) * size;
                if (j != jd)
                    j = jd + 1;
                if (j < numrest) {
                    base_stack[sp] = baserest;
                    num_stack[sp] = numrest;
                    num = j;
                } else {
                    base_stack[sp] = base;
                    num_stack[sp] = j;
                    base = baserest;
                    num = numrest;
                }
                sp++;
                continue;
            }
        }
        if (!sp)
            break;
        sp--;
        base = base_stack[sp];
        num = num_stack[sp];
    }
}
