#include "BallsScene.h"

#include "BallGenerator.h"
#include "GameState.h"
#include "NativeCaller.h"
#include "ResultScene.h"
#include "SimpleAudioEngine.h"
//include "StatisticsReporter.h"

USING_NS_CC;


Scene* BallsScene::createScene() {
    CCLOG("BallsScene::createScene");
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BallsScene::create();
    layer->newLevel();
    
    // add layer as a child to scene
    scene->addChild(layer);
    CCLOG("BallsScene::createScene done");


    // return the scene
    return scene;
}

void BallsScene::setYellow(int yellow) {
    _yellow = yellow;
}

void BallsScene::setBlue(int blue) {
    _blue = blue;
}

bool BallsScene::generate() {
    CCLOG("BallsScene::generate");
    
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction));
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto balls = BallGenerator::getInstance();
    
    balls->addButtonArea(Rect(_yellowbutton->getPosition(), _yellowbutton->getContentSize()));
    balls->addButtonArea(Rect(_bluebutton->getPosition(), _bluebutton->getContentSize()));
    balls->addButtonArea(Rect(origin, Size(visibleSize.width, _yellowbutton->getContentSize().height / 2)));
    
    balls->setContentSize(origin.x, origin.y, visibleSize.width, visibleSize.height);
    balls->setResolution(128); // todo: change resolution on other platforms?
    balls->generate(_yellow, _blue);
    balls->createSprites();
    balls->animate();
    
    CCLOG("BallsScene::generate num balls to render: %zd", balls->_balls_array.size());
    
    for (auto str: balls->_balls_array) {
        Ball *ball = dynamic_cast<Ball *>(str);
        
        ball->_ball_sprite->setZOrder(1);
        ball->_shadow_sprite->setZOrder(0);
        
        this->addChild(ball->_ball_sprite);
        this->addChild(ball->_shadow_sprite);
    }
  
    schedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction), 2.25f + BALL_VISIBLE_TIME);
    schedule(CC_SCHEDULE_SELECTOR(BallsScene::inputBeginAction), 2.25f);
    schedule(CC_SCHEDULE_SELECTOR(BallsScene::playBounceEffect), 0.75f);
    _tmp_counter = 0;
    _tmp_time = milliseconds_now();
    _tmp_start_time = _tmp_time;
    CCLOG("start: %llu", milliseconds_now());
    //schedule(CC_SCHEDULE_SELECTOR(BallsScene::updateAction), 0.0001f); // needed
    
    //Set Ad to hidden
    NativeCaller::hideAd();
    // Load new Main Ad
    CCLOG("Load Main Ad");
    NativeCaller::loadNewMainAd();
    
    CCLOG("BallsScene::generate done");
    
    // Test hiscore call
/*    CCLOG("Test StatisticsReporter::get_hiscore");
    player p;
	p.device_hash = NativeCaller::getDeviceHash();
	p.gender = std::string("MALE");
	p.date_of_birth = std::string("2000-01-01");
	_sr.hiscore_callback = CC_CALLBACK_0(BallsScene::hiscore_callback, this);
	_sr.get_hiscore(p);
    GraphLine* gl_90th = new GraphLine();
    GraphLine* gl_10th = new GraphLine(); */
//	while(!sr.has_hiscore()) { // Wait for hiscore response to come in
//		CCLOG("Sleep 1 seconds");
//    	std::chrono::seconds duration( 1 );
//		std::this_thread::sleep_for(duration);
//	}
//	sr.parse_hiscore(gl_10th, gl_90th);
//	CCLOG("Test StatisticsReporter::get_hiscore - SUCCESS");

    return true;
}

void BallsScene::playBounceEffect(float dt) {
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::playBounceEffect));
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/bounce.mp3");
}

void BallsScene::updateAction(float dt) {
    //char time_elapsed[256];
    
    //sprintf(time_elapsed, "Timer: %.3f", GameState::getInstance()->getElapsed());
    if (_tmp_time != milliseconds_now()) {
        BallGenerator::getInstance()->debug(_tmp_start_time);
        _tmp_time = milliseconds_now();
    }
    
    //_elapsed_label->setString(time_elapsed);
}

