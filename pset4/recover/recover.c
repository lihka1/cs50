#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./recover image\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        fprintf(stderr, "file cannot be opened or not exits: %s\n", argv[1]);
        return 2;
    }
    // why uint8_t??
    uint8_t *buffer = malloc(512);
    
    // remember whether writing or not
    int writing = 0;
    
    // remember no of jpgs
    int count = 0;
    
    // i dont know why size of(char *) not works in spellings ..but OKhere....
    // but better to 
    // initialization use char *filename[maxsize];
    char *filename = malloc(sizeof(char *));
    FILE *img;
    
    while ((fread(buffer, 512, 1, f) > 0))
    {
        
        
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            
            if (writing == 1)
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", count);
                count++;
                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                writing = 1;
            }
            else
            {
                sprintf(filename, "%03i.jpg", count);
                count++;
                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                writing = 1;
            }
            
        }
        else
        {
             if (writing == 1)
            {
                fwrite(buffer, 1, 512, img);
                writing = 1;
            }
        }
    }
    
    // close everything and free every thing ..no valgring errors
    fclose(img);
    fclose(f);
    free(buffer);
    free(filename);
    
    
    
}