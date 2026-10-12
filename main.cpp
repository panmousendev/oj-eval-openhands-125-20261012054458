#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Union-Find data structure for managing equivalence classes
class UnionFind {
private:
    unordered_map<int, int> parent;
    unordered_map<int, int> rank;
    
public:
    int find(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rank[x] = 0;
            return x;
        }
        
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return;
        
        // Union by rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
    
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while (t--) {
        int n;
        cin >> n;
        
        // Store equality and inequality constraints
        vector<pair<int, int>> equals;
        vector<pair<int, int>> not_equals;
        
        for (int i = 0; i < n; i++) {
            int x, y, e;
            cin >> x >> y >> e;
            
            if (e == 1) {
                equals.push_back({x, y});
            } else {
                not_equals.push_back({x, y});
            }
        }
        
        // Process equality constraints using Union-Find
        UnionFind uf;
        
        for (auto& p : equals) {
            uf.unite(p.first, p.second);
        }
        
        // Check if any inequality constraint violates the equivalence classes
        bool valid = true;
        for (auto& p : not_equals) {
            if (uf.connected(p.first, p.second)) {
                valid = false;
                break;
            }
        }
        
        cout << (valid ? "YES" : "NO") << '\n';
    }
    
    return 0;
}