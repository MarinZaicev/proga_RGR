#include "magicSquare.h"

MAGIC_SQUARE_API int isValidMagicSquareSize(){
    while(true){
        cout << "Введите размер магического квадрата(только нечетное число): ";
        
        int size;
        cin >> size;

        if(size % 2 != 0){
            return size;
        }

        cout << "Неверный размер решетки! Попробуйте еще раз." << endl;
    }
}
// Генерация квадрата с нечетным размером по методу Сиамского.
vector<vector<int>> createSquare(int size) {
    vector<vector<int>> square(size, vector<int>(size, 0));
    int i = 0, j = size / 2;

    for (int n = 0; n < size * size; n++) {
        square[i][j] = n;

        int nextI = (i - 1 + size) % size;
        int nextJ = (j + 1) % size;

        if (square[nextI][nextJ] != 0) {
            i = (i + 1) % size;
        }
        else {
            i = nextI;
            j = nextJ;
        }
    }
    return square;
}

string matrixInString(vector<vector<int>> square) {
    string matrix;
    for (int i = 0; i < square.size(); i++) {
        for (int j = 0; j < square.size(); j++) {
            matrix += to_string(square[i][j]);
        }
    }
    return matrix;
}

vector<vector<string>> stringInMatrix(string& message, int matSize) {
    vector<vector<string>> matrix(matSize, vector<string>(matSize, " "));
    for (int i = 0; i < matSize; i++) {
        for (int j = 0; j < matSize; j++) {
            if (i * matSize + j < message.size()) {
                matrix[i][j] = string(1, message[i * matSize + j]);
            }
        }
    }
    return matrix;


}

string crypt(string& message, int size) {
    auto square = createSquare(size);

    vector<int> numbers; // Список индексов.

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            numbers.emplace_back(square[i][j]);
        }
    }

    // Шифруем
    string result(size * size, ' ');
    for (int i = 0; i < size * size; i++) {
        result[i] = message[numbers[i]];
    }
    return result;
}


string decrypt(string& encryptMsg, int size) {
    int i = 0;
    vector<int> numbers; // Список индексов.
    auto square = createSquare(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            numbers.emplace_back(square[i][j]);
        }
    }

    vector<int> decryptNumbers(size * size, 0); // Создаем последовательный список чисел.
    string decrypt;
    for (int& n : decryptNumbers) { // Заполняем.
        n = i;
        i++;
    }
    for (int i = 0; i < size * size; i++) { // Процесс дешифровки.
        for (int j = 0; j < size * size; j++) {
            if (decryptNumbers[i] == numbers[j]) {
                decrypt += encryptMsg[j];
            }
        }
    }

    return decrypt;
}

MAGIC_SQUARE_API string cryptMagicSquare(string& inputfile, int size) {
    string result;
    string forCrypt;
    long i = 0;
    int squareSize = size * size;
     
    while (i < inputfile.size()) {
        // Берем подстроку длиной size*size + 1 или до конца строки
        size_t remaining = inputfile.size() - i;
        size_t chunkSize;
        if (remaining >= squareSize) {
            chunkSize = squareSize;
        }
        else {
            chunkSize = remaining;
        }
        forCrypt = inputfile.substr(i, chunkSize);

        // Дополняем пробелами
        if (forCrypt.size() < squareSize - 1) {
            forCrypt.resize(squareSize, ' ');
        }

        result += crypt(forCrypt, size);
        i += squareSize;
    }
    return result;
}

MAGIC_SQUARE_API string decryptMagicSquare(string& inputfile, int size) {
    string result;
    string forDecrypt;
    long i = 0;
    int squareSize = size * size;

    while (i < inputfile.size()) {
        // Берем подстроку длиной size*size или до конца строки
        size_t remaining = inputfile.size() - i;
        size_t chunkSize;
        if (remaining >= squareSize) {
            chunkSize = squareSize;
        }
        else {
            chunkSize = remaining;
        }
        forDecrypt = inputfile.substr(i, chunkSize);
        // Дополняем пробелами
        if (forDecrypt.size() < squareSize - 1) {
            forDecrypt.resize(squareSize, ' ');
        }

        result += decrypt(forDecrypt, size);
        i += squareSize;
    }
    return result;
}