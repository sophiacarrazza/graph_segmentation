#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

struct Edge {
    int u, v;
    float weight;
};

struct Component {
    int root;  // Raiz do componente
    int size;  // Tamanho do componente
    float max_weight; // Peso máximo do componente
    int color;  // Cor ou identificador único do componente
};

// Função para encontrar a raiz de um componente
int findRoot(std::vector<Component>& components, int u) {
    if (components[u].root != u) {
        components[u].root = findRoot(components, components[u].root);
    }
    return components[u].root;
}

// Função para mesclar dois componentes
void mergeComponents(std::vector<Component>& components, int u_root, int v_root, float weight, int new_color) {
    // Mesclar o componente menor no maior
    if (components[u_root].size < components[v_root].size) {
        std::swap(u_root, v_root);
    }

    // Atualizar a raiz e o tamanho do componente
    components[v_root].root = u_root;
    components[u_root].size += components[v_root].size;
    components[u_root].max_weight = std::max(components[u_root].max_weight, weight);

    // Atribuir uma cor única ao componente resultante
    components[u_root].color = new_color;  // Novo identificador de cor
}

// Função de comparação para ordenar as arestas
bool compareEdges(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

// Função para segmentação de imagem
std::vector<int> segmentImage(const std::vector<std::vector<std::vector<int>>>& image, int width, int height, float k) {
    int num_pixels = width * height;
    std::vector<Edge> edges;

    // Construir as arestas com base na diferença de cor entre pixels adjacentes
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int u = y * width + x;

            // Conectar à direita
            if (x < width - 1) {
                int v = u + 1;
                float dr = image[y][x][0] - image[y][x + 1][0]; // Diferença no canal R
                float dg = image[y][x][1] - image[y][x + 1][1]; // Diferença no canal G
                float db = image[y][x][2] - image[y][x + 1][2]; // Diferença no canal B
                float weight = std::sqrt(dr * dr + dg * dg + db * db);
                if (weight < 4.0f) {
                    weight = 0.0f; // Marcar como iguais
                }
                edges.push_back({u, v, weight});
            }

            // Conectar abaixo
            if (y < height - 1) {
                int v = u + width;
                float dr = image[y][x][0] - image[y + 1][x][0]; 
                float dg = image[y][x][1] - image[y + 1][x][1];
                float db = image[y][x][2] - image[y + 1][x][2];
                float weight = std::sqrt(dr * dr + dg * dg + db * db);
                if (weight < 4.0f) {
                    weight = 0.0f;
                }
                edges.push_back({u, v, weight});
            }

            // Conectar na diagonal inferior direita
            if (x < width - 1 && y < height - 1) {
                int v = u + width + 1;
                float dr = image[y][x][0] - image[y + 1][x + 1][0]; 
                float dg = image[y][x][1] - image[y + 1][x + 1][1];
                float db = image[y][x][2] - image[y + 1][x + 1][2];
                float weight = std::sqrt(dr * dr + dg * dg + db * db);
                if (weight < 4.0f) {
                    weight = 0.0f;
                }
                edges.push_back({u, v, weight});
            }

            // Conectar na diagonal inferior esquerda
            if (x > 0 && y < height - 1) {
                int v = u + width - 1;
                float dr = image[y][x][0] - image[y + 1][x - 1][0]; 
                float dg = image[y][x][1] - image[y + 1][x - 1][1];
                float db = image[y][x][2] - image[y + 1][x - 1][2];
                float weight = std::sqrt(dr * dr + dg * dg + db * db);
                if (weight < 4.0f) {
                    weight = 0.0f;
                }
                edges.push_back({u, v, weight});
            }
        }
    }

    // Ordenar as arestas por peso
    std::sort(edges.begin(), edges.end(), compareEdges);

    // Inicializar os componentes com cores únicas
    std::vector<Component> components(num_pixels);
    for (int i = 0; i < num_pixels; ++i) {
        components[i] = {i, 1, 0.0f, i}; // Cada pixel é um componente inicialmente
    }

    // Algoritmo de Kruskal para segmentação com critérios de fusão
    int color_counter = num_pixels;
    for (const auto& edge : edges) {
        int u_root = findRoot(components, edge.u);
        int v_root = findRoot(components, edge.v);

        if (u_root != v_root) {
            float threshold_u = components[u_root].max_weight + k / components[u_root].size;
            float threshold_v = components[v_root].max_weight + k / components[v_root].size;
            
            if (edge.weight <= std::min(threshold_u, threshold_v)) {
                mergeComponents(components, u_root, v_root, edge.weight, color_counter++);
            }
        }
    }

    // Atribuir o rótulo final com base na cor dos componentes
    std::vector<int> labels(num_pixels);
    for (int i = 0; i < num_pixels; ++i) {
        labels[i] = components[findRoot(components, i)].color;
    }

    return labels;
}


bool loadPPM(const std::string &filename, std::vector<std::vector<std::vector<int>>> &image, int &width, int &height) {
    std::ifstream file(filename, std::ios::binary); // Abre o arquivo no modo binário
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo PPM!" << std::endl;
        return false;
    }

    std::string header;
    file >> header;
    if (header != "P6") {
        std::cerr << "Formato PPM não suportado! Esperado: P6." << std::endl;
        return false;
    }

    // Ignorar comentários
    char nextChar;
    file.get(nextChar);
    while (nextChar == '#') {
        std::string comment;
        std::getline(file, comment);
        file.get(nextChar);
    }
    file.unget(); // Retorna um caractere para o stream

    // Ler dimensões e valor máximo
    int max_value;
    file >> width >> height >> max_value;
    file.get(); // Consome o caractere de nova linha após o cabeçalho

    if (width <= 0 || height <= 0 || max_value != 255) {
        std::cerr << "Dimensões ou valor máximo inválido no arquivo PPM!" << std::endl;
        return false;
    }

    // Inicializar a imagem
    image.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));

    // Lê os dados RGB
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char rgb[3];
            if (!file.read(reinterpret_cast<char *>(rgb), 3)) {
                std::cerr << "Erro ao ler dados RGB do arquivo PPM!" << std::endl;
                return false;
            }
            image[y][x][0] = rgb[0];
            image[y][x][1] = rgb[1];
            image[y][x][2] = rgb[2];
        }
    }

    return true;
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
    if (!loadPPM("input.ppm", image, width, height)) {
        return -1;
    }

    // Segmentar a imagem
    float k = 500.0f;
    std::vector<int> labels = segmentImage(image, width, height, k);

    // Salvar a imagem segmentada
    savePPM("imagem_segmentada.ppm", image, labels, width, height);

    std::cout << "Segmentação concluída e imagem salva como imagem_segmentada.ppm!" << std::endl;
    return 0;
}
