#pragma once

#include <vector>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

enum FramePart {
    FP_LEFT = 0,
    FP_TOPLEFT,
    FP_TOP,
    FP_TOPRIGHT,
    FP_RIGHT,
    FP_BOTTOMRIGHT,
    FP_BOTTOM,
    FP_BOTTOMLEFT
};

class THUISpriteBorderFrame : public Layer {
public:
    static THUISpriteBorderFrame* create(const Size& size);
    
    THUISpriteBorderFrame();
    ~THUISpriteBorderFrame();
    
    virtual bool init(const Size& size);
    void redraw();
    
    std::vector<Sprite*> _sp;
};