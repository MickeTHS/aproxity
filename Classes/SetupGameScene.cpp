#include "SetupGameScene.h"
#include "ui/CocosGUI.h"
#include "THUILabeledCheckbox.h"
#include "GameState.h"
#include "BallsScene.h"
#include "NativeCaller.h"
#include "THUIComboBox.h"
#include "THUILabeledToggle.h"
#include "THUISpriteBorderFrame.h"

bool StartScene::init() {
    if ( !LayerColor::initWithColor(Color4B(53,34,54,255)) ) {
        return false;
    }
    
    CCLOG("StartScene::init");
    
    return true;
}

void StartScene::setVisible(bool visible) {
    LayerColor::setVisible(visible);
    
    for (auto c : getChildren()) {
        c->setVisible(visible);
    }
}

void StartScene::onComboBoxAgeClosed (std::vector<THCBSelection> selection, TH_CMB_EventType type) {
    CCLOG("Age closed");

    GameState::getInstance()->_selected_date_of_birth = selection[0].item.id + "-" + selection[1].item.id  + "-" + selection[2].item.id;
}

void StartScene::onComboBoxGenderClosed (std::vector<THCBSelection> selection, TH_CMB_EventType type) {
    CCLOG("Gender closed");
    
    if (type == TH_CMB_Event_Cancel || selection.size() <= 0)
        return;
    
    CCLOG("selected gender: %s", selection[0].item.id.c_str());

    GameState::getInstance()->_selected_gender = selection[0].item.id;
}


bool StartScene::make(bool* locked) {
    _parent_lock = locked;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    auto label1 = Label::createWithTTF("YOUR AGE", "fonts/AGENCY.TTF", 16);
    label1->setPosition(btn_pos);
    label1->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 30;
    
    auto age = THUIComboBox::create("SELECT AGE", "dropdown.png");
    age->setPosition(btn_pos);
    age->setVisible(false);
    
    char str[5];
    
    for (int y = 1915; y <= 2005; ++y) {
        sprintf(str, "%d", y);
        std::string val(str);
        age->addItem("Year", val, val);
    }
    
    for (int m = 1; m <= 12; ++m) {
        sprintf(str, "%d", m);
        std::string val(str);
        age->addItem("Month", val, val);
    }

    for (int d = 1; d <= 31; ++d) {
        sprintf(str, "%d", d);
        std::string val(str);
        age->addItem("Day", val, val);
    }

    
    age->onClosed = CC_CALLBACK_2(StartScene::onComboBoxAgeClosed, this);
    
    btn_pos.y -= 30;
    
    auto label2 = Label::createWithTTF("YOUR GENDER", "fonts/AGENCY.TTF", 16);
    label2->setPosition(btn_pos);
    label2->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 30;

    std::vector<THUICmbCol> gender_cols;
    gender_cols.push_back(THUICmbCol("gender", 100));
    
    auto gender = THUIComboBox::create("SELECT GENDER", "dropdown.png", gender_cols);
    gender->setPosition(btn_pos);
    gender->setVisible(false);
    gender->addItem("gender", "male", "Male");
    gender->addItem("gender", "female", "Female");
    
    
    btn_pos.y -= 30;
    
    gender->onClosed = CC_CALLBACK_2(StartScene::onComboBoxGenderClosed, this);
    
    _do_practice = THUILabeledToggle::create("DO PRACTICE ROUND");
    _do_practice->setPosition(btn_pos);
    _do_practice->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 40;
    
    auto start = Button::create("btn_play.png", "btn_play_pushed.png", "btn_play.png");
    start->setAnchorPoint(Vec2(0.5,0.5));
    start->setPosition(btn_pos);
    start->addClickEventListener(CC_CALLBACK_0(StartScene::startButton, this, start));
    
    btn_pos.y -= 40;

    auto back = Button::create("button.png", "button.png", "button.png");
    back->setAnchorPoint(Vec2(0.5,0.5));
    back->setPosition(btn_pos);
    back->setTitleFontName("fonts/AGENCY.TTF");
    back->setTitleFontSize(24.0f);
    back->setTitleColor(Color3B::BLACK);
    back->setTitleText("Back");
    back->addClickEventListener(CC_CALLBACK_0(StartScene::backButton, this, back));

    addChild(label1);
    addChild(age);
    addChild(label2);
    addChild(gender);
    addChild(_do_practice);
    addChild(back);
    addChild(start);
    
    return true;
}

