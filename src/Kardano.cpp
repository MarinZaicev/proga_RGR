#include "Kardano.h"
#include "textReader.h"
#include <iostream>

using namespace std;

KARDANO_API int isValidKardanoSize(){
    while(true){
        cout << "Введите размер решетки Кардано(только четное число): ";
        
        int size;
        cin >> size;

        if(size % 2 == 0){
            return size;
        }

        cout << "Неверный размер решетки! Попробуйте еще раз." << endl;
    }
}

KARDANO_API vector<vector<bool>> createCardanoGrid(int size) {
    if (size % 2 != 0) {
        cerr << "Размер решетки должен быть четным!";
        exit(1);
    }
    vector<vector<bool>> grid(size, vector<bool>(size, false));

    // Заполняем решётку по блокам 2x2
    for (int i = 0; i < size / 2; i += 1) {
        for (int j = 0; j < size / 2; j += 1) {
            int pozition = rand() % 4 + 1;
            if (pozition == 1) grid[i][j] = 1;
            if (pozition == 2) grid[j][size - i - 1] = 1;
            if (pozition == 3) grid[size - i - 1][size - j - 1] = 1;
            if (pozition == 4) grid[size - j - 1][i] = 1;
        }
    }
    return grid;
}

// Реализуем поворот решетки
vector<vector<bool>> cardanoGridReverse(int size, vector<vector<bool>>& grid) {
    vector<vector<bool>> rotated(size, vector<bool>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            rotated[j][size - 1 - i] = grid[i][j];
        }
    }

    return rotated;
}

string matrixInString(vector<vector<char>> messageGrid){
    string result;
    for (const auto& i : messageGrid) {
        for (char j : i) {
            result += j;
        }
    }
    return result;
}

vector<vector<char>> stringInMatrix(int gridSize, string& chiphertext) {
    vector<vector<char>> matrix(gridSize, vector<char>(gridSize));
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            matrix[i][j] = chiphertext[i * gridSize + j];
        }
    }
    return matrix;
}

// Функция шифрования
string cryptText(const string& message, vector<vector<bool>> grid) {
    int size = grid.size();
    vector<vector<char>> messageGrid(size, vector<char>(size, ' '));
    string msg = message;

    // Подсчитываем общее количество отверстий за 4 поворота
    int totalHoles = 0;
    vector<vector<bool>> tempGrid = grid;
    for (int rotation = 0; rotation < 4; rotation++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (tempGrid[i][j]) totalHoles++;
            }
        }
        tempGrid = cardanoGridReverse(size, tempGrid);
    }

    // Дополняем сообщение пробелами если нужно
    if (msg.length() < totalHoles) {
        msg.resize(totalHoles, ' ');
    }

    int position = 0;
    
    // Заполняем матрицу символами через 4 поворота
    for (int rotation = 0; rotation < 4; rotation++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] && position < msg.length()) {
                    messageGrid[i][j] = msg[position++];
                }
            }
        }
        grid = cardanoGridReverse(size, grid);
    }
    
    return matrixInString(messageGrid);
}

// Функция дешифровки
string decrypt(string& chiphertext, vector<vector<bool>> grid) {
    int size = grid.size();
    string message;
    
    // Преобразуем строку в матрицу
    vector<vector<char>> matrix = stringInMatrix(size, chiphertext);
    
    // Подсчитываем сколько символов должно быть в результате
    int totalHoles = 0;
    vector<vector<bool>> tempGrid = grid;
    for (int rotation = 0; rotation < 4; rotation++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (tempGrid[i][j]) totalHoles++;
            }
        }
        tempGrid = cardanoGridReverse(size, tempGrid);
    }
    
    // Собираем символы в правильном порядке
    int charsRead = 0;
    for (int rotation = 0; rotation < 4; rotation++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] && charsRead < totalHoles) {
                    message += matrix[i][j];
                    charsRead++;
                }
            }
        }
        grid = cardanoGridReverse(size, grid);
    }
    
    return message;
}

KARDANO_API string cryptKardano(const string& inputfile, vector<vector<bool>> grid){
    int size = grid.size();
    string result;
    
    // Подсчитываем общее количество отверстий для одного блока
    int totalHoles = 0;
    vector<vector<bool>> tempGrid = grid;
    for (int rotation = 0; rotation < 4; rotation++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (tempGrid[i][j]) totalHoles++;
            }
        }
        tempGrid = cardanoGridReverse(size, tempGrid);
    }
    
    // Обрабатываем сообщение блоками
    for (size_t i = 0; i < inputfile.size(); i += totalHoles) {
        string block = inputfile.substr(i, totalHoles);
        // Если блок меньше нужного размера, он будет дополнен в cryptText
        result += cryptText(block, grid);
    }
    
    return result;
}

KARDANO_API string decryptKardano(const string& inputfile, vector<vector<bool>> grid){ 
    int size = grid.size();
    int blockSize = size * size;
    string result;
    
    // Обрабатываем зашифрованный текст блоками
    for (size_t i = 0; i < inputfile.size(); i += blockSize) {
        string block = inputfile.substr(i, blockSize);
        result += decrypt(block, grid);
    }
    
    return result;
}