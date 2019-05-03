#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "THUILabeledToggle.h"
#include "THUIComboBox.h"

class THUILabeledCheckbox;
class THUILabeledToggle;
class THCBSelection;

#define TOGGLE_BOUNCE_TAG 101
#define TOGGLE_ASK_PRACTICE_TAG 102
#define TOGGLE_LEVEL_COUNTER_TAG 103
#define TOGGLE_LEVEL_BREAK_TAG 104

using namespace cocos2d;
using namespace ui;

class StartScene : public cocos2d::LayerColor {
public:
    virtual bool init() override;
    bool make(bool* parent_lock);
    
    CREATE_FUNC(StartScene);
    
    void backButton(Ref* btn);
    void startButton(Ref* btn);
    
    void onComboBoxAgeClosed (std::vector<THCBSelection> selection, TH_CMB_EventType type);
    void onComboBoxGenderClosed (std::vector<THCBSelection> selection, TH_CMB_EventType type);
    
    bool* _parent_lock;
    
    void setVisible(bool visible) override;
    
    THUILabeledToggle* _do_practice;
};

class AboutScene : public cocos2d::LayerColor {
public:
    virtual bool init();
    bool make(bool* parent_lock);
    
    CREATE_FUNC(AboutScene);
    
    void backButton(Ref* btn);
    
    bool* _parent_lock;
};

class SettingsScene : public cocos2d::LayerColor {
public:
    virtual bool init();
    bool make(bool* parent_lock);
    void onToggle(ToggleState state, THUIToggleSwitch* toggleSwitch);
    
    CREATE_FUNC(SettingsScene);
    
    void backButton(Ref* btn);
    void setVisible(bool visible);
    
    bool* _parent_lock;
};

class SetupGameScene : public cocos2d::LayerColor {
public:
    SetupGameScene();
    virtual ~SetupGameScene();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void make();
    
    void setVisibleButtons(bool visible);
    
    CREATE_FUNC(SetupGameScene);
    
    void buttonClicked(Ref* btn);
    
private:
    bool locked;
    
    SettingsScene*  _settings_layer;
    AboutScene*     _about_layer;
    StartScene*     _start_layer;
    
    Button*         _resumegame_btn;
    Button*         _newgame_btn;
    Button*         _settings_btn;
    Button*         _prev_results_btn;
    Button*         _about_btn;
    Button*         _exit_btn;
    
    Button*     _back_btn;
};
