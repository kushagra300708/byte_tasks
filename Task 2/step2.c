#include <stdio.h>

unsigned char buf[1000000];

int main()
{
    FILE *fp;
    int n, i;
    int pos = -1;

    fp = fopen("step1.png", "rb");
    if (fp == NULL) {
        printf("could not open step1.png\n");
        return 1;
    }
    n = fread(buf, 1, 1000000, fp);
    fclose(fp);
    printf("size = %d\n", n);

    /* look for the letters IEND, the real one is at the very end */
    for (i = 4; i + 8 <= n; i++) {
        if (buf[i] == 'I' && buf[i+1] == 'E' && buf[i+2] == 'N' && buf[i+3] == 'D') {
            printf("IEND letters at %d\n", i);
            /* if the chunk does not end the file, it is the fake one */
            if (i + 8 < n && pos == -1)
                pos = i - 4;   
        }
    }

    if (pos == -1) {
        printf("no fake IEND found\n");
        return 0;
    }
    printf("fake chunk starts at %d\n", pos);

    /* remove the 12 bytes by moving everything after them back */
    for (i = pos; i < n - 12; i++)
        buf[i] = buf[i + 12];
    n = n - 12;

    fp = fopen("step2.png", "wb");
    fwrite(buf, 1, n, fp);
    fclose(fp);
    printf("saved step2.png, size = %d\n", n);
    return 0;
}
