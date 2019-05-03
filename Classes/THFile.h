#pragma once

#include <string>

#include "cocos2d.h"


class THFile {
public:
    THFile(const std::string& file);
    
    ~THFile();
    
    bool fileExists();
    
    std::string readFile();
    
    void setString(const std::string& str);
    
    void save();
    
    std::string _data;

private:
    std::string _filename;
};