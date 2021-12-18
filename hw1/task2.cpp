#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;


vector<int> generate_random_vector(int n) {
    srand(unsigned(time(nullptr)));
    vector<int> v(n);
    generate(v.begin(), v.end(), std::rand);
    return v;
}

void get_max(vector<int>& v, int treads, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        int max_value = v[0];

        #pragma omp parallel for reduction(max:max_value) num_threads(treads)
        for (int i = 0; i < v.size(); ++i) {
            max_value = max(max_value, v[i]);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    cout << treads << ": " << (end - start).count() / retries << "\n";
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    auto v = generate_random_vector(n);

    for (int t = 1; t <= 10; ++t) {
        get_max(v, t, 5);
    }
}

