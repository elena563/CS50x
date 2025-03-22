#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define ALPHA_LEN 26

string encrypt_text(string t, string k);

int main(int argc, string argv[])
{
    string key = argv[1];
    int unique[ALPHA_LEN] = {0};
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    } else if (strlen(key) != ALPHA_LEN)
    {
        printf("Key must contain %i characters\n", ALPHA_LEN);
        return 1;
    } else
    {
        for (int i = 0; i < ALPHA_LEN; i++)
        {
            key[i] = tolower(key[i]);
            char curr = key[i];
            int id = curr - 'a';
            if (isalpha(curr) == 0)
            {
                printf("Key must be alphabetic\n");
                return 1;
            } else if (unique[id] == 1)
            {
                printf("Key must contain unique characters\n");
                return 1;
            }
            unique[id] = 1;
        }
        string text = get_string("plaintext: ");
        string mess = encrypt_text(text, key);
        printf("ciphertext: %s\n", mess);
        return 0;
    }

}

string encrypt_text(string t, string k)
{
    string newt = t;
    int len = strlen(t);
    string alpha = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i< len; i++)
    {
        char lett = newt[i];
        int upp = isupper(lett);
        for (int j = 0; j< ALPHA_LEN; j++)
        {
            char a = alpha[j];

            if (lett == a || lett == toupper(a))
            {
                char b = k[j];
                if (upp != 0)
                {
                    b = toupper(b);
                }
                newt[i] = b;
                break;
            }
        }
    }
    return newt;
}
