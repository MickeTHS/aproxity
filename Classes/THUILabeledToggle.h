#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include <vector>
#include "THGlobal.h"

class THUIToggleSwitch;

using namespace cocos2d;
using namespace ui;

enum ToggleState {
    TOGGLE_ON = 0,
    TOGGLE_OFF = 1
};

typedef std::function<void(ToggleState, THUIToggleSwitch*)> callbackToggleFunc;

class THUITouchableSprite;

class THUIToggleSwitch : public Node {
public:
    static THUIToggleSwitch* create();
    virtual bool init();
    void setToggleEnable(bool enable);
    bool isToggledOn();
    void setToggleState(ToggleState state);
    
    callbackToggleFunc onToggle;
private:
    void toggle(bool force);
    void toggleClick();
    
    std::vector<THUITouchableSprite*> _toggle_sprites_enabled;
    
    bool _enabled;
    ToggleState  _state;
    std::uint64_t _toggle_timer; // workaround for multiple touches incomming
};

class THUILabeledToggle : public Node {
public:
    static THUILabeledToggle* create(const std::string& text);
    
    virtual bool init(const std::string& text);
    
    void setOnToggleCallback(const callbackToggleFunc& func);
    
    void setToggledOn(bool toggle);
    bool isToggledOn();
    void setVisible(bool visible);
    void setSwitchTag(int tag);
private:
    Label*      _label;
    THUIToggleSwitch* _toggle;
};
