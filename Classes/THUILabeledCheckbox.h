#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace ui;

class THUILabeledCheckbox : public Node {
public:
    static THUILabeledCheckbox* create(const std::string& text);
    
    virtual bool init(const std::string& text);
    
    void setTouchCallback(const cocos2d::ui::AbstractCheckButton::ccWidgetTouchCallback& callback);
    void setSelected(bool selected);
    
    CheckBox*   _checkbox;
    
private:
    Label*      _label;

};
