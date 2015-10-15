#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

const int MAX_WORD_LENGTH = 20;

void moveWord (int n, int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nRules) {
    for (int i = n; i < nRules-1; i++)
    {
        distance[i] = distance[i+1];
        strcpy(word1[i], word1[i+1]);
        strcpy(word2[i], word2[i+1]);
    } // move the rules from position n+1 to the end of the array one place to the left
}

int standardizeRules(int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nRules) {
    int i = 0;
    if (nRules < 0) return 0; // in case nRules is negative
    while ( i < nRules ) // check every rule one by one
    {
        if (distance[i] <= 0)
        {
            moveWord(i, distance, word1, word2, nRules); // if distance is non-positive, delete the current rule by moving the rules
            i--; nRules--; // because the elements after the current one are moved one place to the left, we need to decrease both i and nRules by 1.
            continue;
        }
        
        if ((word1[i][0] == '\0') || (word2[i][0] == '\0'))
        {
            moveWord(i, distance, word1, word2, nRules);
            i--; nRules--;
            continue;
        } // if the first or second of the word of the current rule contains no character, delete the current rule
        
        for (int j = 0; word1[i][j] != '\0'; j++)
        {
            word1[i][j] = tolower(word1[i][j]); // transform the letter to lower case; if the character is not a letter, it will return the original character
            if ((word1[i][j] < 'a') || (word1[i][j] > 'z'))
            {
                moveWord(i, distance, word1, word2, nRules); // if there exists any character other than a letter in word1, delete the current rule by moving the rules
                i--; nRules--;
                continue;
            }
        }
        
        for (int j = 0; word2[i][j] != '\0'; j++)
        {
            word2[i][j] = tolower(word2[i][j]);
            if ((word2[i][j] < 'a') || (word2[i][j] > 'z'))
            {
                moveWord(i, distance, word1, word2, nRules);
                i--; nRules--;
                continue;
            }
        } // do the same thing to word2 as is done on word1
        
        for (int j = 0; j < i; j++)
            if (  ( (strcmp(word1[i], word1[j]) == 0) && (strcmp(word2[i], word2[j]) == 0) ) || ( (strcmp(word1[i], word2[j]) == 0) && (strcmp(word2[i], word1[j]) == 0) )  ) // check if there is any same rule before the current rule
            {
                if (distance[i] <= distance[j]) // if there is, delete the one with the shorter distance
                    moveWord(i, distance, word1, word2, nRules);
                else
                    moveWord(j, distance, word1, word2, nRules);
                i--; nRules--;
                continue;
            }
        i++;
    }
    return nRules;
}

int determineQuality(const int distance[],
                     const char word1[][MAX_WORD_LENGTH+1],
                     const char word2[][MAX_WORD_LENGTH+1],
                     int nRules,
                     const char document[])
{
    char word[200][200+1];
    char ch;
    int i = 0, j = 0, k = 0, quality = 0;
    bool isEmp = true; // record if the current element is empty
    while (document[i] != '\0') {
        ch = tolower(document[i]); // transform the current character to lowercase
        if ((ch >= 'a') && (ch <='z'))
        {
            word[j][k]=ch;
            k++; isEmp = false; // if the current character is a letter, put it into the word array, increase the length of the array(k) by 1 and set isEmp to false
        }
        if ((ch == ' ') && (!isEmp))
        {
            word[j][k+1]='\0';
            j++;
            k = 0;
            isEmp = true; // if the current character is a space and the current word is not empty, then the current word is finished and we should add the terminal character and prepare to start a new word.
        }
        i++;
    }
    if ((i-1>=0) && (document[i-1] != ' ')) j++; // when the last word is added, the total length of the word array does not increase by 1, so we need to increase the length here; however, if the last character is a space, the length has already been increased in the while-loop.
    bool flag[nRules];
    for (i = 0; i < nRules; i++) // record if the rules are used or not
        flag[i] = true;
    for (i = 0; i < j; i++) // loop of the word array
        for (k = 0; k < nRules; k++) // loop of the rules
            if ((flag[k]) && (strcmp(word1[k], word[i]) == 0)) // if rule is not used and the first word of the rule is the same as the current word in document
            {
                int l = (i-distance[k]>=0) ? i-distance[k] : 0; // set the left bound of checking
                int r = (i+distance[k]<j) ? i+distance[k] : j-1; // se the right bound of checking
                for (int m = l; m <= r; m++)
                    if ((strcmp(word2[k], word[m]) == 0) && (m != i)) // if within the distance there is a word same as the second word of the current rule, and word[m] is not word[i] (in case there is any rule where two words are the same)
                    {
                        flag[k] = false; // set the current rule to false
                        quality++; // increase quality by 1
                    }
            }
    return quality;
}
                    
int main () {
    int nRules;
    int distance[200];
    char word1[200][MAX_WORD_LENGTH+1];
    char word2[200][MAX_WORD_LENGTH+1];
    char document[201];
    cin >> nRules;
    for (int i = 0; i < nRules; i++)
    {
        cin >> distance[i];
        cin >> word1[i];
        cin >> word2[i];
    }
    int n = standardizeRules(distance, word1, word2, nRules);
    cout << n << endl;
    for (int i = 0; i < n; i++)
        cout << distance[i] << " " << word1[i] << " " << word2[i] << endl;
    cin.ignore(10000, '\n');
    cin.getline(document, 201);
    for (int i = 0; document[i] != '\0'; i++)
        cout << document[i];
    cout << endl;
    cout << determineQuality(distance, word1, word2, n, document) << endl;
}

/*int main()
{
    const int TEST1_NCRITERIA = 4;
    int test1dist[TEST1_NCRITERIA] = {
        2,           1,          2,           4
    };
    char test1w1[TEST1_NCRITERIA][MAX_WORD_LENGTH+1] = {
        "upset",       "nov", "brandnew", "my"
    };
    char test1w2[TEST1_NCRITERIA][MAX_WORD_LENGTH+1] = {
        "that", "th",    "lis",      "stolen"
    };
    assert(determineQuality(test1dist, test1w1, test1w2, TEST1_NCRITERIA,
                            "I'm upset that on Nov. 15th, 2014, my 2 brand-new BMW 750Lis were stolen!!") == 3);
    cout << "All tests succeeded" << endl;
    cout << standardizeRules(test1dist, test1w1, test1w2, TEST1_NCRITERIA) << endl;
    for (int i = 0; i < 4; i++)
        cout << test1dist[i] << " " << test1w1[i] << " " << test1w2[i] << endl;
}*/