#include <iostream>
#include "../include/lib.h"
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;


int main() {

    int command;
    string lib1 = "./libdynamic1.so";
    string lib2 = "./libdynamic2.so";
    void *curlib = dlopen(lib1.c_str(), RTLD_LAZY);

    int *(*Sort)(int *array, int size, int left);
    float (*SinIntegral)(float a, float b, float e);

    Sort = (int *(*)(int *, int, int)) dlsym(curlib, "Sort");
    SinIntegral = (float (*)(float, float, float)) dlsym(curlib, "SinIntegral");

    int lib_id = 1;

    while (cin >> command) {
        switch (command) {
            case 0: { //change lib
                dlclose(curlib);
                if (lib_id == 1) {
                    curlib = dlopen(lib2.c_str(), RTLD_LAZY);
                    lib_id = 2;
                } else {
                    curlib = dlopen(lib1.c_str(), RTLD_LAZY);
                    lib_id = 1;
                }
                Sort = (int *(*)(int *, int, int)) dlsym(curlib, "Sort");
                SinIntegral = (float (*)(float, float, float)) dlsym(curlib, "SinIntegral");
                continue;
            }
            case 1: { //sin function
                cout << "Insert a, b, e for function\n";
                float a, b, e;
                cin >> a >> b >> e;
                float ans = SinIntegral(a, b, e);
                cout << "result: " << ans << '\n';
                break;
            }
            case 2: { //sort function
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
            }
            default: {
                cout << "--Wrong command!--\n";
                cout << "Insert a command\n 0 - change library\n 1 - engage Pi function\n 2 - engage Sort function\n";
            }
        }
    }
    cout << "Library closed.\n";
}
