#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cs50.h>
typedef uint8_t BYTE;

#define BLOKSIZE 512
 
int main(int argc, char *argv[])
{
    // checking correct usage 
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    char *inputFile = argv[1];
    if (inputFile == NULL)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    // open file for reading
    FILE *inputPtr = fopen(inputFile, "r");

    // check its validity
    if (inputPtr == NULL)
    {
        printf("cannot open file: %s\n", inputFile);
        return 2;
    }

    // defining the buffer array for storing data in 512 byte chunks
    unsigned char buffer[BLOKSIZE];

    // image number tracker
    int counter = 0;

    //output file pointer
    FILE *outputPtr = NULL;
    
    // allocating needed memory for the output file
    char *outputFileName = malloc(8);

    // read the bloks of 512 bytes
    while (fread(buffer, 1, BLOKSIZE, inputPtr) == BLOKSIZE)
    {
        // check if bytes indicate the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) 
        {
            // write the jpeg file names
            sprintf(outputFileName, "%03i.jpg", counter);

            //open outputPtr for writing
            outputPtr = fopen(outputFileName, "w");

            // track images
            counter++;

        }

        // check if output has been used for valid input and write the recovered block
        if (outputPtr != NULL)
        {
            fwrite(buffer, 1, BLOKSIZE, outputPtr);
        }
        
    }
    free(outputFileName);
    fclose(outputPtr);
    fclose(inputPtr);

    return 0;

}