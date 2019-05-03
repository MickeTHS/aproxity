#include "THUITouchableSprite.h"

using namespace cocos2d;

THUITouchableSprite::THUITouchableSprite(const std::string& name) : _name(name) {}

THUITouchableSprite::~THUITouchableSprite() {}

THUITouchableSprite* THUITouchableSprite::create(const std::string& file) {
    THUITouchableSprite* pSprite = new THUITouchableSprite(file);
    
    if (pSprite->initWithFile(file)) {
        pSprite->autorelease();
        pSprite->setInputEnabled(false);
        pSprite->initOptions();
        
        pSprite->addEvents();
        pSprite->onClicked = NULL;
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void THUITouchableSprite::setInputEnabled(bool enabled) {
    _enabled_input = enabled;
}

void THUITouchableSprite::initOptions() {
    _enabled_input = true;
}

void THUITouchableSprite::addEvents() {
    _touch_listener = EventListenerTouchOneByOne::create();
    
    _touch_listener->onTouchEnded = CC_CALLBACK_2(THUITouchableSprite::onTouchEnded, this);
    _touch_listener->onTouchBegan = CC_CALLBACK_2(THUITouchableSprite::onTouchBegan, this);
    _touch_listener->onTouchMoved = CC_CALLBACK_2(THUITouchableSprite::onTouchMoved, this);
    _touch_listener->onTouchCancelled = CC_CALLBACK_2(THUITouchableSprite::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touch_listener, this);
}

void THUITouchableSprite::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_enabled_input)
        return;
    
    cocos2d::Vec2 p = touch->getLocation();
    
    cocos2d::Rect rect(convertToWorldSpace(getPosition()), getContentSize());
    
    if (rect.containsPoint(p)) {
        if (onClicked != NULL) {
            onClicked();
        }
    }
}

bool THUITouchableSprite::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!_enabled_input)
        return false;
    
    cocos2d::Vec2 p = touch->getLocation();
    cocos2d::Rect rect(convertToWorldSpace(getPosition()), getContentSize());
    
    if (rect.containsPoint(p)) {
        return true;
    }
    
    return false;
}

void THUITouchableSprite::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void THUITouchableSprite::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {}