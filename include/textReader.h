#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

fs::path prepareFilePath(const std::string& filename, bool checkExists = true);

string readFileBinary(const string& filename);
void writeFileBinary(const string& filename, const string& data);
string readFile(const string& filename);