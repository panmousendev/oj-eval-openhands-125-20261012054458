#include <cstdio>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Union-Find with coordinate compression for optimal performance
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        
        if (rank[rx] < rank[ry]) {
            parent[rx] = ry;
        } else if (rank[rx] > rank[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rank[rx]++;
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
        std::vector<int> vars;
        vars.reserve(n * 2);
        
        // Store constraints
        std::vector<std::tuple<int, int, int>> constraints;
        constraints.reserve(n);
        
        for (int i = 0; i < n; i++) {
            int x, y, e;
            scanf("%d %d %d", &x, &y, &e);
            vars.push_back(x);
            vars.push_back(y);
            constraints.emplace_back(x, y, e);
        }
        
        // Coordinate compression
        std::sort(vars.begin(), vars.end());
        vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
        
        std::unordered_map<int, int> comp;
        for (int i = 0; i < vars.size(); i++) {
            comp[vars[i]] = i;
        }
        
        // Process constraints with Union-Find
        UnionFind uf(vars.size());
        bool valid = true;
        
        // Process all constraints in order
        for (const auto& [x, y, e] : constraints) {
            int cx = comp[x], cy = comp[y];
            
            if (e == 1) {
                uf.unite(cx, cy);
            } else {
                if (uf.connected(cx, cy)) {
                    valid = false;
                    break;
                }
            }
        }
        
        printf("%s\n", valid ? "YES" : "NO");
    }
    
    return 0;
}