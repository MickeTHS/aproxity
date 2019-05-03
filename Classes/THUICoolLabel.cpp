
#include "THUICoolLabel.h"
#include "THUITouchableSprite.h"


THUICoolLabel* THUICoolLabel::create(const std::string& top_text, const std::string& bottom_text, const std::string& bottom_image) {
    auto ret = new (std::nothrow) THUICoolLabel();
    if(ret && ret->init(top_text, bottom_text, bottom_image)) {
        ret->autorelease();

        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THUICoolLabel::init(const std::string& top_text, const std::string& bottom_text, const std::string& bottom_image) {
    if ( !Node::init() ) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    
    _label_top = Label::createWithTTF(top_text, "fonts/FRAMD.TTF", 12);
    _label_top->setPosition(Vec2(0, 0));
    _label_top->setAnchorPoint(Vec2(0.5, 1));

    int padding = 8;

    _background_bottom = THUITouchableSprite::create(bottom_image);
    _background_bottom->setPosition(Vec2(0, -(_label_top->getContentSize().height + padding)));
    _background_bottom->setAnchorPoint(Vec2(0.5, 1));
    
    _label_bottom = Label::createWithTTF(bottom_text, "fonts/AGENCY.TTF", 16);
    _label_bottom->setPosition(Vec2(0, -(_label_top->getContentSize().height + padding + _background_bottom->getContentSize().height/2)));
    _label_bottom->setAnchorPoint(Vec2(0.5, 0.5));


    addChild(_label_top, 1);
    addChild(_background_bottom, 2);
    addChild(_label_bottom, 3);
    
    
    return true;
}

void THUICoolLabel::setClickCallback(const callbackFunc& func) {
    _background_bottom->onClicked = func;
}
