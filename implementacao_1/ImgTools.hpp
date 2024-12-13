#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;

class ImgTools
{
private:
    /* data */
public:

    // Função para gerar a imagem com novas cores
    void gerarImagemComCores(int largura, int altura, const std::vector<int>& imagem, const std::vector<int>& componentes, const std::string& nomeArquivo) {
        // Número máximo de componentes
        int numComponentes = *std::max_element(componentes.begin(), componentes.end()) + 3;//1;
        
        // Gerar uma cor aleatória para cada componente
        std::vector<cv::Vec3b> cores(numComponentes);
        for (int i = 0; i < numComponentes; ++i) {
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;
            cores[i] = cv::Vec3b(b, g, r); // OpenCV usa BGR
        }
        
        // Criar uma imagem de saída
        cv::Mat imagemSegmentada(altura, largura, CV_8UC3); // Matriz de 8 bits por 3 canais (RGB)
        
        // Atribuir cores aos pixels de acordo com seus componentes
        for (int y = 0; y < altura; ++y) {
            for (int x = 0; x < largura; ++x) {
                int indicePixel = y * largura + x;
                int componente = componentes[indicePixel];
                
                //------------------------REMOVER_PARA_SEGMENTACAO------------------------//
                if (imagem[y * largura + x] > 150)
                {
                    imagemSegmentada.at<cv::Vec3b>(y, x) = cores[2];
                }else{
                    imagemSegmentada.at<cv::Vec3b>(y, x) = cores[1];
                }
                //------------------------REMOVER_PARA_SEGMENTACAO------------------------//
                
                //imagemSegmentada.at<cv::Vec3b>(y, x) = cores[componente];
            }
        }
        
        // Salvar a imagem segmentada
        cv::imwrite(nomeArquivo, imagemSegmentada);
    }


    // Função para carregar uma imagem e convertê-la em um vetor de pixels em escala de cinza
    std::vector<int> carregarImagemEmVetor(const std::string& nomeArquivo, int& largura, int& altura) {
        // Carregar a imagem usando o OpenCV
        cv::Mat imagem = cv::imread(nomeArquivo, cv::IMREAD_GRAYSCALE); // Carregar como imagem em escala de cinza

        // Verificar se a imagem foi carregada corretamente
        if (imagem.empty()) {
            std::cerr << "Erro ao carregar a imagem!" << std::endl;
            exit(-1);
        }

        // Obter as dimensões da imagem
        largura = imagem.cols;
        altura = imagem.rows;

        // Criar um vetor para armazenar os pixels
        std::vector<int> pixels(largura * altura);

        // Preencher o vetor com os valores de intensidade dos pixels da imagem
        for (int y = 0; y < altura; ++y) {
            for (int x = 0; x < largura; ++x) {
                pixels[y * largura + x] = imagem.at<uchar>(y, x); // Valor de intensidade do pixel
                //cout << "pixels: " << pixels[y * largura + x] << endl;
            }
        }

        return pixels;
    }

    
};

/*
int main() {
    ImgTools it = ImgTools();
    int largura = 798; 
    int altura = 1200;
    
    // Nome do arquivo da imagem
    std::string nomeImagem = "./base/Anime-Evangelion-Asuka-Langley-EVA-02-989343.png";  // Substitua pelo caminho da sua imagem

    // Carregar a imagem e convertê-la para vetor de pixels
    std::vector<int> imagem = it.carregarImagemEmVetor(nomeImagem, largura, altura);

    // Vetor para armazenar os componentes
    std::vector<int> componentes(largura * altura);
    
    // Realiza a segmentação
    //it.segmentarImagem(largura, altura, imagem, componentes);
    
    // Gera e salva a imagem segmentada com novas cores
    it.gerarImagemComCores(largura, altura, imagem, componentes, "imagem_segmentada.jpg");
    //salvarVetorComoImagem(imagem, largura, altura, "imagem_recuperada.jpg");
    
    return 0;
}*/
//g++ -o a cod5.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc `pkg-config --cflags --libs opencv4`