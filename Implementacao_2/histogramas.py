from PIL import Image
import numpy as np

def calcular_histogramas(arquivo_entrada, mascara_obj, mascara_fundo):

    # Calcula histogramas para objeto e fundo com base nas máscaras fornecidas.
    
    #arquivo_entrada: Caminho da imagem PPM.
    #mascara_obj: Máscara binária indicando pixels do objeto (1).
    #mascara_fundo: Máscara binária indicando pixels do fundo (1).

    try:
        imagem = Image.open(arquivo_entrada)
        if imagem.format != "PPM":
            raise ValueError("O arquivo de entrada não é uma imagem PPM.")
        
        matriz_imagem = np.array(imagem)

        # Pixels do objeto e do fundo
        objeto_pixels = matriz_imagem[mascara_obj == 1]
        fundo_pixels = matriz_imagem[mascara_fundo == 1]

        histogramas = {}
        for i, canal in enumerate(['R', 'G', 'B']):
            hist_obj, _ = np.histogram(objeto_pixels[:, i], bins=256, range=(0, 255), density=True)
            hist_fundo, _ = np.histogram(fundo_pixels[:, i], bins=256, range=(0, 255), density=True)
            histogramas[canal] = {'obj': hist_obj, 'bkg': hist_fundo}

        return histogramas

    except Exception as erro:
        print(f"Erro ao calcular histogramas: {erro}")
        return None


def calcular_Rp(pixel, histogramas, tipo):
    # Calcula Rp para um pixel com base nos histogramas.
 
    R = 0
    for i, canal in enumerate(['R', 'G', 'B']):
        prob = histogramas[canal][tipo][pixel[i]]
        if prob > 0:
            R -= np.log(prob)
        else:
            R += np.inf  # Penalidade máxima para probabilidade zero
    return R
