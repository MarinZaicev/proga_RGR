#include "checkPassword.h"
#include "xor.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
namespace fs = std::filesystem;

void clearingConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string extractFilename(const string& filePath) {
    fs::path path(filePath);
    return path.filename().string();
}

bool isPasswordCreated(const string& fileName) {
    string justFilename = extractFilename(fileName);
    
    ifstream paswdFile("passwords.txt");
    if (!paswdFile.is_open()) return true;
    
    string line;
    while(getline(paswdFile, line)){
        int SpacePos = line.find(" ");
        if (SpacePos != string::npos) {
            string newLine = line.substr(0, SpacePos);
            if(justFilename == newLine){
                paswdFile.close();
                return false;
            }
        }
    }
    
    paswdFile.close();
    return true;
}

void createPassword(const string& fileName) {
    string justFilename = extractFilename(fileName);

    if(!isPasswordCreated(justFilename)){
        cout << "Для этого файла уже задан пароль!" << endl;
        return;
    }

    cout << "Для шифрования файла необходимо создать пароль."  << endl;
    cout << "Введите пароль: ";
    
    string password;
    cin >> password;
    string CryptPassword = xorCrypt(password, 'P');

    ofstream paswdFile("passwords.txt", ios::app);
    paswdFile << justFilename << " " << CryptPassword << endl;

    clearingConsole();
    paswdFile.close();
}

bool isPasswordTrue(const string& fileName) {
    string justFilename = extractFilename(fileName);
    
    cout << "Проверка доступа для файла: " << justFilename << endl;
    
    ifstream paswdFile("passwords.txt");
    if (!paswdFile.is_open()) {
        cout << "Файл passwords.txt не найден" << endl;
        return false;
    }
    
    string line;
    bool fileFound = false;

    while(getline(paswdFile, line)){
        int spacePos = line.find(" ");
        if (spacePos == string::npos) continue;

        string choosenName = line.substr(0, spacePos);
        string choosenPassword = line.substr(spacePos + 1);

        if (justFilename == choosenName){
            fileFound = true;
            string DecryptChoosenPassword = xorDecrypt(choosenPassword, 'P');
            
            for(int i = 1; i <= 3; i++){
                cout << "Введите пароль: ";
                string password;
                cin >> password;

                if (password == DecryptChoosenPassword){
                    cout << "Успешно!" << endl;
                    paswdFile.close();
                    return true;
                }
                else{
                    cout << "Неверный пароль, попробуйте еще раз" << endl;
                    if (i < 3) {
                        cout << "У вас осталось " << 3 - i << " попыток" << endl;
                    }
                }
            }
            break;
        }
    }
    
    paswdFile.close();
    
    if (!fileFound) {
        cout << "Файл '" << justFilename << "' не найден в базе паролей." << endl;
    }
    
    cout << "У вас нет доступа к файлу." << endl;
    return false;
}