void BallsScene::displayAnswer() {
	CCLOG("Showing Main Ad");
	show_ad_if_time();

	CCLOG("Report statistics");
	GameState* gs = GameState::getInstance();
	BallColor answer = gs->_levels[gs->_current_level]->picked_answer;
	BallColor actual = gs->_levels[gs->_current_level]->correct_color;
//	BallColor actual = (gs->level_correct() && answer == BallColor::BLUE)
//			|| (!gs->level_correct() && answer == BallColor::YELLOW) ? BallColor::BLUE : BallColor::YELLOW;
//    statReporter.report_statistics(actual == BallColor::BLUE ? "BLUE" : "YELLOW", answer == BallColor::BLUE ? "BLUE" : "YELLOW");
    
    CCLOG("BallsScene::dislay_answer");
    if (GameState::getInstance()->levelCorrect()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/correct.mp3");
        
        _res_label->setString("CORRECT ANSWER");
    }
    else {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/fail.mp3");
        
        _res_label->setString("INCORRECT ANSWER");
    }
    
    char str[64];
    
    sprintf(str, "RESPONSE TIME %dms", (int)(GameState::getInstance()->getElapsed() * 1000.0f));
    
    _ms_label->setString(str);
    
    _ms_label->setVisible(true);
    _res_label->setVisible(true);
    _continue_label->setVisible(true);
    
    _bluebutton->setVisible(false);
    _yellowbutton->setVisible(false);
    
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}

bool BallsScene::init() {
    if ( !LayerColor::initWithColor(Color4B(255,255,255,255)) ) {
        return false;
    }
    
    CCLOG("BallsScene::init");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Color3B font_color = Color3B(155, 155, 155);
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(BallsScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto background = Sprite::create("background.png");
    background->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    background->setScaleX((visibleSize.width / background->getContentSize().width) * _scaleX);
    background->setScaleY((visibleSize.height / background->getContentSize().height) * _scaleY);
    background->setAnchorPoint(Vec2(0.5,0.5));
    addChild(background);
    
    _res_label = Label::createWithTTF("XXX", "fonts/FRAMD.TTF", 16);
    _res_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - _res_label->getContentSize().height - 40));
    _res_label->setColor(font_color);
    
    
    _ms_label = Label::createWithTTF("XXX", "fonts/FRAMD.TTF", 16);
    _ms_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - _ms_label->getContentSize().height - 70));
    _ms_label->setColor(font_color);
    
    _continue_label = Label::createWithTTF("TOUCH ANYWHERE TO PROCEED", "fonts/FRAMD.TTF", 16);
    _continue_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                     origin.y + visibleSize.height/2 - _continue_label->getContentSize().height));
    _continue_label->setColor(font_color);
    
    _progress_label = Label::createWithTTF("", "fonts/FRAMD.TTF", 8);
    _progress_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                      origin.y + _progress_label->getContentSize().height + 20));
    _progress_label->setColor(font_color);
    
    addChild(_progress_label);
    
    _continue_label->setVisible(false);
    _res_label->setVisible(false);
    _ms_label->setVisible(false);
    
    // add the label as a child to this layer
    this->addChild(_ms_label, 1);
    this->addChild(_res_label, 1);
    this->addChild(_continue_label, 2);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    _bluebutton = Button::create("button_blue.png", "button_blue_pushed.png", "button_blue.png");
    _bluebutton->setAnchorPoint(Vec2(0,0));
    _bluebutton->setPosition(Vec2(origin.x, origin.y));
    _bluebutton->addClickEventListener(CC_CALLBACK_0(BallsScene::bluePressed, this, _bluebutton));
    
    addChild(_bluebutton, 1);
    
    _yellowbutton = Button::create("button_yellow.png", "button_yellow_pushed.png", "button_yellow.png");
    _yellowbutton->setAnchorPoint(Vec2(0,0));
    _yellowbutton->setPosition(Vec2(origin.x + visibleSize.width - _yellowbutton->getContentSize().width,
                                    origin.y));
    
    _yellowbutton->addClickEventListener(CC_CALLBACK_0(BallsScene::yellowPressed, this, _yellowbutton));
    
    addChild(_yellowbutton, 1);
    
    _touch_listener = EventListenerTouchOneByOne::create();
    
    _touch_listener->onTouchEnded = CC_CALLBACK_2(BallsScene::onTouchEnded, this);
    _touch_listener->onTouchBegan = CC_CALLBACK_2(BallsScene::onTouchBegan, this);
    _touch_listener->onTouchMoved = CC_CALLBACK_2(BallsScene::onTouchMoved, this);
    _touch_listener->onTouchCancelled = CC_CALLBACK_2(BallsScene::onTouchCancelled, this);
    
    
    GameState::getInstance()->onGameInterrupted = CC_CALLBACK_0(BallsScene::onGameInterrupted, this);
    GameState::getInstance()->onGameResumed = CC_CALLBACK_0(BallsScene::onGameResumed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touch_listener, this);
    
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
    
    _yellowbutton->setTouchEnabled(false);
    _bluebutton->setTouchEnabled(false);
    
    CCLOG("BallsScene::init done");
    
    return true;
}

