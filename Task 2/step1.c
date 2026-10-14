#include <stdio.h>

int buf[1000000];

int main(void)
{
    FILE *fp;
    int c, n, i;

    /* read challenge.png one byte at a time into buf */
    fp = fopen("challenge.png", "rb");
    if (fp == NULL) {
        printf("could not open challenge.png\n");
        return 1;
    }
    n = 0;
    c = fgetc(fp);
    while (c != EOF) {
        buf[n] = c;
        n++;
        c = fgetc(fp);
    }
    fclose(fp);

    /* height is stored in bytes 20 to 23 */
    printf("old height = %d\n",
           buf[20] * 16777216 + buf[21] * 65536 + buf[22] * 256 + buf[23]);

    /* change it to 850 */
    buf[20] = 0;
    buf[21] = 0;
    buf[22] = 850 / 256;
    buf[23] = 850 % 256;
    printf("new height = %d\n",
           buf[20] * 16777216 + buf[21] * 65536 + buf[22] * 256 + buf[23]);

    /* write everything to step1.png one byte at a time */
    fp = fopen("step1.png", "wb");
    for (i = 0; i < n; i++)
        fputc(buf[i], fp);
    fclose(fp);
    printf("saved step1.png\n");
    return 0;
}
