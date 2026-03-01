#ifndef CHECKPASSWORD_H
#define CHECKPASSWORD_H

#include <string>
#include <filesystem>

// Все функции должны принимать const string& для единообразия
bool isPasswordCreated(const std::string& fileName);
void createPassword(const std::string& fileName);
bool isPasswordTrue(const std::string& fileName);

#endif // CHECKPASSWORD_H
