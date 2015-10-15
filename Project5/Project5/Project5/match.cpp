#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <cstdlib>

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
            nRules--; // because the elements after the current one are moved one place to the left, we need to decrease both i and nRules by 1.
            continue;
        }
        
        if ((word1[i][0] == '\0') || (word2[i][0] == '\0'))
        {
            moveWord(i, distance, word1, word2, nRules);
            nRules--;
            continue;
        } // if the first or second of the word of the current rule contains no character, delete the current rule
        
        for (int j = 0; word1[i][j] != '\0'; j++)
        {
            word1[i][j] = tolower(word1[i][j]); // transform the letter to lower case; if the character is not a letter, it will return the original character
            if ((word1[i][j] < 'a') || (word1[i][j] > 'z'))
            {
                moveWord(i, distance, word1, word2, nRules); // if there exists any character other than a letter in word1, delete the current rule by moving the rules
                nRules--;
                continue;
            }
        }
        
        for (int j = 0; word2[i][j] != '\0'; j++)
        {
            word2[i][j] = tolower(word2[i][j]);
            if ((word2[i][j] < 'a') || (word2[i][j] > 'z'))
            {
                moveWord(i, distance, word1, word2, nRules);
                nRules--;
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
                nRules--;
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
                        break;
                    }
            }
    return quality;
}
                    
int d[3] =                      {    2,    1,    3 };
char w1[3][MAX_WORD_LENGTH+1] = { "aa", "bb", "cc" };
char w2[3][MAX_WORD_LENGTH+1] = { "dd", "ee", "ff" };

int e[2] =                      {    4,    4 };
char x1[2][MAX_WORD_LENGTH+1] = { "aa", "cc" };
char x2[2][MAX_WORD_LENGTH+1] = { "bb", "dd" };

const int THREE_LETTERS = 26*26*26;
int dd[THREE_LETTERS];
char ww1[THREE_LETTERS][MAX_WORD_LENGTH+1];
char ww2[THREE_LETTERS][MAX_WORD_LENGTH+1];

bool eq(int c, int dist, const char wd1[], const char wd2[])
{
    return d[c] == dist  &&  strcmp(w1[c], wd1) == 0  &&
    strcmp(w2[c], wd2) == 0;
}

