#include <iostream>
#include <cmath>
#include "../include/lib.h"
using namespace std;

int *Sort(int *s_arr, int last, int first) {
    int i = first, j = last, x = s_arr[(first + last) / 2];
    do {
        while (s_arr[i] < x) i++;
        while (s_arr[j] > x) j--;

        if (i <= j) {
            if (s_arr[i] > s_arr[j]) {
                int t;
                t = s_arr[i];
                s_arr[i] = s_arr[j];
                s_arr[j] = t;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        Sort(s_arr, last, i);
    if (first < j)
        Sort(s_arr, j, first);
}


float SinIntegral(float a, float b, float e)
{
    float square = 0;
    for (float i = a; i < b; i += e) {
        square += e * ((sin(i) + sin(i + e)) / 2);
    }
    return square;
}
