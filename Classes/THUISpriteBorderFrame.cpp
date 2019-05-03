#include "THUISpriteBorderFrame.h"


THUISpriteBorderFrame* THUISpriteBorderFrame::create(const Size& size) {
    auto ret = new (std::nothrow) THUISpriteBorderFrame();
    if(ret && ret->init(size)) {
        ret->autorelease();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

THUISpriteBorderFrame::THUISpriteBorderFrame() {}

THUISpriteBorderFrame::~THUISpriteBorderFrame() {

}

void THUISpriteBorderFrame::redraw() {
    removeAllChildren();
    
    /** CORNERS **/
    _sp[FP_TOPLEFT]->setPosition(Vec2(0, getContentSize().height));
    _sp[FP_TOPRIGHT]->setPosition(Vec2(getContentSize().width, getContentSize().height));
    _sp[FP_BOTTOMRIGHT]->setPosition(Vec2(getContentSize().width, 0));
    _sp[FP_BOTTOMLEFT]->setPosition(Vec2(0,0));
    
    
    /** SIDES **/
    float side_height = _sp[FP_TOPLEFT]->getContentSize().height + _sp[FP_BOTTOMLEFT]->getContentSize().height;
    float exp_vertical = getContentSize().height - side_height;
    
    float top_side_width = _sp[FP_TOPLEFT]->getContentSize().width + _sp[FP_TOPRIGHT]->getContentSize().width;
    float bottom_side_width = _sp[FP_BOTTOMLEFT]->getContentSize().width + _sp[FP_BOTTOMRIGHT]->getContentSize().width;
    
    float exp_top_width = getContentSize().width - top_side_width;
    float exp_bottom_width = getContentSize().width - bottom_side_width;
    
    
    _sp[FP_LEFT]->setPosition(Vec2(0.0f, exp_vertical / 2 + _sp[FP_BOTTOMLEFT]->getContentSize().height));
    _sp[FP_LEFT]->setScale(1.0f, (exp_vertical + 1.0f) / _sp[FP_LEFT]->getContentSize().height);
    
    _sp[FP_RIGHT]->setPosition(Vec2(getContentSize().width, exp_vertical / 2 + _sp[FP_BOTTOMRIGHT]->getContentSize().height));
    _sp[FP_RIGHT]->setScale(1.0f, (exp_vertical + 1.0f) / _sp[FP_RIGHT]->getContentSize().height);
    
    _sp[FP_TOP]->setPosition(Vec2(exp_top_width / 2 + _sp[FP_TOPLEFT]->getContentSize().width, getContentSize().height));
    _sp[FP_TOP]->setScale(exp_top_width / _sp[FP_TOP]->getContentSize().width, 1.0f);
    
    _sp[FP_BOTTOM]->setPosition(Vec2(exp_bottom_width / 2 + _sp[FP_BOTTOMLEFT]->getContentSize().width, 0.0f));
    _sp[FP_BOTTOM]->setScale(exp_bottom_width / _sp[FP_BOTTOM]->getContentSize().width, 1.0f);
    
    
    for (auto s : _sp) {
        //CCLOG("adding child");
        addChild(s);
    }

}

bool THUISpriteBorderFrame::init(const Size& size) {
    if ( !Layer::init() ) {
        return false;
    }
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //topleft, topright, bottomleft, bottomright are non expandable
    
    //left, right, top, bottom are expandable
    
    setContentSize(size);
    
    _sp.push_back(Sprite::create("cmb_frame_left.png"));
    _sp.push_back(Sprite::create("cmb_frame_topleft.png"));
    _sp.push_back(Sprite::create("cmb_frame_top.png"));
    _sp.push_back(Sprite::create("cmb_frame_topright.png"));
    _sp.push_back(Sprite::create("cmb_frame_right.png"));
    _sp.push_back(Sprite::create("cmb_frame_bottomright.png"));
    _sp.push_back(Sprite::create("cmb_frame_bottom.png"));
    _sp.push_back(Sprite::create("cmb_frame_bottomleft.png"));
    
    _sp[FP_LEFT]->setAnchorPoint(Vec2(0.0f, 0.5f));
    _sp[FP_TOPLEFT]->setAnchorPoint(Vec2(0.0f, 1.0f));
    _sp[FP_TOP]->setAnchorPoint(Vec2(0.5f, 1.0f));
    _sp[FP_TOPRIGHT]->setAnchorPoint(Vec2(1.0f, 1.0f));
    _sp[FP_RIGHT]->setAnchorPoint(Vec2(1.0f, 0.5f));
    _sp[FP_BOTTOMRIGHT]->setAnchorPoint(Vec2(1.0f, 0.0f));
    _sp[FP_BOTTOM]->setAnchorPoint(Vec2(0.5f, 0.0f));
    _sp[FP_BOTTOMLEFT]->setAnchorPoint(Vec2(0.0f, 0.0f));
    
    for (auto sp : _sp) {
        sp->retain();
    }
    
    redraw();
    
    return true;
}