#pragma once

#include "cocos2d.h"
#include "ansa.h"
#include "Common.h"
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <time.h>
#include "THGlobal.h"
#include <math.h>


class THSettingsFile;

using namespace cocos2d;



enum DiffLevel {
    DIFF_NONE = 0,
    DIFF_EASY = 1,
    DIFF_MEDIUM = 2,
    DIFF_HARD = 3,
    DIFF_MIXED = 4
};

struct GameLevel : Node {
    GameLevel() {
        index       = 0;
        num_blue    = 0;
        num_yellow  = 0;
        picked_answer = BallColor::NONE;
        correct_color = BallColor::NONE;
        time        = 0.0f;
        ended       = false;
    }
    
    virtual ~GameLevel() {
    
    }
    
    CREATE_FUNC(GameLevel);
    
    int index;
    
    int num_yellow;
    int num_blue;
    float ratio;
    
    BallColor picked_answer;
    BallColor correct_color;
    float time; // time in seconds
    std::uint64_t time_ms; //time in milliseconds
    bool ended;
    
    void startClock() {
        _begin = milliseconds_now();
    }
    
    void generateWithRatio(float l_ratio, int seed) {
        ratio = l_ratio;
        
        //caluclate how many balls of one color we need bare mininum to have a correct ratio
        int min_balls_of_one_color = ceilf(1.0f / (ratio - 1.0f));
        
        CCLOG("generateWithRatio: ratio: %f, min: %d", ratio, min_balls_of_one_color);
        srand((unsigned int)milliseconds_now() + seed);
        
        //now randomize the the amount of balls for one color
        int r = rand() / 2;
        
        int c1 = min_balls_of_one_color;
        int c2 = (float)min_balls_of_one_color * ratio;
        
        int total = (c1+c2) + (r % (MAX_BALLS-(c1+c2)));
        
        int mul = total / (c1+c2);
        
        int f1 = mul * c1;
        int f2 = mul * c2;
        
        CCLOG("%d, %d", f1, f2);
        
        CCASSERT(f1+f2 >= 3, "mininum total balls not satisfied");
        CCASSERT(f1 != f2, "blue and yellow cant be equal");
        
        if ((rand()/2) % 2 == 0) {
            num_yellow = f1;
            num_blue = f2;
            correct_color = BallColor::BLUE;
        }
        else {
            num_blue = f1;
            num_yellow = f2;
            correct_color = BallColor::YELLOW;
        }
        
        //CCLOG("total: %d, c1: %d, c2: %d, f1: %d, f2: %d, mul: %d", total, c1, c2, f1, f2, mul);
    }
    
    void endClock() {
        time = elapsed();
        time_ms = elapsedMS();
    }
    
    std::uint64_t elapsedMS() {
        return milliseconds_now() - _begin;
    }
    
    float elapsed() {
        std::uint64_t end = milliseconds_now();
        
        return (float)(end - _begin) / 1000.0000f;
    }
    
    bool correct() {
        CCASSERT(correct_color != BallColor::NONE && picked_answer != BallColor::NONE, "illegal call when color hasnt been assigned");
        
        return picked_answer == correct_color;
    }
    
    private:
    std::uint64_t _begin;
};

class LevelRatio {
public:
    LevelRatio(float ratio, int num) : _ratio(ratio), _num(num) {}
    
    float   _ratio;
    int     _num;
};

class GameState : Node {
public:
    static GameState* getInstance();
    
    GameState();
    virtual ~GameState();
    
    CREATE_FUNC(GameState);
    
    
    callbackFunc onGameInterrupted;
    callbackFunc onGameResumed;
    
    void setPickedColor(BallColor color);
    void setDifficulty(DiffLevel diff);
    
    
    
    bool compareSetting(const std::string& setting, const std::string& comp);
    void setSetting(const std::string& setting, const std::string& val);
    
    int numBlue();
    int numYellow();
    
    void moveToNextLevel();
    
    void startLevel();
    void endLevel();
    
    int getCurrentLevel();
    int getNumLevels();
    
    bool levelCorrect();
    bool levelEnded();
    bool isLastLevel();
    
    void endGame();
    
    void startClock();
    void endClock();
    float getElapsed();
    
    int getTotalScore();
    int getRanking();
    float getAvgRespTime();
    int getAccuracy();
    
    void generateRatioTable();
    void generateLevels();
    void exitGame();
    //
    
    std::vector<GameLevel*> _levels;
    
    int _current_level;
    int _num_levels;
    
    std::string _selected_gender;
    std::string _selected_date_of_birth;
    DiffLevel _difficulty;
        
    THSettingsFile* _settings_file;
    std::vector<LevelRatio> _ratio_table;
};
