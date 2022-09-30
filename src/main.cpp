#include <iostream>
#include "unistd.h"
#include "vector"
#include "fstream"
#include "string"
 
using namespace std;
 
int main() {
    int pipefd[2];
    pipe(pipefd);
    int id = fork();
 
    if (id == -1) {
        return -1;
    }
    if (id == 0) {
        string filename;
        int lengthFilename;
        read(pipefd[0], &lengthFilename, sizeof(int));
        for (int i = 0; i < lengthFilename; ++i) {
            char c;
            read(pipefd[0], &c, sizeof(char));
            filename.push_back(c);
        }
        ofstream outfile(filename);
        int numbersSize;
        read(pipefd[0], &numbersSize, sizeof(int));
        int result;
        int buff;
        int current;
        for (int i = 0; i < numbersSize; ++i) {
            if (i == 0) {
                read(pipefd[0], &buff, sizeof(int));
            } else {
                read(pipefd[0], &current, sizeof(int));
                if (current == 0) {
                    exit(-1);
                } else {
                    result = buff / current;
                    buff = current;
                }
            }
        }
        outfile << result << "\n";
        outfile.close();
        close(pipefd[0]);
        close(pipefd[1]);
    } else {
        cout << "Parent's pid " << getpid() << "\n";
        cout << "Child's pid " << id << "\n";
        vector<int> nums;
        string filename;
        cout << "Input filename\n";
        cin >> filename;
        int lengthFilename = filename.length();
        int number;
        while (cin >> number) {
            nums.push_back(number);
        }
        int numbersSize = nums.size();
        write(pipefd[1], &lengthFilename, sizeof(int));
        for (int i = 0; i < lengthFilename; ++i) {
            write(pipefd[1], &filename[i], sizeof(char));
        }
        write(pipefd[1], &numbersSize, sizeof(int));
        for (int i = 0; i < numbersSize; ++i) {
            write(pipefd[1], &nums[i], sizeof(int));
        }
        close(pipefd[1]);
        close(pipefd[0]);
 
 
    }
    return 0;
}
