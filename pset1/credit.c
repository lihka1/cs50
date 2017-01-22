#include <stdio.h>
#include <cs50.h> 

bool validNumDigits(long long num);
int getSum(long long num);

int main(void)
{
    printf("Number: ");
    long long num = get_long_long();
    
    int total = 0;
    if (validNumDigits(num))
    {
        total = getSum(num);
        if (total % 10 == 0)
        {
            while (num > 100)
            {
                num = num/10;
            }
            if (num == 34 || num == 37)
            {
                printf("AMEX\n");
            }
            else if (num > 50 && num < 56)
            {
                printf("MASTERCARD\n");
            }
            else if ( (num/10) == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
        
    }
    else
    {
        printf("INVALID\n");
    }
}

bool validNumDigits(long long num)
{
    int count = 0;
    while (num > 0)
    {
        count++;
        num = num/10;
    }
    if (count == 15 || count == 16 || count == 13)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int getSum(long long num)
{
    int sum = 0;
    int count = 1;
    while (num > 0)
    {
        if (count % 2 == 0)
        {
            int rem = num % 10;
            switch(rem)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                    sum += (rem * 2);
                    break;
                case 5:
                    sum += 1;
                    break;
                case 6:
                    sum += 3;
                    break;
                case 7:
                    sum += 5;
                    break;
                case 8:
                    sum += 7;
                    break;
                case 9:
                    sum += 9;
                    break;
            }
        }
        else
        {
            int rem = num % 10;
            sum += rem;
        }
        num = num/10;
        count++;
    }
    return sum;
}