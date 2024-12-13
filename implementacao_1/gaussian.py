import cv2
import sys

def apply_gaussian_filter(input_file, output_file, sigma):
    try:
        # Ler a imagem PPM
        image = cv2.imread(input_file)
        
        if image is None:
            raise FileNotFoundError(f"Não foi possível carregar a imagem: {input_file}")
        
        # Aplicar o filtro Gaussiano
        kernel_size = int(2 * round(3 * sigma) + 1)  # Tamanho do kernel baseado em sigma
        blurred = cv2.GaussianBlur(image, (kernel_size, kernel_size), sigma)
        
        # Salvar a imagem resultante
        cv2.imwrite(output_file, blurred)
        print(f"Filtro Gaussiano aplicado com sucesso: {output_file}")
    except Exception as e:
        print(f"Erro ao aplicar o filtro Gaussiano: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Uso: python3 applyGaussian.py <arquivo_entrada.ppm> <arquivo_saida.ppm> <sigma>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        try:
            sigma = float(sys.argv[3])
        except ValueError:
            print("Sigma deve ser um número!")
            sys.exit(1)
        apply_gaussian_filter(input_file, output_file, sigma)
