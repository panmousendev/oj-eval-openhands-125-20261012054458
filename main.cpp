#include <cstdio>
#include <vector>
#include <algorithm>

// Custom hash table for better performance than std::unordered_map
template<typename K, typename V>
class HashMap {
private:
    static constexpr int INITIAL_CAPACITY = 16;
    static constexpr float LOAD_FACTOR = 0.75f;
    
    struct Entry {
        K key;
        V value;
        bool occupied;
        
        Entry() : occupied(false) {}
    };
    
    std::vector<Entry> table;
    int size;
    int capacity;
    
    int hash(const K& key) const {
        return (key * 2654435761U) % capacity; // Fibonacci hash
    }
    
    void resize() {
        std::vector<Entry> old_table = std::move(table);
        int old_capacity = capacity;
        
        capacity *= 2;
        table.resize(capacity);
        std::fill(table.begin(), table.end(), Entry());
        size = 0;
        
        // Rehash all entries
        for (int i = 0; i < old_capacity; i++) {
            if (old_table[i].occupied) {
                (*this)[old_table[i].key] = old_table[i].value;
            }
        }
    }
    
public:
    HashMap() : size(0), capacity(INITIAL_CAPACITY) {
        table.resize(capacity);
    }
    
    V& operator[](const K& key) {
        // Find or create entry
        int i = hash(key);
        while (table[i].occupied && table[i].key != key) {
            i = (i + 1) % capacity; // Linear probing
        }
        
        if (!table[i].occupied) {
            table[i].key = key;
            table[i].occupied = true;
            size++;
            
            // Check load factor
            if (size >= capacity * LOAD_FACTOR) {
                resize();
                // Need to re-find after resize
                i = hash(key);
                while (table[i].occupied && table[i].key != key) {
                    i = (i + 1) % capacity;
                }
            }
        }
        
        return table[i].value;
    }
    
    void put(const K& key, const V& value) {
        (*this)[key] = value;
    }
    
    bool contains(const K& key) const {
        int i = hash(key);
        while (table[i].occupied) {
            if (table[i].key == key) {
                return true;
            }
            i = (i + 1) % capacity;
        }
        return false;
    }
    
    int get_size() const {
        return size;
    }
};

// Union-Find data structure
class UnionFind {
private:
    HashMap<int, int> parent;
    HashMap<int, int> rank;
    
public:
    int find(int x) {
        if (!parent.contains(x)) {
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