void BallsScene::onGameInterrupted() {
    CCLOG("BallsScene::onGameInterrupted");
}

void BallsScene::onGameResumed() {
    CCLOG("BallsScene::onGameResumed");
}


void BallsScene::yellowPressed(Ref* btn) {
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction));
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::updateAction));
    
    giveAnswer(BallColor::YELLOW);
}

void BallsScene::bluePressed(Ref* btn) {
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction));
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::updateAction));
    

    giveAnswer(BallColor::BLUE);
}

void BallsScene::giveAnswer(BallColor color) {
    GameState::getInstance()->endClock();
    CCLOG("BallsScene::giveAnswer");
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction));
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::updateAction));
    
    BallGenerator::getInstance()->hide();

    CCLOG("picking color to: %d", color);
    
    GameState::getInstance()->setPickedColor(color);
    displayAnswer();
    GameState::getInstance()->endLevel();
}

bool BallsScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    return true;
}

void BallsScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void BallsScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {}

void BallsScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    CCLOG("BallsScene::onTouchEnded");
    if (GameState::getInstance()->levelEnded()) {
        
        if (GameState::getInstance()->isLastLevel()) {
            //GameState::getInstance()->endGame();
            
            auto result = ResultScene::createScene();
            
            Director::getInstance()->pushScene(result);

        }
        else {
            GameState::getInstance()->moveToNextLevel();
            newLevel();
        }
    }
}

void BallsScene::hideBallsAction(float dt) {
    CCLOG("BallsScene::hideBallsAction");
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::hideBallsAction));
    
    BallGenerator::getInstance()->hide();
}

void BallsScene::inputBeginAction(float dt) {
    CCLOG("BallsScene::inputBeginAction");
    unschedule(CC_SCHEDULE_SELECTOR(BallsScene::inputBeginAction));

    GameState::getInstance()->startClock();
    _bluebutton->setTouchEnabled(true);
    _yellowbutton->setTouchEnabled(true);
}

void BallsScene::newLevel() {
    CCLOG("BallsScene::newLevel");
    _bluebutton->setTouchEnabled(false);
    _yellowbutton->setTouchEnabled(false);
    
	CCLOG("Hide ad");
    NativeCaller::hideAd();

    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
    
    GameState::getInstance()->startLevel();

    CCASSERT(GameState::getInstance()->numBlue() > 0, "invalid num balls: blue");
    CCASSERT(GameState::getInstance()->numYellow() > 0, "invalid num balls: yellow");
    
    CCLOG("BallsScene::next_level ballcounts %d %d", GameState::getInstance()->numBlue(), GameState::getInstance()->numYellow());
    setBlue(GameState::getInstance()->numBlue());
    setYellow(GameState::getInstance()->numYellow());
    
    CCLOG("num blue: %d, num yellow: %d", GameState::getInstance()->numBlue(), GameState::getInstance()->numYellow());
    
    _bluebutton->setVisible(true);
    _yellowbutton->setVisible(true);
    
    _ms_label->setVisible(false);
    _res_label->setVisible(false);
    _continue_label->setVisible(false);
    
    generate();
    
    char progress[16];
    
    sprintf(progress, "%d/%d", GameState::getInstance()->getCurrentLevel()+1, GameState::getInstance()->getNumLevels());
    
    _progress_label->setString(progress);
    
}

void BallsScene::hiscore_callback() {
	CCLOG("BallsScene::hiscore_call_back called!");

//	GraphLine* gl_90th = new GraphLine();
//    GraphLine* gl_10th = new GraphLine();

//    _sr->parse_hiscore(gl_10th, gl_90th);
}

void BallsScene::show_ad_if_time() {
	if(++_ad_counter % _ADD_INTERVAL == 0) {
		NativeCaller::showMainAd();
	}
}


void BallsScene::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
