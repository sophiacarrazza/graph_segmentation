
#include "Grafo.h" // incluindo o arquivo Segmentacao.h
#include "ConversorImagemMatriz.cpp" // incluindo o arquivo ConversorImagemMatriz.h
#include <algorithm> // biblioteca algorithm
using namespace std;


Grafo::Grafo(int numVertices, int numArestas, float limiar)
    : numVertices(numVertices), numArestas(numArestas), limiar(limiar) {
    representantes.resize(numVertices); // redimensiona o vetor representantes para o tamanho numVertices
    tamanhos.resize(numVertices, 1);
    for (int i = 0; i < numVertices; ++i) {
        representantes[i] = i;
    }
    }


void Grafo::construcaoGrafo(const std::string path) {
    
}



void Grafo::unirComponentes(int c1, int c2) {
    // Unir componentes c1 e c2
}

void Grafo::segmentar() {
    // construir grafo
    //vector<Aresta> arestas = contrucaoGrafo(imagem);

    // ordenar arestas
    sort(arestas.begin(), arestas.end(), [](Aresta a, Aresta b) {
        return a.peso < b.peso;
    });
    // Segmentar


}

