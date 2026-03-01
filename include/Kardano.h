#pragma once
#include <string>
#include <vector>
using namespace std;

#ifdef _WIN32
    #ifdef KARDANO_LIB_EXPORTS
        #define KARDANO_API declspec(dllexport)
    #else
        #define KARDANO_API __declspec(dllimport)
    #endif
#else
    #define KARDANO_API __attribute((visibility("default")))
#endif

KARDANO_API int isValidKardanoSize();
KARDANO_API vector<vector<bool>> createCardanoGrid(int size);
vector<vector<bool>> cardanoGridReverse(int size, vector<vector<bool>>& grid);
string matrixInString(vector<vector<char>> messageGrid);
vector<vector<char>> stringInMatrix(int gridSize, string& chiphertext);
string criptText(const string& message, vector<vector<bool>> reshetka);
string decrypt(const string& ciphertext, vector<vector<bool>> grid);
KARDANO_API string cryptKardano(const string& inputfile, vector<vector<bool>> reshetka);
KARDANO_API string decryptKardano(const string& inputfile, vector<vector<bool>> reshetka);