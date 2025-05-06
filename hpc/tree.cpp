#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

// Generates a random tree with n nodes using an adjacency matrix
vector<vector<int>> generateTreeAdjMatrix(int n) {
    vector<vector<int>> adj(n, vector<int>(n, 0));
    srand(time(0));

    // Connect each node i (1 to n-1) with a random node j (0 to i-1) to form a
    // tree
    for (int i = 1; i < n; ++i) {
        int j = rand() % i; // Ensure no cycles
        adj[i][j] = 1;
        adj[j][i] = 1;
    }

    return adj;
}

// Function to print adjacency matrix
void printMatrix(const vector<vector<int>> &adj) {
    for (const auto &row : adj) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }
}

int main() {
    int n = 6; // Number of nodes
    auto tree = generateTreeAdjMatrix(n);

    cout << "Adjacency Matrix of the Tree:\n";
    printMatrix(tree);

    return 0;
}
