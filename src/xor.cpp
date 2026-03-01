#include "xor.h"
XOR_API string xorCrypt(string& message, char xorKey)
{
    string encrypted;
    for (char n : message) {
        encrypted += n ^ xorKey;
    }
    return encrypted;
}

XOR_API string xorDecrypt(string& encrypted, char xorKey)
{
    return xorCrypt(encrypted, xorKey);
}
