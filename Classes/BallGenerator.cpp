#include "BallGenerator.h"
#include "Common.h"

static BallGenerator *_shared_ball_generator = NULL;
static float BallSpriteWidth = 0.0f;
static float BallSpriteHeight = 0.0f;

BallGenerator* BallGenerator::getInstance() {
    if (!_shared_ball_generator) {
        _shared_ball_generator = BallGenerator::create();
        _shared_ball_generator->retain();
        _shared_ball_generator->init();
    }
    
    return _shared_ball_generator;
}

Ball::Ball() {}
Ball::~Ball() {}

void Ball::setResolution(int res) {
    resolution = res;
}

float Ball::calcRealHeight() {
    return (BallSpriteHeight * scale);
}

float Ball::calcRealWidth() {
    return (BallSpriteWidth * scale);
}


bool Ball::overlap(Ball* b) {
    int real_width    = calcRealWidth() / 2;
    int real_height   = calcRealHeight() / 2;
    int b_real_width  = b->calcRealWidth() / 2;
    int b_real_height = b->calcRealHeight() / 2;
    
    
    if (pos.x + real_width < b->pos.x - b_real_width) return false;
    if (pos.x - real_width > b->pos.x + b_real_width) return false;
    if (pos.y + real_height < b->pos.y - b_real_height) return false;
    if (pos.y - real_height > b->pos.y + b_real_height) return false;
    
    return true; //overlaps!
}

void Ball::setFinalTexture(Ball* sender) {
    CCASSERT(sender != nullptr, "didnt get ball from timer event");
    
//    char filename[255];

    if (sender->color == BallColor::BLUE) {
//    	sprintf(filename, "res/sphere_blue_%dpx.png", sender->resolution);

        sender->_ball_sprite->setTexture("blue_ball.png");
    }
    else {
//        sprintf(filename, "yellow_ball.png", sender->resolution);

        sender->_ball_sprite->setTexture("yellow_ball.png");
    }
}

void Ball::createSprite() {
    char filename[255];
    sprintf(filename, "res/shadow_%dpx.png", resolution);
    
    _shadow_sprite = Sprite::create(filename); // TODO: different resolutions

    //sprintf(filename, "gray_ball.png", resolution);
    
    _ball_sprite = Sprite::create("gray_ball.png");
    _ball_sprite->setAnchorPoint(Vec2(0.5,0.5));
    _ball_sprite->setPosition(Vec2(pos.x, pos.y+500)); // place the sprite way above
    _ball_sprite->setScale(scale, scale);

    _shadow_sprite->setAnchorPoint(Vec2(0.5,0.5));
    _shadow_sprite->setOpacity(0); // shadow sprite starts as hidden
    _shadow_sprite->setScale(scale, scale); //use width because its not quadratic
    _shadow_sprite->setPosition(Vec2(pos.x, pos.y - (_ball_sprite->getContentSize().height * scale) /2)); // place the shadow at the final position
    
}

void Ball::animate() {
    // BALL ANIMATION
    
    
    //auto move_ease_out = EaseBounceOut::create(move->clone());
    
    
//    auto delay = DelayTime::create(0.25f);
    
    FiniteTimeAction *callback = CallFunc::create(CC_CALLBACK_0(Ball::setFinalTexture, this, this));

    auto move = MoveBy::create(1.8f, Vec2(0, -500));
    
    auto move_ease_out = EaseBounceOut::create(move->clone());
//    auto move_ease_out = EaseElasticOut::create(move->clone());
    auto seq1 = Sequence::create(move_ease_out, nullptr);
    auto a = _ball_sprite->runAction(seq1);
    
    // SHADOW SPRITE
//    auto fadeOut = FadeIn::create(1.5f);
//    auto fade_ease_out = EaseBounceOut::create(fadeOut->clone());
    
    
//    auto seq_fade = Sequence::create(delay2->clone(), fade_ease_out, nullptr);
//    auto a2 = _shadow_sprite->runAction(seq_fade);
    
    auto delay3 = DelayTime::create(2.25f);
    auto seq_change = Sequence::create(delay3->clone(), callback, nullptr);
    _shadow_sprite->runAction(seq_change);
    
    a->setTag(1); // not needed
//    a2->setTag(1);
}

BallGenerator::BallGenerator() {}
BallGenerator::~BallGenerator() {}

// on "init" you need to initialize your instance
bool BallGenerator::init() {
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}


void BallGenerator::addButtonArea(Rect r) {
    _button_areas.push_back(r);
}

void BallGenerator::setContentSize(float origin_x, float origin_y, float width, float height) {
    _origin_x = origin_x;
    _origin_y = origin_y;
    _width = width;
    _height = height;
}

void BallGenerator::createSprites() {
    for (auto str: _balls_array) {
        Ball *ball = dynamic_cast<Ball *>(str);
        ball->createSprite();
    }
}

