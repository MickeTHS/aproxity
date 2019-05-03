#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ansa.h"
//#include "StatisticsReporter.h"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace ui;

class BallGenerator;
class GameState;

class BallsScene : public cocos2d::LayerColor {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    bool generate();
    
    void displayAnswer();
    void newLevel();
    
    void hideBallsAction(float dt);
    void inputBeginAction(float dt);
    void updateAction(float dt);
    void playBounceEffect(float dt);
    
    void setYellow(int yellow);
    void setBlue(int blue);
    
    void giveAnswer(BallColor color);
    
    void yellowPressed(Ref* btn);
    void bluePressed(Ref* btn);

    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    
    // implement the "static create()" method manually
    CREATE_FUNC(BallsScene);
    
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    virtual void onGameInterrupted();
    virtual void onGameResumed();
    
    //void hiscore_call_back(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void hiscore_callback();

private:
    
    int _tmp_counter;
    std::uint64_t _tmp_time;
    std::uint64_t _tmp_start_time;
    //BallGenerator* _balls;
    
    int         _yellow;
    int         _blue;
    
    Button*     _yellowbutton;
    Button*     _bluebutton;
    
    Label*      _res_label;
    Label*      _ms_label;
    Label*      _continue_label;
    Label*      _progress_label;
    
    int			_ad_counter;

    const int	_ADD_INTERVAL = 20;

//    StatisticsReporter statReporter;

    void show_ad_if_time();
    
    //StatisticsReporter _sr;

    EventListenerTouchOneByOne* _touch_listener;
};
