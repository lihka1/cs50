#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Enter a non-negative integer < 24: ");
    int height = get_int();
    while (height > 23 || height < 0)
    {
        printf("Enter a non-negative integer < 24: ");
        height = get_int();
    }
    for (int i = 0; i < height; i++)
    {
        for (int k = height-1; k > i; k--)
        {
            printf(" ");
        }
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        
        printf("\n");
    }
        
        
}
