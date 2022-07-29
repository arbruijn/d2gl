#include <unistd.h>
#include <sys/syscall.h>
#include <asm/ldt.h>

int next_ldt = 0;

int ldt_len[8192];

int ldt_update(int n, void *base, int len) {
    struct user_desc ldt_entry;
    ldt_entry.entry_number = n;
    ldt_entry.base_addr = (unsigned long)base;
    ldt_entry.limit = len;
    ldt_entry.seg_32bit = 0x1;
    ldt_entry.contents = 0x0;
    ldt_entry.read_exec_only = 0x0;
    ldt_entry.limit_in_pages = 0x0;
    ldt_entry.seg_not_present = 0x0;
    ldt_entry.useable = 0x1;
    if (syscall( __NR_modify_ldt, 1, &ldt_entry, sizeof(ldt_entry)) == -1)
        return -1;
    ldt_len[n] = len;
    return 0;
}

int ldt_create(void *base, int len) {
    int n = next_ldt;

    if (ldt_update(n, base, len) == -1)
        return -1;

    next_ldt++;

    return (n << 3) | 7;
}

int ldt_update_base(int sel, void *base) {
    return ldt_update(sel >> 3, base, ldt_len[sel >> 3]);
}
