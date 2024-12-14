#define IMAGEM_PARA_MATRIZ

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace std;

typedef unsigned char BytePixel;
struct PixelRGB { // Struct que representa um pixel RGB

    BytePixel vermelho, verde, azul;

    bool operator<(const PixelRGB& outro) { return true; } // operadores de comparação entre pixels
    bool operator>(const PixelRGB& outro) { return true; }
    bool operator==(const PixelRGB& outro) {
        return this->vermelho == outro.vermelho && this->verde == outro.verde && this->azul == outro.azul;
    }

    // sobrecarga do operador de saída para exibir a soma dos valores RGB do pixel
    friend ostream& operator<<(ostream& saida, const PixelRGB& pixel) {
        saida << pixel.vermelho + pixel.verde + pixel.azul; // soma dos valores RGB
        return saida;
    }
};

// função que carrega o arquivo PGM
vector<vector<int>> lerPGM(const string& caminhoArquivo) {

    ifstream arquivoEntrada(caminhoArquivo, ios::binary);

    if (!arquivoEntrada.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo: " + caminhoArquivo); // exceção caso o arquivo não seja aberto
    }

    string formatoArquivo;
    arquivoEntrada >> formatoArquivo; // lê o formato do arquivo

    if (formatoArquivo != "P2" && formatoArquivo != "P5") {
        throw runtime_error("Formato de PGM não suportado: " + formatoArquivo); // exceção caso o arquivo não seja PGM
    }

    int larguraImagem, alturaImagem, valorMaximo;
    arquivoEntrada >> larguraImagem >> alturaImagem >> valorMaximo; // lê a largura, altura e o valor máximo (255)

    arquivoEntrada.ignore();

    vector<vector<int>> imagemEscalaCinza(alturaImagem, vector<int>(larguraImagem));

    if (formatoArquivo == "P2") {
        for (int linha = 0; linha < alturaImagem; linha++) {
            for (int coluna = 0; coluna < larguraImagem; coluna++) {
                arquivoEntrada >> imagemEscalaCinza[linha][coluna]; // lê os valores da imagem
            }
        }
    } else if (formatoArquivo == "P5") {
        for (int linha = 0; linha < alturaImagem; linha++) {
            for (int coluna = 0; coluna < larguraImagem; coluna++) {
                imagemEscalaCinza[linha][coluna] = arquivoEntrada.get(); // lê os valores da imagem
            }
        }
    }

    arquivoEntrada.close();

    return imagemEscalaCinza;
}

// função que converte a imagem PNG para PPM usando um script Python
void converterPNGparaPPM(const string& caminhoEntrada, const string& caminhoSaida) {

    string comando = "python3 png_to_ppm.py \"" + caminhoEntrada + ".png\" \"" + caminhoSaida + ".ppm\"";

    int resultadoComando = system(comando.c_str());

    if (resultadoComando != 0) {
        cerr << "Falha na conversão de PNG para PPM." << endl;
    }
}

// função que aplica o filtro Gaussiano em uma imagem (para suavização)
void aplicarFiltroGaussiano(const string& caminhoOrigem, const string& caminhoDestino, float sigmaFiltro) {

    string comando = "python3 gaussian.py \"" + caminhoOrigem + ".ppm\" \"" + caminhoDestino + ".ppm\" " + to_string(sigmaFiltro);

    int resultadoComando = system(comando.c_str());

    if (resultadoComando != 0) {
        cerr << "Falha na aplicação do filtro Gaussiano." << endl;
    }
}

// função para carregar arquivo PPM com pré-processamento
vector<vector<PixelRGB>> lerPPM(const string& caminhoArquivo, float sigmaGaussiano, bool habilitarDebug = false) {

    converterPNGparaPPM(caminhoArquivo, caminhoArquivo); // converte o arquivo PNG para PPM

    aplicarFiltroGaussiano(caminhoArquivo, caminhoArquivo, sigmaGaussiano); // aplica o filtro Gaussiano

    ifstream arquivoEntrada(caminhoArquivo + ".ppm", ios::binary); // abre o arquivo PPM

    if (!arquivoEntrada.is_open()) {
        throw runtime_error("Não foi possível abrir o arquivo: " + caminhoArquivo); 
    }

    string formatoArquivo;
    arquivoEntrada >> formatoArquivo;

    if (formatoArquivo != "P3" && formatoArquivo != "P6") {
        throw runtime_error("Formato de PPM não suportado: " + formatoArquivo);
    }

    int larguraImg, alturaImg, valorMaximo;
    arquivoEntrada >> larguraImg >> alturaImg >> valorMaximo;

    if (valorMaximo > 255) {
        throw runtime_error("Apenas arquivos PPM de 8 bits são suportados."); // exceção para arquivos PPM de 8 bits
    }

    arquivoEntrada.ignore();

    vector<vector<PixelRGB>> imagemRGB(alturaImg, vector<PixelRGB>(larguraImg));

    if (formatoArquivo == "P3") { // leitura do arquivo PPM no formato ASCII
        for (int linha = 0; linha < alturaImg; linha++) {
            for (int coluna = 0; coluna < larguraImg; coluna++) {

                int r, g, b;
                arquivoEntrada >> r >> g >> b;

                imagemRGB[linha][coluna] = {(BytePixel)r, (BytePixel)g, (BytePixel)b};
            }
        }
    } else if (formatoArquivo == "P6") { // leitura do arquivo PPM no formato binário
        for (int linha = 0; linha < alturaImg; linha++) {
            for (int coluna = 0; coluna < larguraImg; coluna++) {

                PixelRGB pixel {
                    .vermelho = (BytePixel)arquivoEntrada.get(),
                    .verde = (BytePixel)arquivoEntrada.get(),
                    .azul = (BytePixel)arquivoEntrada.get(),
                };

                imagemRGB[linha][coluna] = pixel;
            }
        }
    }

    arquivoEntrada.close();

    if (habilitarDebug) {
        cout << "Imagem carregada com sucesso: " << imagemRGB.size() << "x" << imagemRGB[0].size() << "\n"; // exibe a imagem carregada
    }

    return imagemRGB;
}


template<typename TipoElemento> // sobrecarga do operador de saída para exibir as matrizes
ostream& operator<<(ostream& saida, const vector<vector<TipoElemento>>& matriz) {
    for (const auto& linha : matriz) {
        for (const auto& elemento : linha) { // exibe cada elemento da matriz
            saida << elemento << " ";
        }
        saida << endl;
    }
    return saida; 
}

int main() {

    string arquivoOrigem = "church";
    string arquivoDestino = "output";

    try {
        converterPNGparaPPM(arquivoOrigem, arquivoDestino); // converte o arquivo PNG para PPM
    } catch (const exception& erro) {
        cerr << "Erro: " << erro.what() << endl; // exibe a mensagem de erro
        return 1;
    }

    return 0;
}
