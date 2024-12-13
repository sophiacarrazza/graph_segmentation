#include <vector> // biblioteca vector
#include <string>

class UnionFind {
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) { // Construtor
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        // Retorna o representante do conjunto ao qual um v pertence
        // Ajusta a estrutura da árvore para que todos os v no caminho apontem diretamente para o representante.
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Compressão de caminho
        }
        return parent[x];
    }

    void unite(int x, int y) { // Une dois conjuntos
    // Compara os ranks dos representantes dos dois conjuntos e anexa a árvore menor à maior. 
    // Se os ranks forem iguais, um dos ranks é incrementado.

        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            // União por rank
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};
