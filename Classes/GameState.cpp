//
//  GameState.cpp
//  ANSA
//
//  Created by macbookpro on 2015-09-24.
//
//

#include "GameState.h"
#include "THSettings.h"
#include <stdio.h>
#include <stdlib.h>

static GameState *_shared_game = NULL;

GameState* GameState::getInstance() {
    if (!_shared_game) {
        _shared_game = GameState::create();
        _shared_game->retain();
    }
    
    return _shared_game;
}

GameState::GameState() {
    _current_level      = -1;
    _num_levels         = -1;
    onGameResumed       = NULL;
    onGameInterrupted   = NULL;
    
    _settings_file = new THSettingsFile(cocos2d::FileUtils::getInstance()->getWritablePath() + "/settings.txt");
    
    // if the file doesnt exist, create it
    if (!_settings_file->settingsFileExist()) {
        
        _settings_file->setSetting("BOUNCE","1");
        _settings_file->setSetting("ASK_PRACTICE","1");
        _settings_file->setSetting("SHOW_LEVEL_COUNTER","1");
        _settings_file->setSetting("SHOW_LEVEL_BREAK","1");
        
        _settings_file->saveSettings();
        CCLOG("settings file saved");
    }
    
    _settings_file->parseSettings();
    
    generateRatioTable();
    //generateLevels();
}

GameState::~GameState() {
    delete _settings_file;
}

bool GameState::compareSetting(const std::string& setting, const std::string& comp) {
    
    std::string val = _settings_file->getSettingVal(setting);
    
    return val.compare(comp) == 0;
}

void GameState::setSetting(const std::string& setting, const std::string& val) {
    _settings_file->setSetting(setting, val);
}

void GameState::setPickedColor(BallColor color) {
    _levels[_current_level]->picked_answer = color;
}

void GameState::startClock() {
    _levels[_current_level]->startClock();
}

float GameState::getElapsed() {
    return _levels[_current_level]->elapsed();
}

void GameState::endClock() {
    _levels[_current_level]->endClock();
    
    CCLOG("GameState::end_clock %f", _levels[_current_level]->time);
}

void GameState::setDifficulty(DiffLevel diff) {
    _difficulty = diff;
}

void GameState::startLevel() {
    
}

void GameState::endLevel() {
    _levels[_current_level]->ended = true;
}

bool GameState::isLastLevel() {
    return _current_level+1 == _levels.size();
}

int GameState::getCurrentLevel() {
    return _current_level;
}

int GameState::getNumLevels() {
    return (int)_levels.size();
}

int myrandom (int i) { return std::rand()%i; }

void GameState::generateLevels() {
    
    for (auto l : _levels) {
        l->release();
    }
    
    _levels.clear();
    
    for (auto r : _ratio_table) {
        for (int j = 0; j < r._num; ++j) {
            auto level = GameLevel::create();
            level->retain();
            
            level->generateWithRatio(r._ratio, r._ratio * 10 + j);
            _levels.push_back(level);
        }
    }
    
    // shuffle our levels
    std::srand(unsigned(std::time(0)));
    std::random_shuffle(_levels.begin(), _levels.end(), myrandom);

    _current_level = 0;
}

void GameState::generateRatioTable() {
    _ratio_table.clear();
    
    _ratio_table.push_back(LevelRatio(1.1f,  1));
    _ratio_table.push_back(LevelRatio(1.25f, 1));
    _ratio_table.push_back(LevelRatio(1.35f, 1));
    _ratio_table.push_back(LevelRatio(1.5f,  1));
    /*_ratio_table.push_back(LevelRatio(1.65f, 4));
    _ratio_table.push_back(LevelRatio(1.75f, 4));
    _ratio_table.push_back(LevelRatio(1.9f,  4));
    _ratio_table.push_back(LevelRatio(2.0f,  4));
    _ratio_table.push_back(LevelRatio(2.25f, 4));
    _ratio_table.push_back(LevelRatio(2.5f,  4));*/
}

void GameState::endGame() {
    for (int i = 0; i < _levels.size(); ++i) {
        _levels[i]->release();
    }
    
    _levels.clear();
}

int GameState::getTotalScore() {
    return 1111;
}

int GameState::getRanking() {
    return 111;
}

float GameState::getAvgRespTime() {
    return 1.11f;
}

int GameState::getAccuracy() {
    return 99;
}

bool GameState::levelEnded() {
    return _levels[_current_level]->ended;
}

void GameState::moveToNextLevel() {
    _current_level++;
}

bool GameState::levelCorrect() {
    return _levels[_current_level]->correct();
}

int GameState::numBlue() {
    return _levels[_current_level]->num_blue;
}

int GameState::numYellow() {
    return _levels[_current_level]->num_yellow;
}

void GameState::exitGame() {
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
