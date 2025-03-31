#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define BLOCKS 512

// header of JPEG file
#define FIRSTBIT 0xff
#define SECONDBIT 0xd8
#define THIRDBIT 0xff
#define FOURTHBITMIN 223
#define FOURTHBITMAX 240

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover card\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    uint8_t buffer[BLOCKS];                      // to read blocks of 8 bits

    int nfiles = -1;
    FILE *jpeg = NULL;
    while (fread(buffer, 1, BLOCKS, card) == BLOCKS)
    {
        if (buffer[0] == FIRSTBIT && buffer[1] == SECONDBIT && buffer[2] == THIRDBIT && buffer[3] > FOURTHBITMIN && buffer[3] < FOURTHBITMAX){
            nfiles += 1;

            if (jpeg != NULL)
            {
                fclose(jpeg);
            }

            char filename[8];
            sprintf(filename, "%03i.jpg", nfiles);
            jpeg = fopen(filename, "w");
        }
        if (jpeg != NULL)
            {
                fwrite(buffer, 1, BLOCKS, jpeg);
            }
    }
    fclose(jpeg);
    fclose(card);
    return 0;
}
