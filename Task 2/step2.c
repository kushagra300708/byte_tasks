#include <stdio.h>

#define MAX_SIZE 2000000

unsigned char data[MAX_SIZE];

int main(void)
{
    int size, i, j;
    int count = 0;          
    int fake_start = -1;    
    /* putting step1.png into the array */
    FILE *in = fopen("step1.png", "rb");
    if (in == NULL) {
        printf("Cannot open step1.png\n");
        return 1;
    }
    size = fread(data, 1, MAX_SIZE, in);
    fclose(in);
    printf("File size: %d bytes\n\n", size);

    /* look through the file for the letters I E N D */
    for (i = 4; i + 8 <= size; i++) {
        if (data[i] == 'I' && data[i+1] == 'E' &&
            data[i+2] == 'N' && data[i+3] == 'D') {

            count++;
            printf("Found IEND letters at offset %d (chunk starts at %d)\n",
                   i, i - 4);

            /* A real IEND chunk is always these 12 bytes:
               00 00 00 00  I E N D  AE 42 60 82
               If it matches AND more file follows it, it is fake. */
            if (data[i-4] == 0 && data[i-3] == 0 &&
                data[i-2] == 0 && data[i-1] == 0 &&
                data[i+4] == 0xAE && data[i+5] == 0x42 &&
                data[i+6] == 0x60 && data[i+7] == 0x82 &&
                i + 8 < size && fake_start == -1) {

                fake_start = i - 4;
            }
        }
    }
    printf("Total IEND found: %d (a healthy PNG has exactly 1)\n\n", count);

    /* \delete the fake 12 bytes\*/
    if (fake_start != -1) {
        printf("Fake IEND found at offset %d, removing 12 bytes\n", fake_start);
        for (j = fake_start; j + 12 < size; j++) {
            data[j] = data[j + 12];
        }
        size = size - 12;
    } else {
        printf("No fake IEND found, nothing to remove\n");
    }

    /*save as step2.png */
    FILE *out = fopen("step2.png", "wb");
    if (out == NULL) {
        printf("Cannot create step2.png\n");
        return 1;
    }
    fwrite(data, 1, size, out);
    fclose(out);
    printf("\nSaved step2.png (%d bytes)\n", size);
    return 0;
}