#include "THUILabeledToggle.h"
#include "THUITouchableSprite.h"
#include "Common.h"

THUIToggleSwitch* THUIToggleSwitch::create() {
    auto ret = new (std::nothrow) THUIToggleSwitch();
    if(ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THUIToggleSwitch::init() {
    if ( !Node::init() ) {
        return false;
    }
    
    _state = ToggleState::TOGGLE_ON;
    onToggle = NULL;
    
    _toggle_sprites_enabled.push_back(THUITouchableSprite::create("slide_on_enabled.png"));
    _toggle_sprites_enabled.push_back(THUITouchableSprite::create("slide_off_enabled.png"));
    _toggle_sprites_enabled.push_back(THUITouchableSprite::create("slide_on_disabled.png"));
    _toggle_sprites_enabled.push_back(THUITouchableSprite::create("slide_off_disabled.png"));
    
    
    for(auto t : _toggle_sprites_enabled) {
        addChild(t);
    }
    
    _enabled = false;
    _toggle_timer = 0;
    
    setToggleEnable(true);
    
    return true;
}

void THUIToggleSwitch::setToggleState(ToggleState state) {
    if (_state != state) {
        toggle(true);
    }
}

void THUIToggleSwitch::setToggleEnable(bool enable) {
    for (auto t : _toggle_sprites_enabled) {
        t->onClicked = CC_CALLBACK_0(THUIToggleSwitch::toggleClick, this);
        t->setVisible(false);
        t->setInputEnabled(false);
    }
    
    _toggle_sprites_enabled[_state]->setInputEnabled(true);
    _toggle_sprites_enabled[_state]->setVisible(true);
}

void THUIToggleSwitch::toggleClick() {
    toggle(false);
}

void THUIToggleSwitch::toggle(bool force) {
    if (!isVisible() && !force)
        return;
    
    // some kind of strange cached touch scenario, this solves that problem
    if (_toggle_timer + 100 > milliseconds_now())
        return;
    
    _state = _state == ToggleState::TOGGLE_ON ? ToggleState::TOGGLE_OFF : ToggleState::TOGGLE_ON;
    
    for (auto t: _toggle_sprites_enabled) {
        t->setVisible(false);
        t->setInputEnabled(false);
    }
    
    _toggle_sprites_enabled[_state]->setVisible(true);
    _toggle_sprites_enabled[_state]->setInputEnabled(true);
    
    _toggle_timer = milliseconds_now();

    
    // if user has defined a onToggle callback function, we call it
    if (onToggle != NULL)
        onToggle(_state, this);
}

bool THUIToggleSwitch::isToggledOn() {
    return _state == ToggleState::TOGGLE_ON;
}


THUILabeledToggle* THUILabeledToggle::create(const std::string& text) {
    auto ret = new (std::nothrow) THUILabeledToggle();
    if(ret && ret->init(text)) {
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THUILabeledToggle::isToggledOn() {
    return _toggle->isToggledOn();
}

void THUILabeledToggle::setToggledOn(bool toggle) {
    _toggle->setToggleState(toggle ? ToggleState::TOGGLE_ON : ToggleState::TOGGLE_OFF);
}

void THUILabeledToggle::setOnToggleCallback(const callbackToggleFunc& func) {
    _toggle->onToggle = func;
}

void THUILabeledToggle::setVisible(bool visible) {
    Node::setVisible(visible);
    
    _label->setVisible(visible);
    _toggle->setVisible(visible);
}

void THUILabeledToggle::setSwitchTag(int tag) {
    _toggle->setTag(tag);
}

bool THUILabeledToggle::init(const std::string& text) {
    if ( !Node::init() ) {
        return false;
    }
    
    _label = Label::createWithTTF(text, "fonts/AGENCY.TTF", 16);
    _label->setPosition(Vec2(-10, 0));
    _label->setAnchorPoint(Vec2(1, 0.5));
    
    _toggle = THUIToggleSwitch::create();
    _toggle->setPosition(Vec2(30, 0));
    _toggle->setAnchorPoint(Vec2(0, 0.5));
    
    addChild(_label);
    addChild(_toggle);
    
    _toggle->setToggleEnable(true);
    
    return true;
}