void BallGenerator::animate() {
    for (auto str: _balls_array) {
        Ball *ball = dynamic_cast<Ball *>(str);
        ball->animate();
    }
}

void BallGenerator::hide() {
    for (auto str: _balls_array) {
        Ball *ball = dynamic_cast<Ball *>(str);
        ball->_ball_sprite->setVisible(false);
        ball->_shadow_sprite->setVisible(false);
        ball->update(0.0f);
    }
}

bool BallGenerator::overlapRect(Ball* b, const Rect& r) {
    int b_real_width = b->calcRealWidth() / 2;
    int b_real_height = b->calcRealHeight() / 2;
    
    if (r.origin.x + r.size.width < b->pos.x - b_real_width) return false;
    if (r.origin.x > b->pos.x + b_real_width) return false;
    if (r.origin.y + r.size.height < b->pos.y - b_real_height) return false;
    if (r.origin.y > b->pos.y + b_real_height) return false;
    
    return true;
}

bool BallGenerator::overlaps(Ball* b) {
    // check button areas
    for (int i = 0; i < _button_areas.size(); ++i) {
        if (overlapRect(b, _button_areas[i])) {
            return true;
        }
    }
    
    
    for (auto str: _balls_array) {
        Ball *ball = dynamic_cast<Ball *>(str);
        if (ball->overlap(b)) {
            return true;
        }
    }
    
    return false;
}

void BallGenerator::setResolution(int resolution) {
    _resolution = resolution;
    
    //char filename[255];
    //sprintf(filename, "res/sphere_neutral_%dpx.png", _resolution);
    
    auto tmp = Sprite::create("gray_ball.png");
    
    BallSpriteWidth = tmp->getContentSize().width;
    BallSpriteHeight = tmp->getContentSize().height;
}

void BallGenerator::clearBalls() {
    for (auto ball : _balls_array) {
        ball->release();
    }
    
    _balls_array.clear();
}

void Ball::updateDebug(float dt) {

    CCLOG("update: %llu %f", milliseconds_now(), getPosition().y);
}

void BallGenerator::debug(std::uint64_t start) {
    
    if (_balls_array.size() == 0)
        return;
    
    Ball* b = _balls_array.at(0);
    
    if (b) {
        CCLOG("update: %llu %f", (milliseconds_now() - start), b->_ball_sprite->getPositionY());
    }
}

void BallGenerator::generate(int num_yellow, int num_blue) {
    clearBalls();
    
    CCASSERT(num_blue + num_yellow < MAX_BALLS, "illegal number of balls");
    
    float r = 0.0f;
    
    int stuck = 0;
    
    for (int i = 0; i < num_yellow; ++i) {
        Ball* b = Ball::create();
        b->retain();
        b->setResolution(_resolution);
        stuck = 0;
        
        // keep generating circle as long as it overlaps another circle
        do {
            if (stuck++ > 50) {
                CCLOG("BallGenerator::generate got stuck doing yellow");
                
                generate(num_yellow, num_blue);
                return;
            }
            
            r = cocos2d::RandomHelper::random_real(0.3f, 1.2f);
            b->scale = r;
            
            int rw = b->calcRealWidth() / 2;
            int rh = b->calcRealHeight() / 2;
            
            
            int x = cocos2d::RandomHelper::random_int((int)rw + (int)_origin_x, (int)(_origin_x + _width - (int)rw));
            int y = cocos2d::RandomHelper::random_int((int)rh + (int)_origin_y, (int)(_origin_y + _height - (int)rh));
            
            b->pos.set(x, y);
            b->color = BallColor::YELLOW;
            
        } while(overlaps(b));
        _balls_array.pushBack(b);
    }
    
    for (int i = 0; i < num_blue; ++i) {
        Ball *b = Ball::create();
        b->retain();
        b->setResolution(_resolution);
        
        stuck = 0;
        
        if (i == 0)
            b->setTag(4444);
        
        // keep generating circle as long as it overlaps another circle
        do {
            if (stuck++ > 50) {
                CCLOG("BallGenerator::generate got stuck doing blue");
                
                generate(num_yellow, num_blue);
                return;
            }
            
            r = cocos2d::RandomHelper::random_real(0.3f, 1.2f);
            b->scale = r;
            
            int rw = b->calcRealWidth() / 2;
            int rh = b->calcRealHeight() / 2;
            
            int x = cocos2d::RandomHelper::random_int((int)rw + (int)_origin_x, (int)(_origin_x + _width - (int)rw));
            int y = cocos2d::RandomHelper::random_int((int)rh + (int)_origin_y, (int)(_origin_y + _height - (int)rh));
            
            b->pos.set(x, y);
            b->color = BallColor::BLUE;
            
        } while(overlaps(b));
        
        _balls_array.pushBack(b);
    }
}
