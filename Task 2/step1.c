#include <stdio.h>

unsigned char data[100];
unsigned int table[256];


void make_table(void)
{
    unsigned int n, c;
    int k;
    for (n = 0; n < 256; n++) {
        c = n;
        for (k = 0; k < 8; k++) {
            if (c & 1) c = 0xEDB88320u ^ (c >> 1);
            else       c = c >> 1;
        }
        table[n] = c;
    }
}

unsigned int crc(unsigned char *buf, int len)
{
    unsigned int c = 0xFFFFFFFFu;
    int i;
    for (i = 0; i < len; i++)
        c = table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

int main(void)
{
    unsigned char buf[17];
    unsigned int stored, w, h;

    /* read the first 33 bytes of the file (the whole header) */
    FILE *f = fopen("challenge.png", "rb");
    if (f == NULL) { printf("Cannot open file\n"); return 1; }
    fread(data, 1, 33, f);
    fclose(f);

    /* the saved fingerprint is bytes 29 to 32 */
    stored = data[29] * 16777216u + data[30] * 65536u
           + data[31] * 256u + data[32];
    printf("Saved fingerprint in file: %08x\n", stored);

    make_table();

    /* the 17 bytes the fingerprint covers: "IHDR" + 13 data bytes */
    buf[0] = 'I'; buf[1] = 'H'; buf[2] = 'D'; buf[3] = 'R';
    buf[12] = data[24];   /* bit depth  */
    buf[13] = data[25];   /* color type */
    buf[14] = data[26];
    buf[15] = data[27];
    buf[16] = data[28];

    /* try every width and height, see which one gives the saved fingerprint */
    for (w = 1; w < 2000; w++) {
        for (h = 1; h < 2000; h++) {
            buf[4] = w / 16777216u;  buf[5] = (w / 65536u) % 256;
            buf[6] = (w / 256u) % 256; buf[7] = w % 256;
            buf[8] = h / 16777216u;  buf[9] = (h / 65536u) % 256;
            buf[10] = (h / 256u) % 256; buf[11] = h % 256;

            if (crc(buf, 17) == stored)
                printf("MATCH: width = %u, height = %u\n", w, h);
        }
    }
    return 0;
}