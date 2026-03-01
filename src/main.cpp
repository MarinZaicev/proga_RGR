#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "xor.h"
#include "Kardano.h"
#include "magicSquare.h"
#include "textReader.h"
#include "checkPassword.h"

enum class Command{
    ENCRYPT,
    DECRYPT,
    GENERATE_KEY,
    IN_CONSOLE,

    XOR,
    KARDANO,
    MAGIC_SQUARE,

    EXIT
};

Command choiseCommand(const string& input) {
    static const map<string, Command> commandMap = {
        {"ENCRYPT", Command::ENCRYPT},
        {"DECRYPT", Command::DECRYPT},
        {"GENERATE_KEY", Command::GENERATE_KEY},
        {"IN_CONSOLE", Command::IN_CONSOLE},

        {"XOR", Command::XOR},
        {"KARDANO", Command::KARDANO},
        {"MAGIC_SQUARE", Command::MAGIC_SQUARE},

        {"EXIT", Command::EXIT},
    };

    auto it = commandMap.find(input);
    if (it != commandMap.end()) {
        return it->second;
    }
    return Command::EXIT;
}

vector<vector<bool>> kardanoKey;

void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

string fileNameWithoutType(string& message){
    string fileType;
    int pointPos = message.find(".");
    return message.substr(0, pointPos);
    
}

string fileNameWithoutEncrypted(string& message){
    int encryptedPos = message.find("_");
    return message.substr(encryptedPos + 1, message.size());
}

void createFile(){
    string fileName;
    cout << "Для начала шифрования необходимо выбрать или создать файл для шифрования." << endl;
    cout << "ОБРАТИТЕ ВНИМАНИЕ: в имени файла не должно быть точек, но после названия файла необходимо поставить точку и написать его тип" << endl;
    cout << "Пример: fileName.txt" << endl;
    cout << "Введите имя файла: ";
    cin >> fileName;

     bool fileExists = filesystem::exists(fileName);
    if (fileExists) {
        cout << "Файл " << fileName << " открыт" << endl;
    } 
    else {
        ofstream newFile(fileName);
        if (newFile.is_open()) {
            cout << "Файл " << fileName << " создан" << endl;
        } 
        else {
            cerr << "Ошибка создания файла!" << endl;
            return;
        }
    }
}

string consoleEncryptFile(const string& fileName){
    fs::path filePath(fileName);
    
    // Получаем:
    // - parentPath: директория исходного файла
    // - stem: имя файла без расширения
    fs::path parentDir = filePath.parent_path();
    string baseName = filePath.stem().string();
    
    // Формируем новое имя с сохранением пути
    fs::path encryptedPath;
    if (parentDir.empty()) {
        // Файл в текущей директории
        encryptedPath = "Encrypted_" + baseName + ".bin";
    } else {
        // Файл в другой директории - сохраняем путь
        encryptedPath = parentDir / ("Encrypted_" + baseName + ".bin");
    }
    
    cout << "Файл " << fileName << " будет зашифрован в " << encryptedPath.string() << endl;
    
    return encryptedPath.string();
}

void encryptedXor(){
    clearConsole();
    string fileName;
    cout << "Введите имя шифруемого файла: ";
    cin >> fileName;

    string fileContent = readFileBinary(fileName);

    cout << "Введите ключ: ";
    char xorKey;
    cin >> xorKey;

    clearConsole();

    string encryptedFileName = consoleEncryptFile(fileName);
    createPassword(encryptedFileName);
    string encrypted = xorCrypt(fileContent, xorKey);

    try{
        writeFileBinary(encryptedFileName, encrypted);
    }
    catch(exception& e){
        cerr << "Ошибка " << e.what() << endl;
    }
}

void decryptedXor(){
    clearConsole();

    string decryptedFileName;
    string encryptedFileName;
    char xorKey;

    cout << "Введите имя зашифрованного файла: ";
    cin >> encryptedFileName;
    
    // Проверяем пароль (функция сама извлечет имя)
    if(!isPasswordTrue(encryptedFileName)){
        return;
    }
    
    cout << "Введите имя итогового файла (его тип должен совпадать с исходным!): ";
    cin >> decryptedFileName;
    cout << "Введите ключ: ";
    cin >> xorKey;

    clearConsole();

    try {
        // Читаем зашифрованный файл по ПОЛНОМУ пути
        string fileContent = readFileBinary(encryptedFileName);
        string decrypted = xorDecrypt(fileContent, xorKey);

        // Записываем расшифрованный файл
        writeFileBinary(decryptedFileName, decrypted);
        cout << "Файл " << encryptedFileName << " расшифрован в " << decryptedFileName << endl << endl;
    }
    catch(exception& e){
        cerr << "Ошибка: " << e.what() << endl;
        cerr << "Проверьте существование файла: " << encryptedFileName << endl;
    }
}

template <typename T> 
void printMatrix(vector<vector<T>> matrix){
    for (auto n : matrix){
        for(auto c : n){
            cout << c << " ";
        }
        cout << endl;
    }
}

