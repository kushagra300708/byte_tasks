#include <stdio.h>

#define MAX_SIZE 2000000

unsigned char data[MAX_SIZE];

int main(void)
{
    int size, height;

    /* put the whole file into the array */
    FILE *in = fopen("challenge.png", "rb");
    if (in == NULL) {
        printf("Cannot open challenge.png\n");
        return 1;
    }
    size = fread(data, 1, MAX_SIZE, in);
    fclose(in);

    /* read the height (bytes 20 to 23) */
    height = data[20] * 16777216 + data[21] * 65536
           + data[22] * 256 + data[23];
    printf("Height in header: %d\n", height);

    /* change it to 850 */
    data[20] = 0;
    data[21] = 0;
    data[22] = 850 / 256;      /* 3  */
    data[23] = 850 % 256;      /* 82 */

    height = data[20] * 16777216 + data[21] * 65536
           + data[22] * 256 + data[23];
    printf("Height now: %d\n", height);

    /* save as a new file */
    FILE *out = fopen("step1.png", "wb");
    fwrite(data, 1, size, out);
    fclose(out);
    printf("Saved step1.png\n");
    return 0;
}