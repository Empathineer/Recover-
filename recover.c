#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <math.h>

typedef uint8_t BYTE;

#define BLOCK_SIZE 512
#define FILE_NAME_SIZE 8


bool isStartJpg(BYTE buffer[])
{
    return (buffer [0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0);
}

void checkPtr(FILE *file_ptr)
{
    if (file_ptr == NULL)
    {
        fclose(file_ptr);
    }
}

int main(int argc, char *argv[])
{
    //Check that there is only one cmd line argument
    if (argc != 2)
    {
        printf("Usage: recover image\n");
        return 1;
    }

    //store file name
    char *inputFile = argv[1];

    if (inputFile == NULL)
    {
        printf("File not found.\n");
        return 1;
    }

    FILE *p_in = fopen(inputFile, "r"); //open and read input file via ptr

    if (p_in == NULL) //check if input ptr is null
    {
        // printf("Cannot open file. %s\n", inputFile);
        // return 1;
        fprintf(stderr, "Could not open %s.\n", inputFile);
        return 1;
    }


    //create type BYTE ptr to where read data will be stored
    BYTE buffer[BLOCK_SIZE];
    char filename[FILE_NAME_SIZE];
    int num_jpg = 0;
    // uint8_t [BLOCK_SIZE];
    bool is_first_jpeg = false;
    FILE *p_out = NULL;

    while (fread(buffer, BLOCK_SIZE, 1, p_in) || feof(p_in) == 0)
    {
        if (isStartJpg(buffer))
        {
            //if the file ptr is already being used, close it.
            if(p_out != NULL)
            {
                fclose(p_out); //closes the stream or ptr to the file object
            }
            sprintf(filename, "%03i.jpg", num_jpg);
            p_out = fopen(filename, "w");
            num_jpg++;
        }

        if (p_out != NULL)
        {
            // fwrite(buffer, BLOCK_SIZE, 1, p_out);
            fwrite(buffer, BLOCK_SIZE, 1, p_out);

        }
        // fclose(p_out);
        // fclose(p_in);
    }

    fclose(p_out);
    fclose(p_in);
    return 0;
}
