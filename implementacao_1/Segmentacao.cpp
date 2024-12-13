
#include "Grafo.h" // incluindo o arquivo Segmentacao.h
#include "ConversorImagemMatriz.cpp" // incluindo o arquivo ConversorImagemMatriz.h
#include "UnionFind.hpp" // estrutura UnionFind para otimização
#include <algorithm> // biblioteca algorithm
using namespace std;


Grafo::Grafo(int numVertices, int numArestas, float limiar)
    : numVertices(numVertices), numArestas(numArestas), limiar(limiar), unionFind(numVertices)  {
    representantes.resize(numVertices); // redimensiona o vetor representantes para o tamanho numVertices
    tamanhos.resize(numVertices, 1);
    for (int i = 0; i < numVertices; ++i) {
        representantes[i] = i;
    }
    }


void Grafo::construcaoGrafo(const std::string path) {
    
}

void Grafo::ordenarArestas() { // ordena as arestas em ordem não decrescente de peso
    std::sort(arestas.begin(), arestas.end(), [](const Aresta& a, const Aresta& b) { // a funcao std::sort usa o lambda para comparar as arestas
        return a.peso < b.peso;
    });
}

void Grafo::unirComponentes(int vertice1, int vertice2, float pesoAresta) {
    int representante1 = unionFind.find(vertice1);
    int representante2 = unionFind.find(vertice2);

    if (representante1 != representante2) {
        float int1 = tamanhos[representante1];
        float int2 = tamanhos[representante2];
        float limiarAdaptativo1 = limiar / int1;
        float limiarAdaptativo2 = limiar / int2;

        // critério D(c1, c2)
        if (pesoAresta <= std::min(int1 + limiarAdaptativo1, int2 + limiarAdaptativo2)) {
            
            unionFind.unite(representante1, representante2); // Unindo componentes usando Union-Find

            // Atualizando o tamanho do novo componente
            int novoRepresentante = unionFind.find(representante1);
            tamanhos[novoRepresentante] = tamanhos[representante1] + tamanhos[representante2];
        }
    }
}


void Grafo::segmentar() {

    ordenarArestas(); // ordena as arestas em ordem não decrescente de peso com a funcao ordenarArestas

    // inicializando a estrutura Union-Find
    for (int i = 0; i < numVertices; ++i) {
        unionFind.find(i); 
    }

    // iterando sobre todas as arestas para segmentar a imagem
    for (const Aresta& aresta : arestas) {
        int origem = aresta.origem;
        int destino = aresta.destino;
        float peso = aresta.peso;

        //o UnionFind encontra o representante de cada vertice
        int representanteOrigem = unionFind.find(origem); 
        int representanteDestino = unionFind.find(destino); 

        if (representanteOrigem != representanteDestino) {
            float intOrigem = tamanhos[representanteOrigem];
            float intDestino = tamanhos[representanteDestino];
            float limiarAdaptativoOrigem = limiar / intOrigem;
            float limiarAdaptativoDestino = limiar / intDestino;

            //checando se o peso da aresta é menor ou igual à diferença interna mínima + o limiar
            if (peso <= std::min(intOrigem + limiarAdaptativoOrigem, intDestino + limiarAdaptativoDestino)) {
                // Merge components using Union-Find
                unionFind.unite(representanteOrigem, representanteDestino);

                // atualizando o tamanho do novo componente
                int novoRepresentante = unionFind.find(representanteOrigem);
                tamanhos[novoRepresentante] = tamanhos[representanteOrigem] + tamanhos[representanteDestino];
            }
        }
    }
}

float Grafo::calcularDiferencaInterna(int representante) {
    //A diferença interna de um componente é o peso máximo de uma aresta na MST do componente.
    //A MST é geralmente obtida com o algoritmo de Kruskal, 
    //mas aqui, a MST é representada pela lista de arestas do grafo em ordem nao decrescente de peso.
    float maxPeso = 0.0;
    for (const Aresta& aresta : arestas) {
        int origemRep = unionFind.find(aresta.origem);
        int destinoRep = unionFind.find(aresta.destino);
        if (origemRep == representante && destinoRep == representante) {
            if (aresta.peso > maxPeso) {
                maxPeso = aresta.peso;
            }
        }
    }
    return maxPeso;
}

float Grafo::calcularDiferencaEntreComponentes(int representante1, int representante2) {
    //a diferença entre componentes é o peso mínimo de uma aresta entre os dois componentes
    float minPeso = std::numeric_limits<float>::infinity();
    for (const Aresta& aresta : arestas) {
        int origemRep = unionFind.find(aresta.origem);
        int destinoRep = unionFind.find(aresta.destino);
        if ((origemRep == representante1 && destinoRep == representante2) ||
            (origemRep == representante2 && destinoRep == representante1)) {
            if (aresta.peso < minPeso) {
                minPeso = aresta.peso;
            }
        }
    }
    return minPeso;
}

void imprimirSegmentos(){
    //conectar com a função de salvar imagem segmentada colorida
}
