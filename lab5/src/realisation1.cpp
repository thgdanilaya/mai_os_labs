#include <iostream>
#include <cmath>
#include "../include/lib.h"

using namespace std;


int* Sort(int * array, int size, int left = 0) {
    ++size;
    int tmp;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (array[j] > array[j + 1]) {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}



float SinIntegral(float a, float b, float e)
{
    float square = 0;
    for (float i = a; i <= b; i += e) {
        square += e * sin(i);
    }
    return square;
}
