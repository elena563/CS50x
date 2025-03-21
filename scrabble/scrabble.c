#import <cs50.h>
#import <ctype.h>
#import <stdio.h>
#import <string.h>

int get_score(string word);

int main(void)
{
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    int score1 = get_score(word1);
    int score2 = get_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    } else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }else
    {
        printf("Tie!\n");
    }
}

int get_score(string w)
{
    string word = tolower(w);
    int N = 6;
    string sets[N] = {"qz", "zj", "fhvwy", "bcmp", "dg", "aeilnorstu"};
    int points[N] = {10, 8, 4, 3, 2, 1};
    int score = 0;
    for (int i=0; i<strlen(word); i++)
    {
        for (int s=0; s<N; s++)
        {
            string set = sets[s];
            for (int j=0; j<strlen(set); j++)
            {
                if (word[i] == set[j]){
                    score += points[s];
                    break;
                }
            }
        }
    }
    return score;
}
