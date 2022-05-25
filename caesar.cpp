#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int mod(int a, int b);
void charOps(FILE* output, char word[50], int keyparam, bool dynamic);
void crypt(FILE* input, FILE* output, int keyparam, bool dynamic);

int main()
{
    int op, key = 0;
    bool dynamic = false;

    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    if (input)
    {
        printf("Choose the operation number:\n");
        printf("(1) Decrypt - static key;\n");
        printf("(2) Decrypt - dynamic key;\n");
        printf("(3) Encrypt - static key;\n");
        printf("(4) Encrypt - dynamic key;\n");
        printf("(0) Exit.\n");
        printf(">> ");
        scanf("%d", &op);

        switch (op)
        {
            case 1:
                printf("Enter the key: ");
                scanf("%d", &key);
                crypt(input, output, key, dynamic);
                printf("File decrypted.\n");
                key = 0;
                break;
            case 2:
                dynamic = true;
                crypt(input, output, key, dynamic);
                printf("File decrypted.\n");
                dynamic = false;
                break;
            case 3:
                printf("Enter the key: ");
                scanf("%d", &key);
                crypt(input, output, key, dynamic);
                printf("File encrypted.\n");
                key = 0;
                break;
            case 4:
                key = -1;
                dynamic = true;
                crypt(input, output, key, dynamic);
                printf("File encrypted.\n");
                dynamic = false;
                key = 0;
                break;
            case 0:
                printf("Program closed.\n");
                break;
            default:
                printf("Invalid input.\n");
                break;
        }
        fclose(input);
        fclose(output);
        return 0;
    }
    else
    {
        printf("Error opening input file.\n");
        return 1;
    }
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void charOps(FILE* output, char word[50], int keyparam, bool dynamic)
{
    int key;

    if (dynamic)
    {
        key = strlen(word);
        for (int i = 0; i < strlen(word); i++)
        {
            if (!isalnum(word[i]))
            {
                key--;
            }
        }
        if (!keyparam)
        {
            key = key * (-1);
        }
    }
    else
    {
        key = keyparam;
    }

    for (int i = 0; i < strlen(word); i++)
    {
        if (islower(word[i])) //Lowecase
        {
            word[i] = word[i] + mod(key, 26);
            if (!islower(word[i]))
            {
                word[i] = word[i] - 26;
            }
        }
        else if (isupper(word[i])) //Uppercase
        {
            word[i] = word[i] + mod(key, 26);
            if (!isupper(word[i]))
            {
                word[i] = word[i] - 26;
            }
        }
        else if (isdigit(word[i])) //Number
        {
            word[i] = word[i] + mod(key, 10);
            if (!isdigit(word[i]))
            {
                word[i] = word[i] - 10;
            }
        }
        fprintf(output, "%c", word[i]);
    }
}

void crypt(FILE* input, FILE* output, int keyparam, bool dynamic)
{
    while (!feof(input))
    {
        int pos;
        bool found = false;
        char word[50];
        fscanf(input, "%s", word);

        for (int i = 0; i < strlen(word); i++)
        {
            if (word[i] == '-')
            {
                found = true;
                pos = i;
                break;
            }
        }

        if (!found)
        {
            charOps(output, word, keyparam, dynamic);
        }
        else 
        {
            char aux1[50], aux2[50];
            strncpy(aux1, word, pos);
            aux1[pos] = '\0';
            strncpy(aux2, word + pos + 1, strlen(word) - pos);
            aux2[strlen(word) - pos - 1] = '\0';

            charOps(output, aux1, keyparam, dynamic);
            fprintf(output, "-");
            charOps(output, aux2, keyparam, dynamic);
        }

        char c = getc(input);
        if (c == ' ')
        {
            fprintf(output, " ");
        } 
        else if (c == '\n') 
        {
            fprintf(output, "\n");
        }
    }
}
