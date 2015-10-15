#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cassert>

using namespace std;

string c[6] = {
    "alpha", "beta", "beta", "delta", "gamma", "gamma"
};

void exchange(string& x, string& y)
{
    string temp = x;
    x = y;
    y = temp;
}

int appendToAll(string a[], int n, string value) {
    if (n<0) return -1; // detect if n is negative
    for (int i=0; i<n; i++)
        a[i] += value; // attach value to the end of the n elements of the array
    return n;
}

int lookup(const string a[], int n, string target) {
    for (int i=0; i<n; i++)
        if (a[i] == target) return i; // detect if there is any match
    return -1; // if no match is found or n is negative
}

int positionOfMax(const string a[], int n) {
    string max="";
    int maxn=-1; // in case no element is found, return -1
    for (int i=0; i<n; i++)
        if (max<a[i])
        {
            max=a[i]; // record the value of the currently largest string
            maxn=i; // record the position of the currently largest string
        }
    return maxn; // return position or -1
}
    
int rotateLeft(string a[], int n, int pos)
{
    if ((n<0) || (pos>n-1) || (pos<0)) return -1; // detect if n is negative or pos is out of range
    string b = a[pos]; // record the value of a[pos]
    for (int i=pos; i<n-1; i++)
        a[i] = a[i+1]; // move elements of positions from pos+1 to n one place to left
    a[n-1] = b; // assign the value of a[pos] to the end of the array
    return pos;
}
    
int rotateRight(string a[], int n, int pos)
{
    if ((n<0) || (pos<0) || (pos>n-1)) return -1; // detect if n is negative or pos is out of range
    string b = a[pos]; // record the value of a[pos]
    for (int i=pos; i>0; i--)
        a[i] = a[i-1]; // move elements of positions from 0 to pos-1 one place to right
    a[0] = b; // assign the value of a[pos] to the beginning of the array
    return pos;
}
    
int flip(string a[], int n)
{
    if (n<0) return -1;
    for (int i=0; i<=(n-2)/2; i++) // take the middle of the array
    {
        string temp = a[i];
        a[i] = a[n-1-i];
        a[n-1-i] = temp; // exchange value of elements of first half of the array with those corresponding second half of the array
    }
    return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    int n;
    if (n1<n2) n=n1; else n=n2; // take the smaller of n1 and n1 as the number of iterations
    if (n<0) return -1;
    for (int i=0; i<n; i++)
        if (a1[i] != a2[i]) return i; // detect if there is any difference
    return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if ((n1<0) || (n2<0)) return -1;
    for (int i=0; i<=n1-n2; i++) {
        int flag = true; // record if there is any mismatch among the subsequences
        for (int j=0; j<n2; j++)
            if (a1[i+j] != a2[j]) flag = false; // if there is any mismatch, record it.
        if (flag) return i; // if no mismatch is found, return the position
    }
    return -1; // Otherwise, return -1
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    int n = n1;
    if ((n1<0) || (n2<0)) return -1;
    for (int i=0; i<n2; i++)
        for (int j=0; j<n1; j++)
            if ((a2[i] == a1[j]) && (j<n))
                n = j; // if there is a match and the position number is less than the previous, record it.
    if (n == n1) return -1; // if no match is found, n should still be equal to its initial value n1, and thus we should return -1
        else return n; // Otherwise we should return the recorded smallest position number
}

int separate(string a[], int n, string separator)
{
    if (n<0) return -1;
    int firstNotKnown = 0;
    int firstNotLess = 0;
    int firstGreater = n;
    while (firstNotKnown < firstGreater)
    {
        if (a[firstNotKnown] > separator)
        {
            firstGreater--;
            exchange(a[firstNotKnown], a[firstGreater]);
        }
        else
        {
            if (a[firstNotKnown] < separator)
            {
                exchange(a[firstNotKnown], a[firstNotLess]);
                firstNotLess++;
            }
            firstNotKnown++;
        }
    }
    return firstNotLess;
}

bool separatecheck(const string a[], int n, int p, string separator)
{
    for (int k = 0; k < p; k++)
        if (a[k] >= separator)
            return false;
    for ( ; p < n  &&  a[p] == separator; p++)
        ;
    for (int k = p; k < n; k++)
        if (a[k] <= separator)
            return false;
    string b[100];
    copy(a,a+n,b);
    sort(b,b+n);
    return equal(b,b+n,c);
}

