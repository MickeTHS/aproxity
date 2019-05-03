
#include "THUILabeledCheckbox.h"


THUILabeledCheckbox* THUILabeledCheckbox::create(const std::string& text) {
    auto ret = new (std::nothrow) THUILabeledCheckbox();
    if(ret && ret->init(text)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void THUILabeledCheckbox::setSelected(bool selected) {
    _checkbox->setSelected(selected);
}

bool THUILabeledCheckbox::init(const std::string& text) {
    if ( !Node::init() ) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);

    
    _label = Label::createWithTTF(text, "fonts/AGENCY.TTF", 16);
    _label->setPosition(Vec2(-15,0));
    _label->setAnchorPoint(Vec2(1, 0.5));
    
    
    _checkbox = CheckBox::create("check_box_normal.png",
                                 "check_box_normal_press.png",
                                 "check_box_active.png",
                                 "check_box_normal_disable.png",
                                 "check_box_active_disable.png");
    
    _checkbox->setPosition(Vec2(0,0));
    
    addChild(_checkbox);
    addChild(_label);


    return true;
}

void THUILabeledCheckbox::setTouchCallback(const cocos2d::ui::AbstractCheckButton::ccWidgetTouchCallback& callback) {
    _checkbox->addTouchEventListener(callback);
}