#include "THSettings.h"

THSetting* THSetting::create(const std::string& str) {
    auto ret = new (std::nothrow) THSetting(str);
    if(ret) {
        ret->autorelease();
        ret->retain();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}


THSetting* THSetting::create(const std::string& id, const std::string& val) {
    auto ret = new (std::nothrow) THSetting(id, val);
    if(ret) {
        ret->autorelease();
        ret->retain();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

THSettingsFile::THSettingsFile(const std::string& filename) {
    _file = new THFile(filename);
    
    if (settingsFileExist())
        parseSettings();
}

THSettingsFile::~THSettingsFile() {
    for (auto s : _settings) {
        s->release();
    }
    
    delete _file;
}

bool THSettingsFile::settingsFileExist() {
    return _file->fileExists();
}

void THSettingsFile::parseSettings() {
    std::size_t p_pos = 0;
    std::size_t pos = 0;
    
    _data = _file->readFile();
    
    CCLOG("settings file: %s", _data.c_str());
    
    std::vector<THSetting*> tmp;
    
    while (pos != std::string::npos) {
        pos = _data.find("\n", p_pos);
        
        if (pos == std::string::npos) {
            break;
        }
        
        tmp.push_back(THSetting::create(_data.substr(p_pos, pos-p_pos)));
        
        p_pos = pos+1;
    }
    
    for (auto s : tmp) {
        if (settingExists(s->id, _settings))
            continue;
        _settings.push_back(s);
    }
}

bool THSettingsFile::settingExists(const std::string& id, const std::vector<THSetting*>& vec) {
    for (auto s : vec) {
        if (s->id.compare(id) == 0) {
            return true;
        }
    }
    
    return false;
}

std::string THSettingsFile::getSettingVal(const std::string& id) {
    for (auto s : _settings) {
        if (s->id.compare(id) == 0) {
            return s->val;
        }
    }
    
    return "";
}

void THSettingsFile::setSetting(const std::string& id, const std::string& val) {
    if (!settingExists(id, _settings)) {
        auto s = THSetting::create(id, val);
        
        _settings.push_back(s);
        saveSettings();
        return;
    }
    
    for (auto s : _settings) {
        if (s->id.compare(id) == 0) {
            s->val = val;
            saveSettings();
            return;
        }
    }
}

std::string THSettingsFile::settingsToStr() {
    std::string str;
    
    for (auto s : _settings) {
        str += s->id + ":" + s->val + "\n";
    }
        
    return str;
}

void THSettingsFile::saveSettings() {
    _data = settingsToStr();
    
    CCLOG("begindata:%senddata", _data.c_str());
    _file->setString(_data);
    _file->save();
}
