from PIL import Image
import sys

def convert_png_to_ppm(input_file, output_file):
    try:
        # Abrir a imagem PNG
        image = Image.open(input_file)
        
        # Converter a imagem para o modo RGB, se necessário
        if image.mode != 'RGB':
            image = image.convert('RGB')
        
        # Salvar como PPM
        image.save(output_file, format='PPM')
        print(f"Imagem convertida com sucesso: {output_file}")
    except Exception as e:
        print(f"Erro ao converter PNG para PPM: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python3 pngToPpm.py <arquivo_entrada.png> <arquivo_saida.ppm>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        convert_png_to_ppm(input_file, output_file)
