#include <iostream>
#include <fstream>

using namespace std;

int mod(int a, int b);
void charOps(ofstream &output, string word, int keyparam, bool dynamic, bool decrypt);
void crypt(ifstream &input, ofstream &output, int keyparam, bool dynamic, bool decrypt);

int main()
{
    int op, key;
    bool dynamic = false;
    bool decrypt = false;

    ifstream input;
    input.open("input.txt");
    ofstream output;
    output.open("output.txt");

    if (input)
    {
        cout << "Choose the operation number:" << endl;
        cout << "[1] Decrypt - static key;" << endl;
        cout << "[2] Decrypt - dynamic key;" << endl;
        cout << "[3] Encrypt - static key;" << endl;
        cout << "[4] Encrypt - dynamic key;" << endl;
        cout << "[0] Exit." << endl;
        cout << ">> ";
        cin >> op;
        
        switch (op)
        {
            case 1:
                cout << "Enter the key: ";
                cin >> key;
                decrypt = true;
                crypt(input, output, key, dynamic, decrypt);
                cout << "File decrypted." << endl;
                break;
            case 2:
                dynamic = true;
                decrypt = true;
                crypt(input, output, key, dynamic, decrypt);
                cout << "File decrypted." << endl;
                break;
            case 3:
                cout << "Enter the key: ";
                cin >> key;
                crypt(input, output, key, dynamic, decrypt);
                cout << "File encrypted." << endl;
                break;
            case 4:
                dynamic = true;
                crypt(input, output, key, dynamic, decrypt);
                cout << "File encrypted." << endl;
                break;
            case 0:
                cout << "Program closed." << endl;
                break;
            default:
                cout << "Invalid input." << endl;
                break;
        }
        input.close();
        output.close();
        return 0;
    }
    else
    {
        cout << "Error opening input file." << endl;
        return 1;
    }
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void charOps(ofstream &output, string word, int keyparam, bool dynamic, bool decrypt)
{
    int key = keyparam;
    
    if (dynamic)
    {
        key = word.size();
        for (int i = 0; i < (int) word.size(); i++)
        {
            if (!isalnum(word.at(i)))
            {
                key--;
            }
        }
    }

    if (decrypt)
    {
        key = key * (-1);
    }

    for (int i = 0; i < (int) word.size(); i++)
    {
        if (islower(word.at(i))) // Lowecase
        {
            word.at(i) = word.at(i) + mod(key, 26);
            if (!islower(word.at(i)))
            {
                word.at(i) = word.at(i) - 26;
            }
        }
        else if (isupper(word.at(i))) // Uppercase
        {
            word.at(i) = word.at(i) + mod(key, 26);
            if (!isupper(word.at(i)))
            {
                word.at(i) = word.at(i) - 26;
            }
        }
        else if (isdigit(word.at(i))) //Number
        {
            word.at(i) = word.at(i) + mod(key, 10);
            if (!isdigit(word.at(i)))
            {
                word.at(i) = word.at(i) - 10;
            }
        }
    }
    output << word;
}

void crypt(ifstream &input, ofstream &output, int keyparam, bool dynamic, bool decrypt)
{
    while (!input.eof())
    {
        string word;
        input >> word;

        int hyphen = word.find('-');
        int bar = word.find('/');

        if (hyphen != -1)
        {
            string aux = word.substr(hyphen + 1, word.size() - hyphen);
            word.erase(hyphen, word.size() - hyphen);
            charOps(output, word, keyparam, dynamic, decrypt);
            output << "-";
            charOps(output, aux, keyparam, dynamic, decrypt);
        }
        else if (bar != -1)
        {
            string aux = word.substr(bar + 1, word.size() - bar);
            word.erase(bar, word.size() - bar);
            charOps(output, word, keyparam, dynamic, decrypt);
            output << "/";
            charOps(output, aux, keyparam, dynamic, decrypt);
        }
        else
        {
            charOps(output, word, keyparam, dynamic, decrypt);
        }

        char c = input.get();
        if (c == ' ')
        {
            output << " ";
        } 
        else if (c == '\n') 
        {
            output << endl;
        }
    }
}