void StartScene::startButton(Ref* btn) {
    GameState::getInstance()->generateLevels();
    
    auto balls = BallsScene::createScene();
    
    Director::getInstance()->pushScene(balls);
}

void StartScene::backButton(Ref* btn) {
    setVisible(false);
    *_parent_lock = false;
}

bool AboutScene::init() {
    if ( !LayerColor::initWithColor(Color4B(53,34,54,255)) ) {
        return false;
    }
    
    CCLOG("AboutScene::init");
    
    return true;
}

bool AboutScene::make(bool* locked) {
    _parent_lock = locked;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    auto label1 = Label::createWithTTF("ABOUT APROXITY", "fonts/AGENCY.TTF", 16);
    label1->setPosition(btn_pos);
    label1->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 40;

    auto label2 = Label::createWithTTF("THIS IS A GAME THAT WILL TEST YOUR APTITUDE NUMBER SENSE", "fonts/AGENCY.TTF", 12);
    label2->setPosition(btn_pos);
    label2->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 40;

    auto label3 = Label::createWithTTF("THE GAME WAS DEVELOPED BY THINKING HAT STUDIOS AT OUR OFFICE IN STOCKHOLM, SWEDEN", "fonts/AGENCY.TTF", 12);
    label3->setPosition(btn_pos);
    label3->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 40;

    auto label4 = Label::createWithTTF("PLEASE VISIT WWW.THINKINGHATSTUDIOS.COM TO FIND OUR OTHER GAMES AND PROJECTS", "fonts/AGENCY.TTF", 12);
    label4->setPosition(btn_pos);
    label4->setAnchorPoint(Vec2(0.5, 0.5));
    btn_pos.y -= 40;

    
    auto back = Button::create("button.png", "button.png", "button.png");
    back->setAnchorPoint(Vec2(0.5,0.5));
    back->setPosition(btn_pos);
    back->setTitleFontName("fonts/AGENCY.TTF");
    back->setTitleFontSize(24.0f);
    back->setTitleColor(Color3B::BLACK);
    back->setTitleText("Back");
    back->addClickEventListener(CC_CALLBACK_0(AboutScene::backButton, this, back));
    
    addChild(label1);
    addChild(label2);
    addChild(label3);
    addChild(label4);
    addChild(back);
    
    
    return true;
}

void AboutScene::backButton(Ref* btn) {
    setVisible(false);
    *_parent_lock = false;
}

bool SettingsScene::init() {
    if ( !LayerColor::initWithColor(Color4B(53,34,54,255)) ) {
        return false;
    }
    
    CCLOG("SettingsScene::init");
    
    return true;
}

void SettingsScene::setVisible(bool visible) {
    LayerColor::setVisible(visible);
    
    for (auto c : getChildren()) {
        c->setVisible(visible);
    }
}

void SettingsScene::onToggle(ToggleState state, THUIToggleSwitch* toggleSwitch) {
    if (!isVisible())
        return;
    
    
    switch (toggleSwitch->getTag()) {
        case TOGGLE_BOUNCE_TAG:
            GameState::getInstance()->setSetting("BOUNCE", toggleSwitch->isToggledOn() ? "1" : "0");
            break;
        case TOGGLE_ASK_PRACTICE_TAG:
            GameState::getInstance()->setSetting("ASK_PRACTICE", toggleSwitch->isToggledOn()? "1" : "0");
            break;
        case TOGGLE_LEVEL_BREAK_TAG:
            GameState::getInstance()->setSetting("SHOW_LEVEL_BREAK", toggleSwitch->isToggledOn() ? "1" : "0");
            break;
        case TOGGLE_LEVEL_COUNTER_TAG:
            GameState::getInstance()->setSetting("SHOW_LEVEL_COUNTER", toggleSwitch->isToggledOn() ? "1" : "0");
            break;
        default: break;
    }
}

