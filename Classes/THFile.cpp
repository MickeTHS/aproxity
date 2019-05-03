#include "THFile.h"

THFile::THFile(const std::string& file) {
    _filename = file;
}

THFile::~THFile() {
    
}

bool THFile::fileExists() {
    return cocos2d::FileUtils::getInstance()->isFileExist(_filename);
}

std::string THFile::readFile() {
    return (_data = cocos2d::FileUtils::getInstance()->getStringFromFile(_filename));
}

void THFile::setString(const std::string& str) {
    _data = str;
}

void THFile::save() {
    cocos2d::FileUtils::getInstance()->writeStringToFile(_data, _filename);
}