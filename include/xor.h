#pragma once
#include <string>
using namespace std;

#ifdef _WIN32
    #ifdef XORLIB_EXPORTS
        #define XOR_API declspec(dllexport)
    #else
        #define XOR_API __declspec(dllimport)
    #endif
#else
    #define XOR_API __attribute((visibility("default")))
#endif

XOR_API string xorCrypt(string& message, char xorKey);  // Пример экспортируемой функцииint add(int a, int b)
XOR_API string xorDecrypt(string& encrypted, char xorKey);