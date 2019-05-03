#pragma once

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include <stdlib.h>
#include <string>
#include "ansa.h"

#define COCOS2D_DEBUG 1

using namespace cocos2d;

class Ball : public Node {
public:
    
    Ball();
    virtual ~Ball();
    
    CREATE_FUNC(Ball);
    
    BallColor   color;
    float       scale;
    Vec2        pos;
    int         resolution;
    
    void updateDebug(float dt);
    
    void  setResolution(int res);
    float calcRealWidth();
    float calcRealHeight();
    bool  overlap(Ball* b);
    void  setFinalTexture(Ball* sender);
    
    void  createSprite();
    
    void  animate();
    
    cocos2d::Sprite* _ball_sprite;
    cocos2d::Sprite* _shadow_sprite;
};

class BallGenerator : public Layer {
public:
    static BallGenerator* getInstance();
    
    BallGenerator();
    virtual ~BallGenerator();
    
    CREATE_FUNC(BallGenerator);
    
    virtual bool init();
    
    void hide();
    
    void debug(std::uint64_t start);
    
    void setResolution(int resolution);
    void setContentSize(float origin_x, float origin_y, float width, float height);
    void generate(int num_yellow, int num_blue);
    bool overlaps(Ball* b);
    void addButtonArea(Rect r);
    bool overlapRect(Ball* b, const Rect& r);
    void clearBalls();
    
    void createSprites();
    void animate();
    
    
    
    
    cocos2d::Vector<Ball *> _balls_array;
    float _width;
    float _height;
    float _origin_x;
    float _origin_y;
    int   _resolution;
    
    std::vector<Rect> _button_areas;
};