bool SettingsScene::make(bool* locked) {
    _parent_lock = locked;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    auto bounce = THUILabeledToggle::create("BALLS BOUNCE INTO VIEW");
    bounce->setPosition(btn_pos);
    bounce->setSwitchTag(TOGGLE_BOUNCE_TAG);
    bounce->setToggledOn(GameState::getInstance()->compareSetting("BOUNCE", "1"));
    bounce->setOnToggleCallback(CC_CALLBACK_2(SettingsScene::onToggle, this));

    btn_pos.y -= 30;
    
    auto ask_practice = THUILabeledToggle::create("ALWAYS ASK PRACTICE");
    ask_practice->setPosition(btn_pos);
    ask_practice->setSwitchTag(TOGGLE_ASK_PRACTICE_TAG);
    ask_practice->setToggledOn(GameState::getInstance()->compareSetting("ASK_PRACTICE", "1"));
    ask_practice->setOnToggleCallback(CC_CALLBACK_2(SettingsScene::onToggle, this));
    btn_pos.y -= 30;
    

    auto remove_level_counter = THUILabeledToggle::create("SHOW LEVEL COUNTER");
    remove_level_counter->setPosition(btn_pos);
    remove_level_counter->setSwitchTag(TOGGLE_LEVEL_COUNTER_TAG);
    remove_level_counter->setToggledOn(GameState::getInstance()->compareSetting("SHOW_LEVEL_COUNTER", "1"));
    remove_level_counter->setOnToggleCallback(CC_CALLBACK_2(SettingsScene::onToggle, this));
    btn_pos.y -= 30;
    
    auto remove_level_break = THUILabeledToggle::create("SHOW LEVEL BREAK");
    remove_level_break->setPosition(btn_pos);
    remove_level_break->setSwitchTag(TOGGLE_LEVEL_BREAK_TAG);
    remove_level_break->setToggledOn(GameState::getInstance()->compareSetting("SHOW_LEVEL_BREAK", "1"));
    remove_level_break->setOnToggleCallback(CC_CALLBACK_2(SettingsScene::onToggle, this));
    btn_pos.y -= 50;
    
    auto back = Button::create("button.png", "button.png", "button.png");
    back->setAnchorPoint(Vec2(0.5,0.5));
    back->setPosition(btn_pos);
    back->setTitleFontName("fonts/AGENCY.TTF");
    back->setTitleFontSize(24.0f);
    back->setTitleColor(Color3B::BLACK);
    back->setTitleText("Back");
    back->addClickEventListener(CC_CALLBACK_0(SettingsScene::backButton, this, back));
    
    addChild(back);
    addChild(bounce);
    addChild(ask_practice);
    addChild(remove_level_break);
    addChild(remove_level_counter);

    return true;
}

void SettingsScene::backButton(Ref* btn) {
    setVisible(false);
    *_parent_lock = false;
}


Scene* SetupGameScene::createScene() {
    CCLOG("SetupGameScene::createScene");
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SetupGameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    layer->make();
    
    // return the scene
    return scene;
}

SetupGameScene::SetupGameScene() {}
SetupGameScene::~SetupGameScene() {}

