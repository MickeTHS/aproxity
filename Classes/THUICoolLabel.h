#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "THGlobal.h"

class THUITouchableSprite;

using namespace cocos2d;
using namespace ui;

class THUICoolLabel : public Node {
public:
    static THUICoolLabel* create(const std::string& top_text, const std::string& bottom_text, const std::string& bottom_image);
    
    virtual bool init(const std::string& top_text, const std::string& bottom_text, const std::string& bottom_image);
    void setClickCallback(const callbackFunc& func);
    //void setTouchCallback(const cocos2d::ui::AbstractCheckButton::ccWidgetTouchCallback& callback);

    
    Label* _label_top;
    Label* _label_bottom;
    THUITouchableSprite* _background_bottom;
};