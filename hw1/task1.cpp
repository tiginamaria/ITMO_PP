#include <iostream>
#include <unordered_set>

using namespace std;

int main(int argc, char *argv[]) {
    unordered_set<char> separators = {'\t', '\n', ' '};

    string str = argv[1];
    int last = 0;
    int words_counter = 0;
    for (int i = 0; i < str.size(); ++i) {
        if (separators.find(str[i]) != separators.end()) {
            if (last != i - 1) {
                words_counter += 1;
            }
            last = i;
        }
    }
    if (last != str.size() - 1) {
        words_counter += 1;
    }
    cout << words_counter;
}
