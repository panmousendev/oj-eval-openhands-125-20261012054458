#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_map>

// Union-Find with coordinate compression for better performance
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int n;
    
public:
    UnionFind(int size) : n(size) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
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
        
        // Collect all variables for coordinate compression
        std::vector<int> variables;
        std::vector<std::pair<int, int>> equals;
        std::vector<std::pair<int, int>> not_equals;
        
        for (int i = 0; i < n; i++) {
            int x, y, e;
            scanf("%d %d %d", &x, &y, &e);
            
            variables.push_back(x);
            variables.push_back(y);
            
            if (e == 1) {
                equals.push_back({x, y});
            } else {
                not_equals.push_back({x, y});
            }
        }
        
        // Coordinate compression
        std::sort(variables.begin(), variables.end());
        auto last = std::unique(variables.begin(), variables.end());
        variables.erase(last, variables.end());
        
        // Create mapping from original variable to compressed index
        std::unordered_map<int, int> comp_map;
        for (int i = 0; i < variables.size(); i++) {
            comp_map[variables[i]] = i;
        }
        
        // Process equality constraints using Union-Find with compressed coordinates
        UnionFind uf(variables.size());
        
        for (auto& p : equals) {
            int x = comp_map[p.first];
            int y = comp_map[p.second];
            uf.unite(x, y);
        }
        
        // Check inequality constraints
        bool valid = true;
        for (auto& p : not_equals) {
            int x = comp_map[p.first];
            int y = comp_map[p.second];
            if (uf.connected(x, y)) {
                valid = false;
                break;
            }
        }
        
        printf("%s\n", valid ? "YES" : "NO");
    }
    
    return 0;
}