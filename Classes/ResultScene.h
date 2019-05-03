#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "LineGraph.h"
//#include "StatisticsReporter.h"

using namespace cocos2d;

class StatisticsReporter;

struct lvl : public Ref {
    static lvl* create() {
        auto ret = new (std::nothrow) lvl();
        if(ret) {
            ret->autorelease();
            return ret;
        }
        
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    float ratio;
    int num_total;
    int num_correct;
    std::vector<float> resp_time;
    std::vector<std::uint64_t> resp_time_ms;
    uint64_t average_resp_time_ms;
    
//    bool operator() (float i, float j) { return (i<j);}
};

//bool sort_lvl (float i, float j) { return (i < j); }
//bool operator<(lvl const & a, lvl const & b) {
//    return a.ratio < b.ratio;
//}


class ResultScene : public cocos2d::LayerColor {
public:
    ResultScene();
    virtual ~ResultScene();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void onRankingClicked();
    void onAccuracyClicked();
    void onAvgRespClicked();
    
    CREATE_FUNC(ResultScene);

    void hiscore_callback();
    void report_game_callback(long score, long rank);
    
private:
    uint64_t global_avg_resp;
    int _num_correct;
    int _num_total;
    StatisticsReporter* _sr;
    LineGraph* _graph;
};