bool SetupGameScene::init() {
    if ( !LayerColor::initWithColor(Color4B(255,255,255,255)) ) {
        return false;
    }
    
    locked = false;
    
    CCLOG("SetupGameScene::init");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    _resumegame_btn = Button::create("btn_resume.png", "btn_resume_pushed.png", "btn_resume.png");
    _resumegame_btn->setAnchorPoint(Vec2(0.5,0.5));
    _resumegame_btn->setPosition(btn_pos);
    _resumegame_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _resumegame_btn));
    btn_pos.y -= 30;
    
    _newgame_btn = Button::create("btn_play.png", "btn_play_pushed.png", "btn_play.png");
    _newgame_btn->setPosition(btn_pos);
    _newgame_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _newgame_btn));
    btn_pos.y -= 30;

    _settings_btn = Button::create("btn_settings.png", "btn_settings_pushed.png", "btn_settings.png");
    _settings_btn->setPosition(btn_pos);
    _settings_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _settings_btn));
    btn_pos.y -= 30;

    _prev_results_btn = Button::create("btn_results.png", "btn_results_pushed.png", "btn_results.png");
    _prev_results_btn->setPosition(btn_pos);
    _prev_results_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _prev_results_btn));
    btn_pos.y -= 30;

    _about_btn = Button::create("btn_about.png", "btn_about_pushed.png", "btn_about.png");
    _about_btn->setPosition(btn_pos);
    _about_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _about_btn));
    btn_pos.y -= 30;

    _exit_btn = Button::create("btn_exit.png", "btn_exit_pushed.png", "btn_exit.png");
    _exit_btn->setPosition(btn_pos);
    _exit_btn->addClickEventListener(CC_CALLBACK_0(SetupGameScene::buttonClicked, this, _exit_btn));
    btn_pos.y -= 30;

    addChild(_resumegame_btn);
    addChild(_newgame_btn);
    addChild(_settings_btn);
    addChild(_prev_results_btn);
    addChild(_about_btn);
    addChild(_exit_btn);

    setVisibleButtons(true);

    /** SETTINGS LAYER **/
    _settings_layer = SettingsScene::create();
    _settings_layer->setPosition(origin);
    _settings_layer->setContentSize(visibleSize);
    _settings_layer->make(&locked);
    _settings_layer->setVisible(false);
    addChild(_settings_layer);
    CCLOG("SetupGameScene: post make");
    
    /** ABOUT LAYER **/
    _about_layer = AboutScene::create();
    _about_layer->setPosition(origin);
    _about_layer->setContentSize(visibleSize);
    _about_layer->make(&locked);
    _about_layer->setVisible(false);
    addChild(_about_layer);

    /** NEW GAME LAYER **/
    _start_layer = StartScene::create();
    _start_layer->setPosition(origin);
    _start_layer->setContentSize(visibleSize);
    _start_layer->make(&locked);
    _start_layer->setVisible(false);
    addChild(_start_layer);

    
    /** TEST EXPANDABLE FRAME **/
/*    
    auto frame = THUISpriteBorderFrame::create(Size(233, 222));
    frame->setPosition(origin);
    addChild(frame);
*/
    
    return true;
}

void SetupGameScene::make() {
    /* COMBOBOX TEST */
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto combo = THUIComboBox::create(getScene(), "test", "dropdown.png");
    //combo->setPosition(Vec2(origin.x+100, origin.y+100));
    //addChild(combo);
    
}

void SetupGameScene::buttonClicked(Ref* btn) {
    if (locked) {
        return;
    }
    
    Button* b = (Button*)btn;
    
    _about_layer->setVisible(false);
    _settings_layer->setVisible(false);
    _start_layer->setVisible(false);
    
    if (b == _exit_btn) {
        GameState::getInstance()->exitGame();
    }
    else if (b == _settings_btn) {
        _settings_layer->setVisible(true);
        locked = true;
    }
    else if (b == _about_btn) {
        _about_layer->setVisible(true);
        locked = true;
    }
    else if (b == _newgame_btn) {
        _start_layer->setVisible(true);
        locked = true;
    }
    else if (b == _resumegame_btn) {
        NativeCaller::shareLinkFacebook();
    }
}

void SetupGameScene::setVisibleButtons(bool visible) {
    _resumegame_btn->setVisible(visible);
    _newgame_btn->setVisible(visible);
    _settings_btn->setVisible(visible);
    _prev_results_btn->setVisible(visible);
    _about_btn->setVisible(visible);
    _exit_btn->setVisible(visible);
}