void encryptedKardano(){
    clearConsole();

    string fileName;
    cout << "Введите имя шифруемого файла: ";
    cin >> fileName;

    string fileContent = readFileBinary(fileName);

    int kardanoKeySize  = isValidKardanoSize();

    kardanoKey = createCardanoGrid(kardanoKeySize);
    cout << "Ключ: " << endl;
    printMatrix(kardanoKey);

    clearConsole();
    //Шифруем.
    string encryptedFileName = consoleEncryptFile(fileName);  
    createPassword(encryptedFileName);
    string encrypted = cryptKardano(fileContent, kardanoKey);

    try{
        writeFileBinary(encryptedFileName, encrypted);
    }
    catch(exception& e){
        cerr << "Ошибка " << e.what() << endl;
    }
}

void decryptedKardano(){
    clearConsole();

    string decryptedFileName;
    string encryptedFileName;

    cout << "Введите имя зашифрованного файла: ";
    cin >> encryptedFileName;
    
    if(!isPasswordTrue(encryptedFileName)){
        return;
    }

    cout << "Введите имя итогового файла (его тип должен совпадать с исходным!): ";
    cin >> decryptedFileName;

    clearConsole();
    
    try {
        string fileContent = readFileBinary(encryptedFileName);
        string decrypted = decryptKardano(fileContent, kardanoKey);

        writeFileBinary(decryptedFileName, decrypted);
        cout << "Файл " << encryptedFileName << " расшифрован в " << decryptedFileName << endl << endl;
    }
    catch(exception& e){
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void generateKardanoKey(){
    clearConsole();
    bool isNormalKey = false;
    while (true){
        int sizeGrid;
        cout << "Введите размер решетки(только четное число): ";
        cin >> sizeGrid;
        if(sizeGrid % 2 == 0){
            auto grid = createCardanoGrid(sizeGrid);
            clearConsole();

            cout << "Сгенерированная решетка Кардано: " << endl;
            printMatrix(grid);

            break;
            cout << endl;
        }
        else{
            cout << "недопустимый размер решетки! Попробуйте снова." << endl;
            clearConsole();
        }
    }
}

void encryptedMagicSquare(){

    clearConsole();

    string fileName;
    cout << "Введите имя шифруемого файла: ";
    cin >> fileName;

    string fileContent = readFileBinary(fileName);

    cout << "Введите размер генерируемого квадрата: ";
    int kardanoKeySize;
    cin >> kardanoKeySize;

    //Шифруем.
    clearConsole();

    string encryptedFileName = consoleEncryptFile(fileName);  
    createPassword(encryptedFileName);
    string encrypted = cryptMagicSquare(fileContent, kardanoKeySize);

    try{
        writeFileBinary(encryptedFileName, encrypted);
    }
    catch(exception& e){
        cerr << "Ошибка " << e.what() << endl;
    }
}

void decryptedMagicSquare(){
    clearConsole();

    string decryptedFileName;
    string encryptedFileName;

    cout << "Введите имя зашифрованного файла: ";
    cin >> encryptedFileName;

    if(!isPasswordTrue(encryptedFileName)){
        return;
    }

    cout << "Введите имя итогового файла (его тип должен совпадать с исходным!): ";
    cin >> decryptedFileName;

    int size = isValidMagicSquareSize();

    clearConsole();

    try {
        string fileContent = readFileBinary(encryptedFileName);
        string decrypted = decryptMagicSquare(fileContent, size);

        writeFileBinary(decryptedFileName, decrypted);
        cout << "Файл " << encryptedFileName << " расшифрован в " << decryptedFileName << endl << endl;
    }
    catch(exception& e){
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void generateMagicSquareKey(){
    clearConsole();
    bool isNormalKey = false;

    while (true){
        int sizeSquare;
        cout << "Введите размер квадрата(только нечетное число): ";
        cin >> sizeSquare;

        if (sizeSquare % 2 != 0){
            auto square = createSquare(sizeSquare);
            clearConsole();

            cout << "Сгенерированный магический квадрат: " << endl;
            printMatrix(square);

            break;
            cout << endl;
        }
        else{
            cout << "недопустимый размер квадрата! Попробуйте снова." << endl;
            clearConsole();
        }
    }
}

void encryptionMenu(){

    cout << "Выбeрите шифр для шифрования из списка и введите его название: " << endl;
    cout << "XOR" << endl;
    cout << "KARDANO" << endl;
    cout << "MAGIC_SQUARE" << endl;

    string input;
    cin >> input;

    Command choise = choiseCommand(input);
    switch(choise){
    case Command::XOR:
        encryptedXor();
        break;
    case Command::KARDANO:
        encryptedKardano();
        break;
    case Command::MAGIC_SQUARE:
        encryptedMagicSquare();
        break;
    default:
        cout << "Шифра '" << input << "' не существует" << endl;
        break;
    }
}

void decryptionMenu(){
    clearConsole();

    cout << "Выберите шифр для дешифрования из списка и введите его название: " << endl;
    cout << "XOR" << endl;
    cout << "KARDANO" << endl;
    cout << "MAGIC_SQUARE" << endl;

    string input;
    cin >> input;

    Command choise = choiseCommand(input);
    switch(choise){
    case Command::XOR:
        clearConsole();
        decryptedXor();
        break;
    case Command::KARDANO:
        clearConsole();
        decryptedKardano();
        break;
    case Command::MAGIC_SQUARE:
        clearConsole();
        decryptedMagicSquare();
        break;
    default:
        clearConsole();
        cout << "Шифра '" << input << "' не существует" << endl;
        break;
    }
}

void createKeyMenu(){
    string inputMethod;
    cout << "Введите метод шифрования, для которого нужно сгенерировать ключ." << endl;
    cout << "Доступные методы шифрования: " << endl;
    cout << "MAGIC_SQUARE - шифрование магическим квадратом. Ключ - магический квадрат." << endl;
    cout << "KARDANO - шифрование методом кардано. Ключ - решетка Кардано." << endl;
    cout << ">> ";

    cin >> inputMethod;

    Command choise = choiseCommand(inputMethod);
    switch(choise){
    case Command::KARDANO:
        generateMagicSquareKey();
        break;
    case Command::MAGIC_SQUARE:
        generateKardanoKey();
        break;
    default:
        cout << "Неверный метод шифрования." << endl;
        break;
    }
}

void cryptInConsole(){
    cout << "Выберите метод шифрования из представленного списка: " << endl;
    cout << "XOR - xor-шифрование." << endl;
    cout << "KARDANO - шифрование решеткой кардано." << endl;
    cout << "MAGIC_SQUARE - шифрование магическим квадратом." << endl;
    cout << "ОБРАТИТЕ ВНИМАНИЕ: введенное вами сообщение будет расшифровано сразу." << endl;
    cout << "В случае с шифрами Кардано и магическим квадратом будет сгенерирован ключ и выведен на экран." << endl;
    cout << ">> ";

    cin.ignore();
    string inputMethod;
    getline(cin, inputMethod);

    cout << "Введите сообщение: ";

    string inputMessage;
    getline(cin, inputMessage);

    Command choise = choiseCommand(inputMethod);
    switch(choise){
    case Command::XOR: {
        clearConsole();
        cout << "Введите ключ (символ): ";

        char xorKey;
        cin >> xorKey;

        string xorCrypted = xorCrypt(inputMessage, xorKey);
        string xorDecrypted = xorDecrypt(xorCrypted, xorKey);

        cout << "Исходное сообщение: " << inputMessage << endl;
        cout << "Зашифрованное сообщение: " << xorCrypted << endl;
        cout << "Расшифрованное сообщение: " << xorDecrypted << endl << endl;
        break;
    }
    case Command::KARDANO:{
        clearConsole();
        int gridSize = isValidKardanoSize();

        auto grid = createCardanoGrid(gridSize);

        string kardanoCrypted = cryptKardano(inputMessage, grid);
        string kardanoDecrypted = decryptKardano(kardanoCrypted, grid);

        cout << "Исходное сообщение: " << inputMessage << endl;
        cout << "Зашифрованное сообщение: " << kardanoCrypted << endl;
        cout << "Расшифрованное сообщение: " << kardanoDecrypted << endl << endl;
        break;
    }
    case Command::MAGIC_SQUARE: {
        clearConsole();
        cout << "Введите размер генерируемого магического квадрата (только нечетные числа): ";

        int squareSize = isValidMagicSquareSize();

        string magicSquareCrypted = cryptMagicSquare(inputMessage, squareSize);
        string magicSquareDecrypted = decryptMagicSquare(magicSquareCrypted, squareSize);

        cout << "Исходное сообщение: " << inputMessage << endl;
        cout << "Зашифрованное сообщение: " << magicSquareCrypted << endl;
        cout << "Расшифрованное сообщение: " << magicSquareDecrypted << endl << endl;
    }
    default:
        cout << "Такого метода шифрования нет, попробуйте еще раз." << endl;
        cryptInConsole();
        break;
    }
}

void chooseAction(string& input){
    Command choise = choiseCommand(input);
    switch(choise){
    case Command::ENCRYPT:
        encryptionMenu();
        break;
    case Command::DECRYPT:
        decryptionMenu();
        break;
    case Command::GENERATE_KEY:
        createKeyMenu();
        break;
    case Command::IN_CONSOLE:
        cryptInConsole();
        break;
    default:
        cout << "Такого действия не существует" << endl;
        break;
    }
}
int main(){
    while(true){
        cout << "Выберите действие из списка: " << endl;
        cout << "ENCRYPT - зашифровать файл" << endl;
        cout << "DECRYPT - расшифровать файл" << endl;
        cout << "GENERATE_KEY - сгенерировать ключ" << endl;
        cout << "IN_CONSOLE - шифрование в консоли (сообщение зашифрутся и расшифруется сразу!)" << endl;
        cout << "EXIT - завершить работу программы" << endl;
        cout << ">> ";
        string input;
        cin >> input;
        if (input == "EXIT"){
            break;
        }
        else{
            clearConsole();
            chooseAction(input);
        }
    }
    return 0;
}