void testone(int n)
{
    const int N = 6;
    
    // Act as if  a  were declared:
    //   string a[6] = {
    //       "alpha", "beta", "gamma", "gamma", "beta", "delta"
    //   };
    // This is done in a way that will probably cause a crash if
    // a[-1] or a[6] is accessed:  We place garbage in those positions.
    string aa[1+N+1] = {
        "", "alpha", "beta", "gamma", "gamma", "beta", "delta", ""
    };
    string* a = &aa[1];
    string* z = aa;
    //a[-1].string::~string();
    //a[N].string::~string();
    fill_n(reinterpret_cast<char*>(&a[-1]), sizeof(a[-1]), 0xEF);
    fill_n(reinterpret_cast<char*>(&a[N]), sizeof(a[N]), 0xEF);
    
    string b[N] = {
        "alpha", "beta", "gamma", "delta", "beta", "delta"
    };
    
    switch (n)
    {
        case  1: {
            assert(appendToAll(z, -1, "rho") == -1 && a[0] == "alpha");
        } break; case  2: {
            assert(appendToAll(z, 0, "rho") == 0 && a[0] == "alpha");
        } break; case  3: {
            assert(appendToAll(a, 1, "rho") == 1 && a[0] == "alpharho" &&
                   a[1] == "beta");
        } break; case  4: {
            assert(appendToAll(a, 6, "rho") == 6 && a[0] == "alpharho" &&
                   a[1] == "betarho" && a[2] == "gammarho" &&
                   a[3] == "gammarho" && a[4] == "betarho" &&
                   a[5] == "deltarho");
        } break; case  5: {
            assert(lookup(z, -1, "alpha") == -1);
        } break; case  6: {
            assert(lookup(z, 0, "alpha") == -1);
        } break; case  7: {
            assert(lookup(a, 1, "alpha") == 0);
        } break; case  8: {
            assert(lookup(a, 6, "delta") == 5);
        } break; case  9: {
            assert(lookup(a, 6, "beta") == 1);
        } break; case 10: {
            assert(lookup(a, 6, "zeta") == -1);
        } break; case 11: {
            assert(positionOfMax(z, -1) == -1);
        } break; case 12: {
            assert(positionOfMax(z, 0) == -1);
        } break; case 13: {
            assert(positionOfMax(a, 1) == 0);
        } break; case 14: {
            assert(positionOfMax(a, 3) == 2);
        } break; case 15: {
            assert(positionOfMax(a, 6) == 2);
        } break; case 16: {
            assert(positionOfMax(a+3, 3) == 0);
        } break; case 17: {
            a[0] = "";
            a[1] = " ";
            a[2] = "";
            assert(positionOfMax(a, 3) == 1);
        } break; case 18: {
            assert(rotateLeft(z, -1, 0) == -1 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 19: {
            assert(rotateLeft(a, 6, -1) == -1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 20: {
            assert(rotateLeft(a, 6, 6) == -1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 21: {
            assert(rotateLeft(z, 0, 0) == -1 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 22: {
            assert(rotateLeft(a, 1, 0) == 0 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 23: {
            assert(rotateLeft(a, 6, 0) == 0 &&
                   a[0] == "beta" && a[1] == "gamma" && a[2] == "gamma" &&
                   a[3] == "beta" && a[4] == "delta" && a[5] == "alpha");
        } break; case 24: {
            assert(rotateLeft(a, 6, 5) == 5 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 25: {
            assert(rotateLeft(a, 6, 3) == 3 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "beta" && a[4] == "delta" && a[5] == "gamma");
        } break; case 26: {
            assert(rotateLeft(a, 5, 3) == 3 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "beta" && a[4] == "gamma" && a[5] == "delta");
        } break; case 27: {
            assert(rotateRight(z, -1, 0) == -1 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 28: {
            assert(rotateRight(a, 6, -1) == -1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 29: {
            assert(rotateRight(a, 6, 6) == -1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 30: {
            assert(rotateRight(z, 0, 0) == -1 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 31: {
            assert(rotateRight(a, 1, 0) == 0 &&
                   a[0] == "alpha" && a[1] == "beta");
        } break; case 32: {
            assert(rotateRight(a, 6, 0) == 0 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 33: {
            assert(rotateRight(a, 6, 5) == 5 &&
                   a[0] == "delta" && a[1] == "alpha" && a[2] == "beta" &&
                   a[3] == "gamma" && a[4] == "gamma" && a[5] == "beta");
        } break; case 34: {
            assert(rotateRight(a, 5, 3) == 3 &&
                   a[0] == "gamma" && a[1] == "alpha" && a[2] == "beta" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 35: {
            assert(flip(z, -1) == -1 && a[0] == "alpha");
        } break; case 36: {
            assert(flip(z, 0) == 0 && a[0] == "alpha");
        } break; case 37: {
            assert(flip(a, 1) == 1 && a[0] == "alpha" &&
                   a[1] == "beta");
        } break; case 38: {
            assert(flip(a, 2) == 2 && a[0] == "beta" &&
                   a[1] == "alpha" && a[2] == "gamma");
        } break; case 39: {
            assert(flip(a, 5) == 5 && a[0] == "beta" &&
                   a[1] == "gamma" && a[2] == "gamma" && a[3] == "beta" &&
                   a[4] == "alpha" && a[5] == "delta");
        } break; case 40: {
            a[2] = "zeta";
            assert(flip(a,6) == 6 && a[0] == "delta" && a[1] == "beta" &&
                   a[2] == "gamma" && a[3] == "zeta" && a[4] == "beta" &&
                   a[5] == "alpha");
        } break; case 41: {
            assert(differ(z, -1, b, 6) == -1);
        } break; case 42: {
            assert(differ(a, 6, z, -1) == -1);
        } break; case 43: {
            assert(differ(z, 0, b, 0) == 0);
        } break; case 44: {
            assert(differ(a, 3, b, 3) == 3);
        } break; case 45: {
            assert(differ(a, 3, b, 2) == 2);
        } break; case 46: {
            assert(differ(a, 2, b, 3) == 2);
        } break; case 47: {
            assert(differ(a, 6, b, 6) == 3);
        } break; case 48: {
            assert(subsequence(z, -1, b, 6) == -1);
        } break; case 49: {
            assert(subsequence(a, 6, z, -1) == -1);
        } break; case 50: {
            assert(subsequence(z, 0, b, 6) == -1);
        } break; case 51: {
            assert(subsequence(a, 6, z, 0) == 0);
        } break; case 52: {
            assert(subsequence(a, 6, b, 1) == 0);
        } break; case 53: {
            assert(subsequence(a, 6, b+4, 2) == 4);
        } break; case 54: {
            assert(subsequence(a, 6, b+3, 1) == 5);
        } break; case 55: {
            assert(subsequence(a, 6, b+3, 2) == -1);
        } break; case 56: {
            assert(subsequence(a, 6, b+2, 2) == -1);
        } break; case 57: {
            assert(subsequence(a, 6, a, 6) == 0);
        } break; case 58: {
            assert(lookupAny(a, 6, z, -1) == -1);
        } break; case 59: {
            assert(lookupAny(z, -1, b, 6) == -1);
        } break; case 60: {
            assert(lookupAny(z, 0, b, 1) == -1);
        } break; case 61: {
            assert(lookupAny(a, 6, z, 0) == -1);
        } break; case 62: {
            assert(lookupAny(a, 1, b, 1) == 0);
        } break; case 63: {
            assert(lookupAny(a, 6, b+3, 3) == 1);
        } break; case 64: {
            assert(lookupAny(a, 4, b+3, 3) == 1);
        } break; case 65: {
            assert(lookupAny(a, 2, b+2, 2) == -1);
        } break; case 66: {
            assert(separate(z, -1, "beta") == -1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 67: {
            assert(separate(z, 0, "beta") == 0 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 68: {
            assert(separate(a, 1, "aaa") == 0 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 69: {
            assert(separate(a, 1, "alpha") == 0 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 70: {
            assert(separate(a, 1, "zeta") == 1 &&
                   a[0] == "alpha" && a[1] == "beta" && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 71: {
            assert(separate(a, 2, "aaa") == 0 &&
                   separatecheck(a, 2, 0, "aaa") && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 72: {
            assert(separate(a, 2, "alpha") == 0 &&
                   separatecheck(a, 2, 0, "alpha") && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 73: {
            assert(separate(a, 2, "beta") == 1 &&
                   separatecheck(a, 2, 1, "beta") && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 74: {
            assert(separate(a, 2, "zeta") == 2 &&
                   separatecheck(a, 2, 2, "zeta") && a[2] == "gamma" &&
                   a[3] == "gamma" && a[4] == "beta" && a[5] == "delta");
        } break; case 75: {
            assert(separate(a, 6, "aaa") == 0 && separatecheck(a, 6, 0, "aaa"));
        } break; case 76: {
            assert(separate(a, 6, "alpha") == 0 &&
                   separatecheck(a, 6, 0, "alpha"));
        } break; case 77: {
            assert(separate(a, 6, "beta") == 1 &&
                   separatecheck(a, 6, 1, "beta"));
        } break; case 78: {
            assert(separate(a, 6, "delta") == 3 &&
                   separatecheck(a, 6, 3, "delta"));
        } break; case 79: {
            assert(separate(a, 6, "gamma") == 4 &&
                   separatecheck(a, 6, 4, "gamma"));
        } break; case 80: {
            assert(separate(a, 6, "zeta") == 6 &&
                   separatecheck(a, 6, 6, "zeta"));
        } break; case 81: {
            a[2] = "mu";
            c[5] = "mu";
            assert(separate(a, 6, "mu") == 5 && separatecheck(a, 6, 5, "mu"));
        } break; case 82: {
            assert(separate(a, 6, "chi") == 3 && separatecheck(a, 6, 3, "chi"));
        } break; case 83: {
            // This case tested whether rotateLeft used an extra array
        } break; case 84: {
            // This case tested whether rotateRight used an extra array
        } break; case 85: {
            // This case tested whether flip used an extra array
        } break; case 86: {
            // This case tested whether separate used an extra array
        } break;
    }
    
    new (&a[-1]) string;
    new (&a[N]) string;
}

int main()
{
    cout << "Enter a test number (1 to 86): ";
    int n;
    cin >> n;
    if (n < 1  ||  n > 82)
    {
        cout << "Bad test number" << endl;
        return 1;
    }
    testone(n);
    cout << "Passed test " << n << endl;
}