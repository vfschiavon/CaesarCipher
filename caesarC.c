#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

int mod(int a, int b);
void charOps(FILE* output, char word[50], int keyparam, bool dynamic, bool decrypt);
void crypt(FILE* input, FILE* output, int keyparam, bool dynamic, bool decrypt);

int main()
{
    int op, key;
    bool dynamic = false;
    bool decrypt = false;

    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    if (input)
    {
        printf("Choose the operation number:\n");
        printf("[1] Decrypt - static key;\n");
        printf("[2] Decrypt - dynamic key;\n");
        printf("[3] Encrypt - static key;\n");
        printf("[4] Encrypt - dynamic key;\n");
        printf("[0] Exit.\n");
        printf(">> ");
        scanf("%d", &op);

        switch (op)
        {
            case 1:
                printf("Enter the key: ");
                scanf("%d", &key);
                decrypt = true;
                crypt(input, output, key, dynamic, decrypt);
                printf("File decrypted.\n");
                break;
            case 2:
                dynamic = true;
                decrypt = true;
                crypt(input, output, key, dynamic, decrypt);
                printf("File decrypted.\n");
                break;
            case 3:
                printf("Enter the key: ");
                scanf("%d", &key);
                crypt(input, output, key, dynamic, decrypt);
                printf("File encrypted.\n");
                break;
            case 4:
                dynamic = true;
                crypt(input, output, key, dynamic, decrypt);
                printf("File encrypted.\n");
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

void charOps(FILE* output, char word[50], int keyparam, bool dynamic, bool decrypt)
{
    int key = keyparam;

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
    }
    
    if (decrypt)
    {
        key = key * (-1);
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
    }
    fprintf(output, "%s", word);
}

void crypt(FILE* input, FILE* output, int keyparam, bool dynamic, bool decrypt)
{
    while (!feof(input))
    {
        int pos;
        bool found = false;
        char flag;
        char word[50];
        fscanf(input, "%s", word);

        for (int i = 0; i < strlen(word); i++)
        {
            if (word[i] == '-')
            {
                found = true;
                flag = '-';
                pos = i;
                break;
            }
            else if (word[i] == '/')
            {
                found = true;
                flag = '/';
                pos = i;
                break;
            }
        }

        if (!found)
        {
            charOps(output, word, keyparam, dynamic, decrypt);
        }
        else 
        {
            char aux1[50], aux2[50];
            strncpy(aux1, word, pos);
            aux1[pos] = '\0';
            strncpy(aux2, word + pos + 1, strlen(word) - pos);
            aux2[strlen(word) - pos - 1] = '\0';

            charOps(output, aux1, keyparam, dynamic, decrypt);
            fprintf(output, "%c", flag);
            charOps(output, aux2, keyparam, dynamic, decrypt);
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
