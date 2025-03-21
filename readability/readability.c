#import <cs50.h>
#import <ctype.h>
#import <stdio.h>
#import <string.h>

int calculate_letters(string text);
int calculate_words(string text);
int calculate_sentences(string text);
bool is_punct(char c);

int main(void)
{
    string text = get_string("Text: ");
    float letters = (float) calculate_letters(text);
    float words = (float) calculate_words(text);
    float sentences = (float) calculate_sentences(text);

    float avgletters = letters/words * 100;
    float avgsentences = sentences/words *100;

    const float c1 = 0.0588;
    const float c2 = 0.296;
    const float c3 = 15.8;
    float index = c1 * avgletters - c2 * avgsentences - c3;
    int grade = (int) index + 0.5;

    if (grade > 0 && grade < 16){
        printf("Grade %i\n", grade);
    } else if (grade < 1)
    {
        printf("Before Grade 1");
    } else
    {
        printf("Grade 16+");
    }

}

int calculate_letters(string text)
{
    int letters = 0;
    for (int i=0; i<strlen(text); i++)
    {
        char curr = tolower(text[i]);
        if (isalpha(curr) != 0)
        {
            letters += 1;
        }
    }
    return letters;
}

bool is_punct(char c)
{
    if (c == '.' || c == '?' || c == '!'){
        return true;
    } else{
        return false;
    }
}

int calculate_words(string text)
{
    int words = 0;
    for (int i=0; i<strlen(text); i++)
    {
        char curr = tolower(text[i]);
        if (curr == ' ')
        {
            words += 1;
        } else if ((is_punct(curr)) && (text[i+1] == '\0')){
            words += 1;
        } else if ((is_punct(curr)) && (text[i+1] == ' ')){
            continue;
        }

    }
    return words;
}

int calculate_sentences(string text)
{
    int sentences = 0;
    for (int i=0; i<strlen(text); i++)
    {
        char curr = tolower(text[i]);
        if (is_punct(curr)){
            sentences +=1;
        }

    }
    return sentences;
}


