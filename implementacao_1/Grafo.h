#ifndef GRAFO_H // Verifica se a variável GRAFO_H não foi definida
#define GRAFO_H // Define a variável GRAFO_H


#include <vector> // biblioteca vector
#include <string>

struct Aresta {
    int origem, destino;
    float peso;
};

class Grafo {
    private:
        std::vector<Aresta> arestas;
        std::vector<int> representantes;
        std::vector<int> tamanhos;
        int numVertices;
        int numArestas;
        int numSegmentos;
        float limiar; // Limiar para a segmentação (limites de similaridade)
        float sigma; // para a suavização
        int minSize; // tamanho mínimo de um segmento (para previnir segmentos muito pequenos)
        UnionFind unionFind; // estrutura Union-Find para otimização

    public:
        Grafo(int numVertices, int numArestas, float limiar);
        void adicionarAresta(int origem, int destino, float peso);
        void construcaoGrafo(const std::string path);
        void ordenarArestas();
        void unirComponentes(int vertice1, int vertice2, float pesoAresta);
        void segmentar();
        float calcularDiferencaInterna(int representante);
        float calcularDiferencaEntreComponentes(int representante1, int representante2);
        void imprimirSegmentos();

};

#endif // GRAFO_H