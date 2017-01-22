#include <stdio.h>
#include<stdlib.h>

void swap(int *x, int *y);

typedef struct node
{
    int n;
    struct node *ptr;
}
node;

int main(void)
{
    int *x;
    int *y;
    node *root = malloc(sizeof(node));
    node *trav = root;
    if (trav -> ptr == NULL)
    {
        printf("YOOO NULLL");
    }
    x = malloc(sizeof(int));
    y = malloc(sizeof(int));
    *x = 5;
    *y = 6;
    swap(x, y);
    printf("%i, %i\n", *x, *y);
    printf("%lu\n", sizeof(char *));
    char *filename = malloc(sizeof(char *));
    //sprintf(filename, "%03i.jpg", 5);
   // FILE *img = fopen(filename, "w");
    int *buffer = malloc(sizeof(int));
    *buffer = 10;
    fwrite(buffer, 1, 1, img);
    free(x);
    free(y);
    
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}