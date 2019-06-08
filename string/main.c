#include <stdio.h>
#include <stdlib.h>
#include "usr_string.h"

int main(void)
{
    char str[128] = "";
    int a = 145;
    char c = '@';
    char *s = "hoge";

    void *vp[10];
    vp[0] = &a;
    vp[1] = &c;
    vp[2] = s;

    usr_sprintf(str, "%%xxx%+03d,%c,%syyy",vp, 10);
    printf("%s\n", str);

    return 0;

}