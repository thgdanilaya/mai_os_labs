#include <iostream>
#include "../include/lib.h"
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;


int main() {
    int command;
    cout << "Insert a command\n 1 - engage sin function\n 2 - engage Sort function\n";
    cout << SYSTEM << '\n';

    while (cin >> command) {
        if (command == 1) { //sin function
            float a, b, e;
            cout << "Enter A, B, e: ";
            cin >> a >> b >> e;
            cout << "Integral value " << SinIntegral(a, b, e) << '\n';
            break;
        }
        if (command == 2) { //sort function
            cout << "Insert array length\n";
            int N;
            cin >> N;
            int array[N];
            cout << "Insert array elements\n";
            for (int i = 0; i < N; ++i) {
                cin >> array[i];
            };
            int size = sizeof(array) / sizeof(array[0]);
            Sort(array, size - 1, 0);
            cout << "Result is:\n";
            for (int i = 0; i < N; ++i) {
                cout << array[i] << " ";
            }
            cout << "\n";
            break;
        } else {
            cout << "--Wrong command!--\n";
            cout << "Insert a command\n 0 - change library\n 1 - engage Pi function\n 2 - engage Sort function\n";
        }
    }


cout << "Shutting down...\n";

return 0;
}
