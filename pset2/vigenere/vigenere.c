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
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!(isalpha(argv[1][i])))
        {
            printf("wrong key\n");
            return 1;
        }
    }
    int index = 0;
    
    string key = argv[1];
    int keylen = strlen(key);
    printf("plaintext: ");
    string s = get_string();
    printf("ciphertext: ");
    int shift;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isupper(s[i]))
        {
            if (index >= keylen)
            {
                index = 0;
            }
            
            if (isupper(key[index]))
            {
                shift = key[index] - 'A';
            }
            else 
            {
                shift = key[index] - 'a';
            }
            printf("%c", (s[i] - 'A' + shift ) % 26 + 'A');
            
            index++;
        }
        else if (islower(s[i])) 
        {
            if (index >= keylen)
            {
                index = 0;
            }
            if (isupper(key[index]))
            {
                shift = key[index] - 'A';
            }
            else 
            {
                shift = key[index] - 'a';
            }
            printf("%c", (s[i] - 'a' + shift ) % 26 + 'a');
            
            index++;
        }
        else
        {
            printf("%c", s[i]);
        }
    }
    printf("\n");
}