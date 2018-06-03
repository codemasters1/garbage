#include <iostream>
#include <cmath>

#define _USE_MATH_DEFINES

using namespace std;

int main()
{
    double f, amp;
    double a[20], b[20], x[20];

    cout << "Podaj czestotliwosc sygnalu(Hz): ";
    cin >> f;
    cout << endl << "Podaj amplitude sygnalu (V): ";
    cin >> amp;
    cout << endl << "Trwa rozwijanie w szereg Fouriera podanego sygnalu, prosze czekac" << endl;

    double w = 2.0 * M_PI * f;

    for (int i = 0; i < 20; i++)
    {
        cout << "sin -> " <<i*w<<" "<< cos(i*w) << "\n";

        a[i] = 2.0 * sin(i * w) / (i * w);
        b[i] = (-2.0 * cos(i * w) - 2.0) / (i * w);
        x[i] = a[i] * cos(i * w) + b[i] * sin(i * w);

        cout << i * w << "  " << x[i] << endl;
    }
    return 0;
}