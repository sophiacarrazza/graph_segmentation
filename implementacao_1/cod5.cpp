#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>

using namespace std;

class ImgTools
{
private:
    /* data */
public:

    // Função para gerar a imagem com novas cores
    void gerarImagemComCores(int largura, int altura, const std::vector<int>& imagem, const std::vector<int>& componentes, const std::string& nomeArquivo) {
        // Número máximo de componentes
        int numComponentes = *std::max_element(componentes.begin(), componentes.end()) + 1;

        // Gerar uma cor aleatória para cada componente
        std::vector<std::vector<int>> cores(numComponentes, std::vector<int>(3)); // RGB
        for (int i = 0; i < numComponentes; ++i) {
            cout << numComponentes << endl;
            cores[i][0] = rand() % 256; // R
            cores[i][1] = rand() % 256; // G
            cores[i][2] = rand() % 256; // B
        }

        // Criar uma imagem de saída (representada como vetor)
        std::vector<int> imagemSegmentada(largura * altura * 3);

        // Atribuir cores aos pixels de acordo com seus componentes
        for (int y = 0; y < altura; ++y) {
            for (int x = 0; x < largura; ++x) {
                int indicePixel = (y * largura + x) * 3;
                int componente = componentes[y * largura + x];

                //------------------------REMOVER_PARA_SEGMENTACAO------------------------//
                if (imagem[y * largura + x] > 150) {
                    imagemSegmentada[indicePixel] = cores[2][0];   // Red
                    imagemSegmentada[indicePixel + 1] = cores[2][1]; // Green
                    imagemSegmentada[indicePixel + 2] = cores[2][2]; // Blue
                } else {
                    imagemSegmentada[indicePixel] = cores[1][0];   // Red
                    imagemSegmentada[indicePixel + 1] = cores[1][1]; // Green
                    imagemSegmentada[indicePixel + 2] = cores[1][2]; // Blue
                }
                //------------------------REMOVER_PARA_SEGMENTACAO------------------------//

                //imagemSegmentada[indicePixel] = cores[componente][0];
                //imagemSegmentada[indicePixel + 1] = cores[componente][1];
                //imagemSegmentada[indicePixel + 2] = cores[componente][2];
            }
        }

        // Salvar a imagem segmentada em formato PPM (Portable PixMap)
        salvarImagemPPM(nomeArquivo, largura, altura, imagemSegmentada);
    }

    // Função para carregar uma imagem em escala de cinza (PPM)
    std::vector<int> carregarImagemEmVetor(const std::string& nomeArquivo, int& largura, int& altura) {
        // Carregar a imagem PPM
        ifstream arquivo(nomeArquivo, ios::binary);
        if (!arquivo) {
            std::cerr << "Erro ao carregar a imagem!" << std::endl;
            exit(-1);
        }

        std::string tipoImagem;
        arquivo >> tipoImagem;

        // Verificar se o arquivo é do tipo PPM
        if (tipoImagem != "P6") {
            std::cerr << "Formato de imagem inválido! Somente PPM é suportado." << std::endl;
            exit(-1);
        }

        arquivo >> largura >> altura;
        int maxValor;
        arquivo >> maxValor;
        arquivo.ignore(); // Ignorar o caractere de nova linha após os metadados

        // Criar um vetor para armazenar os valores dos pixels em escala de cinza
        std::vector<int> pixels(largura * altura);

        // Ler os dados de pixels em RGB e converter para escala de cinza
        for (int i = 0; i < largura * altura; ++i) {
            unsigned char r, g, b;
            arquivo.read(reinterpret_cast<char*>(&r), 1);
            arquivo.read(reinterpret_cast<char*>(&g), 1);
            arquivo.read(reinterpret_cast<char*>(&b), 1);

            // Converter para escala de cinza utilizando a fórmula média ponderada
            pixels[i] = static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
        }

        return pixels;
    }

	void salvarVetorComoImagem(const std::vector<int>& pixels, int largura, int altura, const std::string& nomeArquivo) {
    // Abrir o arquivo para escrita
    std::ofstream arquivo(nomeArquivo, std::ios::binary);
    if (!arquivo) {
        std::cerr << "Erro ao salvar a imagem!" << std::endl;
        return;
    }

    // Escrever o cabeçalho PPM (P6)
    arquivo << "P6\n"; // Indica que é uma imagem PPM em formato binário
    arquivo << largura << " " << altura << "\n"; // Largura e altura da imagem
    arquivo << "255\n"; // Valor máximo do pixel (escala de 0 a 255)

    // Escrever os valores dos pixels em formato binário
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int indice = y * largura + x;
            unsigned char valorPixel = static_cast<unsigned char>(pixels[indice]);

            // Escrever os três componentes de cada pixel (R, G, B), todos com o mesmo valor (escala de cinza)
            arquivo.write(reinterpret_cast<char*>(&valorPixel), 1);  // Red
            arquivo.write(reinterpret_cast<char*>(&valorPixel), 1);  // Green
            arquivo.write(reinterpret_cast<char*>(&valorPixel), 1);  // Blue
        }
    }

    // Fechar o arquivo após salvar
    arquivo.close();
    std::cout << "Imagem salva com sucesso em: " << nomeArquivo << std::endl;
}

    // Função para salvar a imagem no formato PPM
    void salvarImagemPPM(const std::string& nomeArquivo, int largura, int altura, const std::vector<int>& imagem) {
        ofstream arquivo(nomeArquivo, ios::binary);
        if (!arquivo) {
            std::cerr << "Erro ao salvar a imagem!" << std::endl;
            exit(-1);
        }

        // Escrever cabeçalho PPM
        arquivo << "P6\n";
        arquivo << largura << " " << altura << "\n";
        arquivo << "255\n";

        // Escrever os pixels da imagem
        for (int i = 0; i < largura * altura; ++i) {
            unsigned char r = imagem[i * 3];     // Red
            unsigned char g = imagem[i * 3 + 1]; // Green
            unsigned char b = imagem[i * 3 + 2]; // Blue
            arquivo.write(reinterpret_cast<char*>(&r), 1);
            arquivo.write(reinterpret_cast<char*>(&g), 1);
            arquivo.write(reinterpret_cast<char*>(&b), 1);
        }
    }

};

int main() {
    // Exemplo de uso da classe ImgTools
    ImgTools imgTools;
    int largura, altura;
    std::vector<int> imagem = imgTools.carregarImagemEmVetor("./base/EVA-02.ppm", largura, altura);
    std::vector<int> componentes(largura * altura, 3); // Exemplo simples: todos os pixels no componente 0
    imgTools.gerarImagemComCores(largura, altura, imagem, componentes, "./saida/imagem_segmentada.ppm"); // com as cores
	//imgTools.salvarVetorComoImagem(imagem, largura, altura, "./saida/Bw.ppm"); // Em preto e branco

    return 0;
}
