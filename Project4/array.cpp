#include <iostream>
#include <string>
#include <cassert>
using namespace std;

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
    string b = a[pos]; // record the value of a[pos]
    if ((n<0) || (pos>n-1) || (pos<0)) return -1; // detect if n is negative or pos is out of range
    for (int i=pos; i<n-1; i++)
        a[i] = a[i+1]; // move elements of positions from pos+1 to n one place to left
    a[n-1] = b; // assign the value of a[pos] to the end of the array
    return pos;
}
    
int rotateRight(string a[], int n, int pos)
{
    string b = a[pos]; // record the value of a[pos]
    if ((n<0) || (pos<0) || (pos>n-1)) return -1; // detect if n is negative or pos is out of range
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
    int i = 0, j = n-1;
    while (i < j) { // if i>=j, the scan is over and we should quit the loop
        while (a[i] < separator) i++; // detect if any element is >= separator from left to right
        while (a[j] >= separator) j--; // detect if any element is < separator from right to left
        if (i < j) { // if i is not larger than or equal to j, exchange the values. Otherwise, quit the loop
            string temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i++; j--;
        }
    }
    for (i = 0; i < n; i++)
        if (a[i] >= separator)
            return i; // detect if there is any element >= separator and return the position
    return n; // Otherwise return n.
}
    int main()
    {
        string h[7] = { "glenn", "carl", "carol", "rick", "", "maggie", "daryl" };
        assert(lookup(h, 7, "maggie") == 5);
        assert(lookup(h, 7, "carol") == 2);
        assert(lookup(h, 2, "carol") == -1);
        assert(positionOfMax(h, 7) == 3);
        
        string g[4] = { "glenn", "carl", "rick", "maggie" };
        assert(differ(h, 4, g, 4) == 2);
        assert(appendToAll(g, 4, "?") == 4 && g[0] == "glenn?" && g[3] == "maggie?");
        assert(rotateLeft(g, 4, 1) == 1 && g[1] == "rick?" && g[3] == "carl?");
        
        string e[4] = { "carol", "rick", "", "maggie" };
        assert(subsequence(h, 7, e, 4) == 2);
        assert(rotateRight(e, 4, 1) == 1 && e[0] == "rick" && e[2] == "");
        
        string f[3] = { "rick", "carol", "tara" };
        assert(lookupAny(h, 7, f, 3) == 2);
        assert(flip(f, 3) == 3 && f[0] == "tara" && f[2] == "rick");
        
        assert(separate(h, 7, "daryl") == 3);
        
        cout << "All tests succeeded" << endl;
}