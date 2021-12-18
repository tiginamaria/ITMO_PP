#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;

vector<vector<int>> generate_random_matrix(int n) {
    vector<vector<int>> matrix;
    for (int i = 0; i < n; ++i) {
        vector<int> row(n);
        generate(row.begin(), row.end(), std::rand);
        matrix.push_back(row);
    }
    return matrix;
}

vector<vector<int>> generate_zero_matrix(int n) {
    vector<vector<int>> matrix;
    for (int i = 0; i < n; ++i) {
        vector<int> row(n, 0);
        matrix.push_back(row);
    }
    return matrix;
}

void multiply_single_ijk(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single ijk: " << (end - start).count() / retries << "\n";

}

void multiply_single_ikj(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {

    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < n; ++k) {
                for (int j = 0; j < n; ++j) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single ijk: " << (end - start).count() / retries << "\n";
}

void multiply_single_jik(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);

        for (int j = 0; j < n; ++j) {
            for (int i = 0; i < n; ++i) {
                for (int k = 0; k < n; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single jik: " << (end - start).count() / retries << "\n";
}


void multiply_single_jki(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                for (int i = 0; i < n; ++i) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single jik: " << (end - start).count() / retries << "\n";
}

void multiply_single_kij(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single kij: " << (end - start).count() / retries << "\n";
}

void multiply_single_kji(int n, vector<vector<int>>& a, vector<vector<int>>& b, int retries) {

    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                for (int i = 0; i < n; ++i) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "single kij: " << (end - start).count() / retries << "\n";
}

void multiply_parallel(int n, vector<vector<int>>& a, vector<vector<int>>& b, int treads, int retries) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int r = 0; r < retries; ++r) {
        auto c = generate_zero_matrix(n);

        #pragma omp parallel num_threads(treads) //shared(a,b,c) private(i,j,k)
        for (int i = 0; i < n; ++i) {
            #pragma omp for schedule(static)
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    cout << treads << ": " << (end - start).count() /retries << "\n";
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);

    srand(unsigned(time(nullptr)));

    auto a = generate_random_matrix(n);
    auto b = generate_random_matrix(n);

    multiply_single_ijk(n, a, b, 5);
    multiply_single_ikj(n, a, b, 5);
    multiply_single_jik(n, a, b, 5);
    multiply_single_jki(n, a, b, 5);
    multiply_single_kij(n, a, b, 5);
    multiply_single_kji(n, a, b, 5);

    for (int t = 1; t <= 10; ++t) {
        multiply_parallel(n, a, b, t, 5);
    }
}
