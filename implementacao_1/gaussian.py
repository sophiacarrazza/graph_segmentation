import sys
from scipy.ndimage import gaussian_filter
from PIL import Image
import numpy as np

def aplicar_desfoque_gaussiano(arquivo_entrada, arquivo_saida, desvio_padrao):
    try:
        imagem = Image.open(arquivo_entrada) # abre a imagem PPM

        if imagem.format != "PPM":
            raise ValueError("O arquivo de entrada não é uma imagem PPM.")

        matriz_imagem = np.array(imagem) # converte a imagem para um array NumPy (matriz de pixels)

        matriz_desfocada = np.zeros_like(matriz_imagem) # cria uma matriz de zeros com o mesmo formato da imagem
        for camada in range(matriz_imagem.shape[2]): # para cada camada de cor (R, G, B)
            matriz_desfocada[:, :, camada] = gaussian_filter(matriz_imagem[:, :, camada], sigma=desvio_padrao) # aplica o desfoque gaussiano

        imagem_desfocada = Image.fromarray(matriz_desfocada, mode="RGB") # cria uma imagem RGB a partir do array desfocado (cria um objeto Image)

        imagem_desfocada.save(arquivo_saida, format="PPM") # salva a imagem desfocada
        print(f"Imagem desfocada salva em {arquivo_saida}")

    except Exception as erro:
        print(f"Erro: {erro}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Uso: python gaussian_blur.py <arquivo_entrada.ppm> <arquivo_saida.ppm> <desvio_padrao>") # mensagem de erro
        sys.exit(1)

    arquivo_entrada = sys.argv[1]
    arquivo_saida = sys.argv[2]
    desvio_padrao = float(sys.argv[3]) # converte o desvio padrão para float

    aplicar_desfoque_gaussiano(arquivo_entrada, arquivo_saida, desvio_padrao)
