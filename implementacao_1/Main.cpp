#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <fstream>
#include <sstream>

struct Edge {
    int u, v; // Nós conectados pela aresta
    float weight; // Peso da aresta
};

struct Component {
    int parent;
    int size;
    float max_weight; // Diferença interna
};

// Função de comparação para ordenar as arestas
bool compareEdges(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

// Encontrar o componente raiz (Union-Find)
int findRoot(std::vector<Component> &components, int x) {
    if (components[x].parent != x) {
        components[x].parent = findRoot(components, components[x].parent);
    }
    return components[x].parent;
}

// União de dois componentes (Union-Find)
void mergeComponents(std::vector<Component> &components, int u, int v, float weight) {
    int rootU = findRoot(components, u);
    int rootV = findRoot(components, v);
    
    if (rootU != rootV) {
        if (components[rootU].size < components[rootV].size) {
            std::swap(rootU, rootV);
        }
        components[rootV].parent = rootU;
        components[rootU].size += components[rootV].size;
        components[rootU].max_weight = std::max(components[rootU].max_weight, weight);
    }
}

// Função para ler a imagem PPM (ASCII)
bool loadPPM(const std::string &filename, std::vector<std::vector<std::vector<int>>> &image, int &width, int &height) {
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo PPM!" << std::endl;
        return false;
    }
    
    // Ignorar o cabeçalho
    std::getline(file, line); // P6
    std::getline(file, line); // Comentário (se houver)
    while (line[0] == '#') std::getline(file, line);
    std::istringstream size_stream(line);
    
    size_stream >> width >> height;
    
    std::getline(file, line); // Máximo valor (geralmente 255)
    
    // Criar a matriz de imagem (cada pixel com 3 componentes: R, G, B)
    image.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            file.read(reinterpret_cast<char*>(&image[y][x][0]), 3);
        }
    }
    
    return true;
}

// Segmentação baseada no algoritmo de Kruskal
std::vector<int> segmentImage(const std::vector<std::vector<std::vector<int>>> &image, int width, int height, float k) {
    int num_pixels = width * height;

    // Construção das arestas
    std::vector<Edge> edges;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int u = y * width + x;
            if (x < width - 1) { // Conectar à direita
                int v = u + 1;
                float weight = 0;
                for (int c = 0; c < 3; ++c) {
                    weight += std::abs(image[y][x][c] - image[y][x + 1][c]);
                }
                edges.push_back({u, v, weight});
            }
            if (y < height - 1) { // Conectar abaixo
                int v = u + width;
                float weight = 0;
                for (int c = 0; c < 3; ++c) {
                    weight += std::abs(image[y][x][c] - image[y + 1][x][c]);
                }
                edges.push_back({u, v, weight});
            }
        }
    }

    // Ordenar as arestas por peso
    std::sort(edges.begin(), edges.end(), compareEdges);

    // Inicializar os componentes
    std::vector<Component> components(num_pixels);
    for (int i = 0; i < num_pixels; ++i) {
        components[i] = {i, 1, 0.0f}; // Cada pixel é um componente inicialmente
    }

    // Algoritmo de Kruskal com critérios de segmentação
    for (const auto &edge : edges) {
        int u_root = findRoot(components, edge.u);
        int v_root = findRoot(components, edge.v);

        if (u_root != v_root) {
            float threshold_u = components[u_root].max_weight + k / components[u_root].size;
            float threshold_v = components[v_root].max_weight + k / components[v_root].size;
            
            if (edge.weight <= std::min(threshold_u, threshold_v)) {
                mergeComponents(components, u_root, v_root, edge.weight);
            }
        }
    }

    // Rotular os componentes finais
    std::vector<int> labels(num_pixels);
    for (int i = 0; i < num_pixels; ++i) {
        labels[i] = findRoot(components, i);
    }

    return labels;
}

// Função para salvar imagem segmentada em PPM
void savePPM(const std::string &filename, const std::vector<std::vector<std::vector<int>>> &image, const std::vector<int> &labels, int width, int height) {
    std::ofstream file(filename);
    
    file << "P6\n";
    file << "# Segmented Image\n";
    file << width << " " << height << "\n";
    file << "255\n";
    
    std::map<int, std::vector<int>> color_map;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int label = labels[y * width + x];
            if (color_map.find(label) == color_map.end()) {
                color_map[label] = {rand() % 256, rand() % 256, rand() % 256};
            }
            file.write(reinterpret_cast<const char*>(&color_map[label][0]), 3);
        }
    }
}

int main() {
    // Carregar a imagem PPM
    int width, height;
    std::vector<std::vector<std::vector<int>>> image;
    if (!loadPPM("output.ppm", image, width, height)) {
        return -1;
    }

    // Segmentar a imagem
    float k = 300.0f; // Parâmetro de escala
    std::vector<int> labels = segmentImage(image, width, height, k);

    // Salvar a imagem segmentada
    savePPM("imagem_segmentada.ppm", image, labels, width, height);

    std::cout << "Segmentação concluída e imagem salva como imagem_segmentada.ppm!" << std::endl;
    return 0;
}
