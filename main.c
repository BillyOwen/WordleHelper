#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define WORD_LENGTH 5


struct Wordle
{
    char options[WORD_LENGTH][26];
    unsigned int length[WORD_LENGTH];
    char mustHaves[WORD_LENGTH];
    unsigned int mustHavesLength;
};

void removeLetter(struct Wordle* wordle, unsigned int letterPosition, char letter)
{
    int switchFlag = 0;
    for (int i = 0; i < wordle->length[letterPosition]; ++i)
    {
        if (switchFlag && i < wordle->length[letterPosition])
        {
            wordle->options[letterPosition][i] = wordle->options[letterPosition][i + 1];
        }
        else if (wordle->options[letterPosition][i] == letter)
        {
            switchFlag = 1;
            if (i != wordle->length[letterPosition])
            {
                wordle->options[letterPosition][i] = wordle->options[letterPosition][i + 1];
            }
            wordle->length[letterPosition]--;
        }
    }
}
void setLetter(struct Wordle* wordle, unsigned int letterPosition, char letter)
{
    wordle->length[letterPosition] = 1;
    wordle->options[letterPosition][0] = letter;

    wordle->mustHaves[(wordle->mustHavesLength)++] = letter;
}
void addMustHave(struct Wordle* wordle, unsigned int letterPosition, char letter)
{
    removeLetter(wordle, letterPosition, letter);
    wordle->mustHaves[(wordle->mustHavesLength)++] = letter;
}


/*
Possible word list
assume each word is the solution
Guess every word, do wordle stuff
If guessed word is correct, add x to guessed
possible word list
guess every word, do wordle stuff



*/



int main(int argc, char* args[])
{
    struct Wordle theWord;

    theWord.mustHavesLength = 0;
    for (int i = 0; i < WORD_LENGTH; ++i)
    {
        for (int j = 0; j < 26; ++j)
        {
            theWord.options[i][j] = j + 97;
        }
        theWord.length[i] = 26;
    }
    //Load info
    if (argc < 2) return 0;
    for (int i = 1; i < argc; i += 2)
    {
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            switch (args[i + 1][j])
            {
            case '0':
                for (int k = 0; k < WORD_LENGTH; ++k)
                    removeLetter(&theWord, k, args[i][j]);
                break;
            case '1':
                addMustHave(&theWord, j, args[i][j]);
                break;
            case '2':
                setLetter(&theWord, j, args[i][j]);
                break;
            }
        }
    }


    /*
    for (int i = 0; i < WORD_LENGTH; ++i)
    {
        for (int j = 0; j < theWord.length[i]; ++j)
        {
            printf("%c", theWord.options[i][j]);
        }
        printf("\n");
    }
    */
    
    //Load word list
    FILE* fp;
    fp = fopen("wordList.txt", "r");

    int wordsInWordList = 0;
    char* wordList;

    char buffer[WORD_LENGTH + 2];

    while (fgets(buffer, WORD_LENGTH + 2, fp))
    {
        wordsInWordList++;
    }


    wordList = (char*)malloc(sizeof(char) * WORD_LENGTH * wordsInWordList);

    fseek(fp, 0, SEEK_SET);

    for (int i = 0; i < wordsInWordList * WORD_LENGTH; i += WORD_LENGTH)
    {
        fgets(buffer, WORD_LENGTH + 2, fp);
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            wordList[i + j] = buffer[j];
        }
    }
    fclose(fp);
    


    //Print out possible answers
    for (int i = 0; i < wordsInWordList * WORD_LENGTH; i += WORD_LENGTH)
    {
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            int notIncluded = 1;
            for (int k = 0; k < theWord.length[j]; ++k)
            {
                if (wordList[i + j] == theWord.options[j][k])
                    notIncluded = 0;
            }
            if (notIncluded)
            {
                goto nextWord;
            }
        }
        int mustHavesUsed = 0;
        char used[WORD_LENGTH];
        for (int j = 0; j < WORD_LENGTH; ++j)
            used[j] = 0;
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            for (int k = 0; k < theWord.mustHavesLength; ++k)
            {
                if (!used[k] && wordList[i + j] == theWord.mustHaves[k])
                {
                    used[k] = 1;
                    mustHavesUsed++;
                }
            }
        }
        if (mustHavesUsed != theWord.mustHavesLength)
            goto nextWord;


        //Good to go
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            printf("%c", wordList[i + j]);
        }
        printf("\n");

        nextWord:

        1;
    }

    
    //Best guess
    
    
    
    
    for (int i = 0; i < theWord.mustHavesLength; ++i)
    {
        printf("%c\t", theWord.mustHaves[i]);
    }
    
    
    
    
    
    printf("\nBest Guess:\t");
    for (int i = 0; i < WORD_LENGTH; ++i)
    {
        printf("%c", wordList[0+i]);
    }


    
    return 0;
}

