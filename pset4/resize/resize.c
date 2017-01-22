/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    int fact = atoi(argv[1]);
    if (argc != 4 || fact > 100 || fact < 0)
    {
        fprintf(stderr, "Usage: ./copy n infile outfile and also check the factor\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // remember original padding,width,height
    int biHeight = abs(bi.biHeight);
    int biWidth = bi.biWidth;
    
    int origP =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
	

    // change the width and height by simply multiply fact
    bi.biWidth *= fact;
    bi.biHeight *= fact;
    
    // new padding
    int newP =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // change the bisizeimage...
    bi.biSizeImage  = ((sizeof(RGBTRIPLE) * bi.biWidth) + newP) * abs(bi.biHeight);
    
    // change the bf size
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < biHeight; i++)
    {
        for (int l = 0; l < fact; l++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                for (int p = 0; p < fact; p++)
                {
                    // write rows wise reiszed
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            
            }
            
            // add the padding new
            for (int k = 0; k < newP; k++)
            {
                fputc(0x00, outptr);
            }
            // skip over input file padding and go back to start 
            fseek(inptr, -(biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            
            
        }
        // put the cursor in next right place...go to second line start
        fseek(inptr, ((biWidth) * sizeof(RGBTRIPLE) + origP), SEEK_CUR);
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
