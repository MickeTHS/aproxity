#pragma once

#include <string>
#include <vector>
#include "THFile.h"

class THSetting : public cocos2d::Ref {
public:
    static THSetting* create(const std::string& id, const std::string& val);
    static THSetting* create(const std::string& str);
    
    std::string id;
    std::string val;
    
private:
    THSetting(const std::string& a_id, const std::string& a_val) : id(a_id), val(a_val) {}
    
    THSetting(const std::string& str) {
        std::size_t p = str.find(":", 0);
        
        id = str.substr(0,p);
        val = str.substr(p+1,str.length()-p-1);
    }
};


class THSettingsFile {
public:
    THSettingsFile(const std::string& filename);
    
    ~THSettingsFile();
    
    bool settingsFileExist();
    
    void parseSettings();
    
    void saveSettings();
    void setSetting(const std::string& id, const std::string& val);
    std::string getSettingVal(const std::string& id);

    
    
private:
    std::string _data;
    std::vector<THSetting*> _settings;

    bool settingExists(const std::string& id, const std::vector<THSetting*>& vec);
    std::string settingsToStr();
    
    THFile* _file;
};