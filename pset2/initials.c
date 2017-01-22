#include <stdio.h> 
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    bool flag = true;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if ( ('a' <= s[i] && s[i] <= 'z' ) || ('A' <= s[i] && s[i] <= 'Z' ) )
        {
            if (flag)
            {
                printf("%c", toupper(s[i]));
                flag = false;
            }
        }
        else if (s[i] == ' ' && i > 0)
        {
            flag = true;
        }
    }
    printf("\n");
}