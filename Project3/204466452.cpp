#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

bool isTuneWellFormed(string tune);
int translateTune(string tune, string& instructions, int& badBeat);
char translateNote(int octave, char noteLetter, char accidentalSign);

int main()
{
    string instrs, tune;
    int badb;
    getline(cin, tune);
    getline(cin, instrs);
    cin >> badb;
    cout << translateTune(tune, instrs, badb) << endl << instrs << endl << badb << endl;
}

bool isTuneWellFormed(string tune)
{
    bool noteFlag=0, signFlag=0, digitFlag=0; // False represents the character is nonexistent in current note.
    for (int i=0; i<tune.length(); i++)
    {
        if ((tune[i]>='A') && (tune[i]<='G'))
        {
            noteFlag = 1;
            digitFlag = 0;
            signFlag = 0;
        }// Mark the exsitence of a letter and set others to false, because a letter represents the start of a new note.
        else if ((tune[i]=='#') || (tune[i]=='b'))
        {
            if ((noteFlag) && (!signFlag) && (!digitFlag)) signFlag = 1;
            else return 0;
        }// If it is a accidental sign, there must be a letter and no other characters before it in the current note.
        else if ((tune[i]>='0') && (tune[i]<='9'))
        {
            if ((noteFlag) && (!digitFlag)) digitFlag = 1;
            else return 0;
        }// If it is a digit, there must be a letter and no digit before it in the current note.
        else if (tune[i]=='/')
        {
            signFlag = 0;
            digitFlag = 0;
            noteFlag = 0;
        }// If it is a slash, the beat is ended and all marks should be set as false.
        else return 0;
    }
    if ((!signFlag) && (!digitFlag) && (!noteFlag)) return 1; // In case that the last character is not a slash.
    else return 0;
}

int translateTune(string tune, string& instructions, int& badBeat)
{
    string instr, inst;
    char noteLetter = '\0', accidentalSign=' ', translatedNote;
    int octave=4, i=0, badb=1;

    if (!isTuneWellFormed(tune)) return 1;
    else
    {
		// Some more commenting is needed for the logic followed
        while (i<tune.length())
        {
            if (isupper(tune[i])) noteLetter=tune[i];
            if (isdigit(tune[i])) octave=tune[i]-'0'; // Subtract the code number of '0' from current digit's ASCII code to transform the digit character into its corresponding integer.
            if ((tune[i])=='#' || (tune[i]=='b')) accidentalSign=tune[i];
            if (tune[i]=='/')
            {
                if (noteLetter=='\0') inst = ' '; // In case that the current beat is empty.
                else
                {
                    translatedNote = translateNote(octave, noteLetter, accidentalSign);
                    if (translatedNote==' ') // According to the translateNote, if it is a badbeat, it will return ' '.
                    {
                        badBeat = badb;
                        return 2;
                    }
                    else inst += translatedNote; // Add the current translated note to this beat.
                    octave = 4;
                    noteLetter = '\0';
                    accidentalSign = ' '; // Current note is translated and we should set all variables to original.
                }
                if (inst.length()>1) instr = instr + "[" + inst + "]"; // If current beat has more than one note, add brackets.
                else instr += inst;
                inst=""; badb++; // Set the next beat as empty and increase the number of beats.
            }
            else if (isupper(tune[i+1]))
            {
                translatedNote = translateNote(octave, noteLetter, accidentalSign);
                if (translatedNote==' ')
                {
                    badBeat = badb;
                    return 2;
                }
                else inst += translatedNote;
                octave = 4;
                noteLetter = '\0';
                accidentalSign = ' ';
            }// If it is a new note, translate it and add it to current beat.
            i++;
        }
        instructions = instr; // If no bad beat is detected, assign the translated tune to instructions.
        return 0;
    }
}

char translateNote(int octave, char noteLetter, char accidentalSign)
{
    int note;
    switch (noteLetter)
    {
        case 'C':  note =  0; break;
        case 'D':  note =  2; break;
        case 'E':  note =  4; break;
        case 'F':  note =  5; break;
        case 'G':  note =  7; break;
        case 'A':  note =  9; break;
        case 'B':  note = 11; break;
        default:   return ' ';
    }
    switch (accidentalSign)
    {
        case '#':  note++; break;
        case 'b':  note--; break;
        case ' ':  break;
        default:   return ' ';
    }
    int sequenceNumber = 12 * (octave - 2) + note;
    string keymap = "Z1X2CV3B4N5M,6.7/A8S9D0FG!H@JK#L$Q%WE^R&TY*U(I)OP";
    if (sequenceNumber < 0  ||  sequenceNumber >= keymap.size())
        return ' ';
    return keymap[sequenceNumber];
}
