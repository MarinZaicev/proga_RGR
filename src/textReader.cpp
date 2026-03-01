#include "textReader.h"
#include <fstream>
#include <stdexcept>
using namespace std;
namespace fs = std::filesystem;

fs::path prepareFilePath(const string& filename, bool checkExists) {
    fs::path filePath = filename;
    
    if (checkExists) {
        if (!fs::exists(filePath)) {
            throw runtime_error("Файл не существует: " + filePath.string());
        }
        if (!fs::is_regular_file(filePath)) {
            throw runtime_error("Не является файлом: " + filePath.string());
        }
    }
    
    return filePath;
}

string readFileBinary(const string& filename) {
    fs::path filePath = prepareFilePath(filename, true);
    
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file.is_open()) {
        throw runtime_error("Ошибка открытия файла: " + filePath.string());
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    string buffer(size, '\0');
    if (!file.read(&buffer[0], size)) {
        throw runtime_error("Ошибка чтения файла: " + filePath.string());
    }

    return buffer;
}

string readFile(const string& filename) {
    fs::path filePath = prepareFilePath(filename, true);
    
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Ошибка открытия файла: " + filePath.string());
    }

    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    string buffer(size, '\0');
    if (!file.read(&buffer[0], size)) {
        throw runtime_error("Ошибка чтения файла: " + filePath.string());
    }

    return buffer;
}

void writeFileBinary(const string& filename, const string& data) {
    fs::path filePath = prepareFilePath(filename, false);
    
    ofstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Ошибка создания файла: " + filePath.string());
    }
    
    file.write(data.data(), data.size());
}