void testone(int n)
{
    switch (n)
    {
        case  1: {
            assert(standardizeRules(d, w1, w2, 0) == 0);
        } break; case  2: {
            assert(standardizeRules(d, w1, w2, 1) == 1  &&
                   eq(0, 2, "aa", "dd"));
        } break; case  3: {
            d[0] = 0;
            assert(standardizeRules(d, w1, w2, 1) == 0);
        } break; case  4: {
            d[0] = 0;
            assert(standardizeRules(d, w1, w2, 2) == 1  &&
                   eq(0, 1, "bb", "ee"));
        } break; case  5: {
            d[1] = 0;
            assert(standardizeRules(d, w1, w2, 2) == 1  &&
                   eq(0, 2, "aa", "dd"));
        } break; case  6: {
            d[1] = 0;
            d[2] = 0;
            assert(standardizeRules(d, w1, w2, 3) == 1  &&
                   eq(0, 2, "aa", "dd"));
        } break; case  7: {
            d[0] = 0;
            d[1] = 0;
            assert(standardizeRules(d, w1, w2, 3) == 1  &&
                   eq(0, 3, "cc", "ff"));
        } break; case  8: {
            d[0] = 0;
            d[1] = 0;
            d[2] = 0;
            assert(standardizeRules(d, w1, w2, 3) == 0);
        } break; case  9: {
            strcpy(w1[1], "");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 10: {
            strcpy(w2[1], "");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 11: {
            strcpy(w1[1], "bb@");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 12: {
            strcpy(w1[1], "bb ");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 13: {
            strcpy(w1[1], "bBBb");
            assert(standardizeRules(d, w1, w2, 2) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 1, "bbbb", "ee"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 1, "bbbb", "ee"))
                    )
                   );
        } break; case 14: {
            strcpy(w1[1], "bBBb");
            strcpy(w2[1], "EeEeE");
            assert(standardizeRules(d, w1, w2, 2) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 1, "bbbb", "eeeee"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 1, "bbbb", "eeeee"))
                    )
                   );
        } break; case 15: {
            strcpy(w2[1], "bb");
            assert(standardizeRules(d, w1, w2, 2) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 1, "bb", "bb"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 1, "bb", "bb"))
                    )
                   );
        } break; case 16: {
            strcpy(w2[0], "aa");
            strcpy(w1[1], "aa");
            strcpy(w2[1], "aa");
            assert(standardizeRules(d, w1, w2, 2) == 1  &&
                   eq(0, 2, "aa", "aa")
                   );
        } break; case 17: {
            strcpy(w1[1], "aa");
            strcpy(w2[1], "dd");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 18: {
            strcpy(w1[1], "aa");
            strcpy(w2[1], "dd");
            d[1] = 2;
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 19: {
            strcpy(w1[1], "aa");
            strcpy(w2[1], "dd");
            d[1] = 5;
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 5, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 5, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 20: {
            strcpy(w1[1], "aa");
            strcpy(w2[1], "dd");
            strcpy(w1[2], "aa");
            strcpy(w2[2], "dd");
            assert(standardizeRules(d, w1, w2, 3) == 1  &&
                   eq(0, 3, "aa", "dd")
                   );
        } break; case 21: {
            strcpy(w1[2], "aa");
            strcpy(w2[2], "dd");
            d[1] = 0;
            assert(standardizeRules(d, w1, w2, 3) == 1  &&
                   eq(0, 3, "aa", "dd")
                   );
        } break; case 22: {
            strcpy(w1[1], "abcdefghijklmnopqrst");
            strcpy(w2[1], "abcdefghijklmnopqrsu");
            d[0] = 0;
            assert(standardizeRules(d, w1, w2, 2) == 1  &&
                   eq(0, 1, "abcdefghijklmnopqrst",
                      "abcdefghijklmnopqrsu")
                   );
        } break; case 23: {
            strcpy(w2[0], "DD");
            strcpy(w1[1], "AA");
            strcpy(w2[1], "dD");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 24: {
            strcpy(w1[1], "dd");
            strcpy(w2[1], "aa");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 25: {
            strcpy(w1[1], "dd");
            strcpy(w2[1], "aa");
            d[1] = 2;
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   (((eq(0, 2, "aa", "dd")  ||  eq(0, 2, "dd", "aa")
                      )  &&  eq(1, 3, "cc", "ff")
                     )  ||
                    ((eq(1, 2, "aa", "dd")  ||  eq(1, 2, "dd", "aa")
                      )  &&  eq(0, 3, "cc", "ff")
                     )
                    )
                   );
        } break; case 26: {
            strcpy(w1[1], "Dd");
            strcpy(w2[1], "aA");
            assert(standardizeRules(d, w1, w2, 3) == 2  &&
                   ((eq(0, 2, "aa", "dd")  &&  eq(1, 3, "cc", "ff"))  ||
                    (eq(1, 2, "aa", "dd")  &&  eq(0, 3, "cc", "ff"))
                    )
                   );
        } break; case 27: {
            int dx[7] = { 2, 4, 1, 3, 2, 1, 13 };
            char w1x[7][MAX_WORD_LENGTH+1] = { "mad", "deranged",
                "NEFARIOUS", "half-witted", "robot", "plot", "have" };
            char w2x[7][MAX_WORD_LENGTH+1] = { "scientist", "robot",
                "PLOT", "assistant", "deranged", "Nefarious", "mad" };
            assert(standardizeRules(dx, w1x, w2x, 7) == 4);
        } break; case 28: {
            assert(determineQuality(e, x1, x2, 2, "") == 0);
        } break; case 29: {
            assert(determineQuality(e, x1, x2, 2, " ") == 0);
        } break; case 30: {
            assert(determineQuality(e, x1, x2, 2, "aa bb") == 1);
        } break; case 31: {
            e[0] = 1;
            assert(determineQuality(e, x1, x2, 2, "aa bb") == 1);
        } break; case 32: {
            e[0] = 1;
            assert(determineQuality(e, x1, x2, 2, "aa     bb") == 1);
        } break; case 33: {
            assert(determineQuality(e, x1, x2, 2, " aa bb") == 1);
        } break; case 34: {
            assert(determineQuality(e, x1, x2, 2, "aa bb ") == 1);
        } break; case 35: {
            assert(determineQuality(e, x1, x2, 2, "aa bb cc dd xx") == 2);
        } break; case 36: {
            assert(determineQuality(e, x1, x2, 1, "aa bb cc dd xx") == 1);
        } break; case 37: {
            assert(determineQuality(e, x1, x2, 2, "aa xx cc bb xx dd xx") == 2);
        } break; case 38: {
            assert(determineQuality(e, x1, x2, 2, "xx dd bb cc aa xx") == 2);
        } break; case 39: {
            assert(determineQuality(e, x1, x2, 2, "aa xx xx xx bb cc dd xx") == 2);
        } break; case 40: {
            assert(determineQuality(e, x1, x2, 2, "aa xx xx xx xx bb cc dd xx") == 1);
        } break; case 41: {
            e[0] = 1;
            e[1] = 1;
            assert(determineQuality(e, x1, x2, 2, "aa xx bb cc dd xx") == 1);
        } break; case 42: {
            e[0] = 2;
            e[1] = 2;
            assert(determineQuality(e, x1, x2, 2, "aa cc bb dd xx") == 2);
        } break; case 43: {
            assert(determineQuality(e, x1, x2, 2, "aa bb aa bb cc dd xx") == 2);
        } break; case 44: {
            assert(determineQuality(e, x1, x2, 2, "aa aa bb bb cc dd xx") == 2);
        } break; case 45: {
            strcpy(x2[0], "aa");
            assert(determineQuality(e, x1, x2, 2, "aa xx xx aa") == 1);
        } break; case 46: {
            strcpy(x2[0], "aa");
            assert(determineQuality(e, x1, x2, 2, "xx aa xx") == 0);
        } break; case 47: {
            assert(determineQuality(e, x1, x2, 2, "aA bb cc dd xx") == 2);
        } break; case 48: {
            assert(determineQuality(e, x1, x2, 2, "aa @@ ## $$ %% ^^ bb cc dd xx") == 2);
        } break; case 49: {
            assert(determineQuality(e, x1, x2, 2, "aa b@@b cc dd xx") == 2);
        } break; case 50: {
            assert(determineQuality(e, x1, x2, 2, "aa b123456789012345678901234567890b cc dd xx") == 2);
        } break; case 51: {
            e[0] = 1;
            assert(determineQuality(e, x1, x2, 2, "aa abcdefghijklmnopqrstuvwxyz bb cc dd xx") == 1);
        } break; case 52: {
            e[0] = 2;
            assert(determineQuality(e, x1, x2, 2, "aa abcdefghijklmnopqrstuvwxyz bb cc dd xx") == 2);
        } break; case 53: {
            strcpy(x1[0], "abcdefghijklmnopqrst");
            assert(determineQuality(e, x1, x2, 2, "abcdefghijklmnopqrst bb cc dd xx") == 2);
        } break; case 54: {
            strcpy(x1[0], "abcdefghijklmnopqrst");
            assert(determineQuality(e, x1, x2, 2, "abcdefghijklmnopqrstu bb cc dd xx") == 1);
        } break; case 55: {
            assert(determineQuality(e, x1, x2, 2, "aaa bb cc dd xx") == 1);
        } break; case 56: {
            assert(determineQuality(e, x1, x2, 2, "aa@bb cc dd xx") == 1);
        } break; case 57: {
            assert(determineQuality(e, x1, x2, 2, "aa bb@cc dd xx") == 0);
        } break; case 58: {
            e[0] = 1000;
            assert(determineQuality(e, x1, x2, 2, "xx aa xx") == 0);
        } break; case 59: {
            strcpy(x1[0], "a");
            strcpy(x2[0], "b");
            e[0] = 100;
            static char doc[200+1];
            strcpy(doc, "a ");
            for (int k = 0; k < 98; k++)
                strcpy(doc+2+2*k, "x ");
            strcpy(doc+198, "b ");
            assert(determineQuality(e, x1, x2, 2, doc) == 1);
        } break; case 60: {
            for (int k = 0; k < THREE_LETTERS; k++)
            {
                dd[k] = k % 2;
                strcpy(ww1[k], "a");
                strcpy(ww2[k], "aaa");
                int kk = k;
                ww2[k][2] += kk % 26;
                kk /= 26;
                ww2[k][1] += kk % 26;
                kk /= 26;
                ww2[k][0] += kk;
            }
            assert(standardizeRules(dd, ww1, ww2, THREE_LETTERS) == THREE_LETTERS / 2);
            for (int k = 0; k < THREE_LETTERS / 2; k++)
                assert(dd[k] == 1 && strcmp(ww1[k], "a") == 0 &&
                       islower(ww2[k][2]) &&
                       (ww2[k][2] - 'a') % 2 == 1);
        } break; case 61: {
            for (int k = 0; k < THREE_LETTERS; k++)
            {
                dd[k] = 1;
                strcpy(ww1[k], "a");
                strcpy(ww2[k], "aaa");
                int kk = k;
                ww2[k][2] += kk % 26;
                kk /= 26;
                ww2[k][1] += kk % 26;
                kk /= 26;
                ww2[k][0] += kk;
            }
            assert(determineQuality(dd, ww1, ww2, THREE_LETTERS,
                                    "a aaa a iaa a pzz a zzz") == 4);
            assert(determineQuality(dd, ww1, ww2, THREE_LETTERS,
                                    "a aaaa a iaaa a pzzz a zzzz") == 0);
        } break;
    }
}

int main()
{
        testone(61);
}