#include <cstdio>
#include <vector>
#include <unordered_map>

// Union-Find data structure for managing equivalence classes
class UnionFind {
private:
    std::unordered_map<int, int> parent;
    std::unordered_map<int, int> rank;
    
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
    int t;
    scanf("%d", &t);
    
    while (t--) {
        int n;
        scanf("%d", &n);
        
        // Process equality constraints first
        UnionFind uf;
        
        // Process all constraints in one pass
        bool valid = true;
        std::vector<std::pair<int, int>> inequalities;
        
        for (int i = 0; i < n; i++) {
            int x, y, e;
            scanf("%d %d %d", &x, &y, &e);
            
            if (e == 1) {
                uf.unite(x, y);
            } else {
                inequalities.push_back({x, y});
            }
        }
        
        // Check inequality constraints
        for (auto& p : inequalities) {
            if (uf.connected(p.first, p.second)) {
                valid = false;
                break;
            }
        }
        
        printf("%s\n", valid ? "YES" : "NO");
    }
    
    return 0;
}