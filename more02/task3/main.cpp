#include <iostream>

using namespace std;

int gcd(int n, int m)
{
    while ((n %= m) && (m %= n));
    return n ? n : m;
}

int gcd(int *array, int size)
{
    int ans = array[0];
    for (int i = 1; i < size; ++i)
        ans = gcd(ans, array[i]);

    return ans;
}

bool solution(int *array, int size)
{
    int *lengths = new int[size];
    int len = 1;
    int counter = 0;

    for (int i = 1; i < size; ++i)
        if (array[i - 1] < array[i])
            len++;
        else
        {
            lengths[counter++] = len;
            len = 1;
        }
    lengths[counter++] = len;

    bool ans = gcd(lengths, counter) > 1;

    delete[] lengths;

    return ans;
}

int main()
{
    cout << "n (sequence length): ";
    int n = 0;
    cin >> n;

    cout << "items: ";
    int *sequence = new int[n];

    for (int i = 0; i < n; ++i)
        cin >> sequence[i];

    if (solution(sequence, n))
        cout << "yes" << endl;
    else
        cout << "no" << endl;


    delete[] sequence;

    return 0;
}
