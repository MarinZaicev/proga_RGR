#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
using namespace std;

#ifdef _WIN32
    #ifdef MAGICSQUARELIB_EXPORTS
        #define MAGIC_SQUARE_API declspec(dllexport)
    #else
        #define MAGIC_SQUARE_API __declspec(dllimport)
    #endif
#else
    #define MAGIC_SQUARE_API __attribute((visibility("default")))
#endif

// Генерация квадрата с нечетным размером по методу Сиамского.
MAGIC_SQUARE_API vector<vector<int>> createSquare(int size);

MAGIC_SQUARE_API int isValidMagicSquareSize();

vector<char> matrixInCharList(vector<vector<int>> square);

vector<vector<string>> stringInMatrix(string& message, int matrixSize);
vector<int> indexList(int size);
vector<int> sequentialNumbersList(int size);
string chankStringForCrypt(string& inputfile, int squareSize);
string addSpace(string& message, int squareSize);
MAGIC_SQUARE_API string cryptMagicSquare(string& message, int size);
MAGIC_SQUARE_API string decryptMagicSquare(string& encryptMsg, int size);