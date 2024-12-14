#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "graph.h" // Inclua a biblioteca MaxFlow
#include "maxflow.cpp" 
// Estrutura para armazenar os pixels da imagem
struct Pixel {
    unsigned char r, g, b;
};

// Função para carregar a imagem PPM
std::vector<Pixel> loadPPM(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir a imagem.");
    }

    std::string format;
    file >> format; // Formato (P6)
    if (format != "P6") {
        throw std::runtime_error("Formato não suportado (deve ser P6).");
    }

    file >> width >> height; // Dimensões
    int maxVal;
    file >> maxVal; // Valor máximo de cor (deve ser 255)
    file.ignore(); // Ignorar o caractere de nova linha após o cabeçalho

    std::vector<Pixel> image(width * height);
    file.read(reinterpret_cast<char*>(image.data()), width * height * 3);
    return image;
}

// Função para salvar a imagem segmentada em PPM
void savePPM(const std::string& filename, const std::vector<bool>& segmentation, int width, int height) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível salvar a imagem.");
    }

    file << "P6\n" << width << " " << height << "\n255\n";

    for (bool segment : segmentation) {
        unsigned char color = segment ? 255 : 0; // Branco para o objeto, preto para o fundo
        file.put(color).put(color).put(color);
    }
}

// Função para calcular a diferença de intensidade entre pixels
double intensityDifference(const Pixel& p1, const Pixel& p2) {
    return std::sqrt(std::pow(p1.r - p2.r, 2) + std::pow(p1.g - p2.g, 2) + std::pow(p1.b - p2.b, 2));
}

// Função principal para segmentação usando cortes de grafos
std::vector<bool> graphCutSegmentation(const std::vector<Pixel>& image, int width, int height) {
    const int numPixels = width * height;
    const double lambda = 10.0; // Peso para o termo regional
    const double sigma = 30.0; // Parâmetro para penalizar diferenças de intensidade

    // Grafo do MaxFlow
    Graph<double, double, double> graph(numPixels, numPixels * 4);

    // Adiciona nós ao grafo
    for (int i = 0; i < numPixels; ++i) {
        graph.add_node();
    }

    // Adiciona arestas (n-links) baseadas em diferenças de intensidade
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            if (x < width - 1) { // Conecta ao pixel à direita
                int neighborIndex = y * width + (x + 1);
                double weight = std::exp(-intensityDifference(image[index], image[neighborIndex]) / (2 * sigma * sigma));
                graph.add_edge(index, neighborIndex, weight, weight);
            }
            if (y < height - 1) { // Conecta ao pixel abaixo
                int neighborIndex = (y + 1) * width + x;
                double weight = std::exp(-intensityDifference(image[index], image[neighborIndex]) / (2 * sigma * sigma));
                graph.add_edge(index, neighborIndex, weight, weight);
            }
        }
    }

    // Adiciona arestas para a fonte e o sorvedouro (t-links)
    for (int i = 0; i < numPixels; ++i) {
        double sourceWeight = lambda * (image[i].r < 128 ? 1.0 : 0.0); // Exemplo: fundo se intensidade < 128
        double sinkWeight = lambda * (image[i].r >= 128 ? 1.0 : 0.0); // Exemplo: objeto se intensidade >= 128
        graph.add_tweights(i, sourceWeight, sinkWeight);
    }

    // Calcula o corte mínimo
    graph.maxflow();

    // Gera a segmentação com base no corte
    std::vector<bool> segmentation(numPixels);
    for (int i = 0; i < numPixels; ++i) {
        segmentation[i] = (graph.what_segment(i) == Graph<double, double, double>::SOURCE);
    }

    return segmentation;
}

int main() {
    try {
        int width, height;
        std::vector<Pixel> image = loadPPM("input.ppm", width, height);
        std::vector<bool> segmentation = graphCutSegmentation(image, width, height);
        savePPM("output.ppm", segmentation, width, height);

        std::cout << "Segmentação concluída! Verifique o arquivo 'output.ppm'.\n";
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
