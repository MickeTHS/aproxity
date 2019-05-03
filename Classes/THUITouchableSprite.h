#pragma once

#include "cocos2d.h"
#include "THGlobal.h"

using namespace cocos2d;

class THUITouchableSprite : public cocos2d::Sprite {
public:
    THUITouchableSprite(const std::string& name);
    ~THUITouchableSprite();
    static THUITouchableSprite* create(const std::string& file);
    
    void setInputEnabled(bool enabled);
    
    void initOptions();
    
    void addEvents();

    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    callbackFunc onClicked;
    
    bool _enabled_input;
private:
    EventListenerTouchOneByOne* _touch_listener;
    
    
    std::string _name;
};
