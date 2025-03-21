#include <cs50.h>
#include <stdio.h>

int main()
{
    int height;
    do
    {
        height = get_int("Choose an height for the pyramid between 1 and 8: ");
    }
    while (height < 1 || height > 8);
    int counter = 1;
    for (int i = 0; i < height; i++)
    {
        for (int j = height - i - 1; j > 0; j--)
        {
            printf(" ");
        }
        for (int k = 0; k < counter; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int k = 0; k < counter; k++)
        {
            printf("#");
        }
        printf("\n");
        counter++;
    }
}
