#include <cs50.h>
#include <stdio.h>

int main()
{
    long card = get_long("Insert your credit card number: ");
    int sum = 0;
    int msum = 0;
    int counter=0;
    int firstdig;
    int firsttwo;
    while (card > 0)
    {
        int lastdig = card%10;
        if (counter%2 == 0)
        {
            msum += lastdig;
        } else
        {
            int doubled = lastdig * 2;
            sum += (doubled % 10) + (doubled / 10);
        }
        card = card/10;
        if (card >= 10 && card < 100)
        {
            firsttwo = card;
        } else if (card >= 1 && card < 10)
        {
            firstdig = card;
        }
        counter++;
    }
    int length = counter;
    int total = sum + msum;
    if (total % 10 == 0)
    {
        if (firstdig == 4 && (length == 13 || length == 16))
        {
            printf("VISA\n");
        } else if ((firsttwo == 34 || firsttwo == 37) && length == 15)
        {
            printf("AMEX\n");
        } else if ((firsttwo >= 51 && firsttwo <= 55) && length == 16)
        {
            printf("MASTERCARD\n");
        } else
        {
            printf("INVALID\n");
        }
    } else
    {
        printf("INVALID\n");
    }
}
