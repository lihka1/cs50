#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("wrong no of arguments\n");
        return 1;
    }
    int key = atoi(argv[1]);
    key = key % 26;
    printf("plaintext: ");
    string s = get_string();
    printf("ciphertext: ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isupper(s[i]))
        {
            printf("%c", (s[i] - 'A' + key ) % 26 + 'A');
        }
        else if (islower(s[i])) 
        {
            printf("%c", (s[i] - 'a' + key ) % 26 + 'a');
        }
        else
        {
            printf("%c", s[i]);
        }
    }
    printf("\n");
}