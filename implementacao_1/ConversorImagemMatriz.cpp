
#define IMAGE_TO_MATRIX

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace std;

typedef unsigned char Byte;

// clear && g++ readingPPM.cc -std=c++20 && ./a.out

struct Pixel {

	Byte R, G, B;

	bool operator<(const Pixel& p) { return true; }
	bool operator>(const Pixel& p) { return true; }
	bool operator == (const Pixel& p) {
		return this->R == p.R && this->G == p.G && this->B == p.B;
	}

	friend ostream& operator<<(ostream& os, const Pixel& pixel) {
		// NOTE: This is just to make the Matrix print prettier
		os << pixel.R + pixel.G + pixel.B;
		// std::ostringstream oss;
		// oss << "(" << pixel.R << ", " << pixel.G << ", " << pixel.B << ")";
		// os << oss.str();
		return os;
	}
};

// Function to load PGM file
vector<vector<int>> loadPGM(const string& filename) {

    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string magicNumber;
    file >> magicNumber;

    if (magicNumber != "P2" && magicNumber != "P5") {
        throw runtime_error("Unsupported PGM format: " + magicNumber);
    }

    // Read image dimensions and max value
    int width, height, maxVal;
    file >> width >> height >> maxVal;

    // Skip single whitespace
    file.ignore();

    // Prepare a matrix to hold the pixel data
	vector<vector<int>> image(height, vector<int>(width));

	if (magicNumber == "P2") {

		// ASCII format
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				file >> image[i][j];
			}
		}
	}

	else if (magicNumber == "P5") {

		// Binary format
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				image[i][j] = file.get();
			}
		}
	}

	file.close();

	return image;
}

void convertPngToPpm(const string& inputFile, const string& outputFile) {

    // Construct the Python command
    string command = "python3 png_to_ppm.py \"" + inputFile + ".png\" \"" + outputFile + ".ppm\"";

    // Execute the command
    int result = system(command.c_str());

	// Check if the command executed successfully
	if (result != 0) {
		cerr << "Error occurred while attempting to convert the file." << endl;
	}
}

void applyGaussian(const string& inputFile, const string& outputFile, float sigma) {

    // Construct the Python command
    string command = "python3 gaussian.py \"" + inputFile + ".ppm\" \"" + outputFile + ".ppm\" " + to_string(sigma);

    // Execute the command
    int result = system(command.c_str());

	// Check if the command executed successfully
	if (result != 0) {
		cerr << "Error occurred while attempting to convert the file." << endl;
	}
}

// Function to load PPM file
vector<vector<Pixel>> loadPPM(const string& filename, float sigma, bool debug = false) {

	convertPngToPpm(filename, filename);

	applyGaussian(filename, filename, sigma);

    ifstream file(filename + ".ppm", ios::binary);

    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string magicNumber;
    file >> magicNumber;

    if (magicNumber != "P3" && magicNumber != "P6") {
        throw runtime_error("Unsupported PPM format: " + magicNumber);
    }

    // Read image dimensions and max value
    int width, height, maxVal;
    file >> width >> height >> maxVal;

    if (maxVal > 255) {
        throw runtime_error("Only 8-bit PPM files are supported.");
    }

    // Skip single whitespace
    file.ignore();

    // Prepare a matrix to hold the pixel data
	vector<vector<Pixel>> image(height, vector<Pixel>(width));

	if (magicNumber == "P3") {

		// ASCII format
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				int r, g, b;
				file >> r >> g >> b;

				image[i][j] = {(Byte)r, (Byte)g, (Byte)b};
			}
		}
	}

	else if (magicNumber == "P6") {

		// Binary format
		for (int i = 0; i < height; i++) {

			for (int j = 0; j < width; j++) {

				Pixel pixel {
					.R = (Byte)file.get(),
					.G = (Byte)file.get(),
					.B = (Byte)file.get(),
				};

				image[i][j] = pixel;
			}
		}
	}

	file.close();

	if (debug) {
		cout << "image loaded successfully: " << image.size() << "x" << image[0].size() << "\n";
	}
	
	return image;
}

template<typename T>
ostream& operator<<(ostream& os, const vector<vector<T>>& matrix) {
	for (const auto& row : matrix) {
		for (const auto& elem : row) {
			os << elem << " ";
		}
		os << endl;
	}
	return os;
}


int main() {
    // Arquivos no mesmo diretório
    string inputFile = "igreja";  
    string outputFile = "input"; 

    try {
        convertPngToPpm(inputFile, outputFile);
    } catch (const exception &e) {
        cerr << "Erro: " << e.what() << endl;
        return 1;
    }
	

    return 0;
}