#include "THUIRollList.h"
#include "fw_drawfuncs.h"
#include "Common.h"
#include "THDrawSmooth.h"

/**** THUIRollListItem *******/

THUIRollListItem* THUIRollListItem::create(const std::string& list_id, const std::string& id, int index, const std::string& text) {
    auto ret = new (std::nothrow) THUIRollListItem();
    if(ret && ret->init(list_id, id, index, text)) {
        ret->autorelease();
        ret->retain();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

std::string THUIRollListItem::getId() {
    return _id;
}


std::string THUIRollListItem::getText() {
    return _text;
}

bool THUIRollListItem::init(const std::string& list_id, const std::string& id, int index, const std::string& text) {
    if ( !Node::init() ) {
        return false;
    }
    
    _list_id = list_id;
    _id = id;
    _text = text;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    auto label = Label::createWithTTF(text, "fonts/FRAMD.TTF", 10);
    label->setPosition(Vec2(0, 0));
    label->setAnchorPoint(Vec2(0, 1));
    label->setColor(Color3B::BLACK);
    
    addChild(label);
    
    setContentSize(label->getContentSize());
    
    return true;
}

std::string THUIRollListItem::getListId() {
    return _list_id;
}

float THUIRollListItem::getWidth() {
    return getContentSize().width + 10;
}

/**** THUIRollList *******/

THUIRollList::THUIRollList() {
    _selected_item = NULL;
}

THUIRollList::~THUIRollList() {
    for (auto label : _items) {
        label->release();
    }
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

THUIRollList* THUIRollList::create(const std::vector<THUIRollListItem*>& items, Color4B color, Size size) {
    auto ret = new (std::nothrow) THUIRollList();
    if(ret && ret->init(items, color, size)) {
        ret->autorelease();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THUIRollList::init(const std::vector<THUIRollListItem*>& items, Color4B color, Size size) {
    if ( !LayerColor::initWithColor(color) ) {
        return false;
    }
    
    setAnchorPoint(Vec2(0,1));
    setContentSize(size);
    
    _items = items;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(THUIRollList::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(THUIRollList::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(THUIRollList::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(THUIRollList::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    isTouchDown = false;
    
    initialTouchPos[0] = 0;
    initialTouchPos[1] = 0;
    
    setRollDirection(RollDirection::NATURAL);
    
    this->scheduleUpdate();
    
    return true;
}

THUIRollListItem* THUIRollList::getItemSelected() {
    return _selected_item;
}

void THUIRollList::setSnap(bool snap) {
    _snap = snap;
}

Vec2 THUIRollList::getWorldPosition() const {
    return convertToWorldSpace(Vec2(_anchorPoint.x * getPosition().x, _anchorPoint.y * getPosition().y));
}


bool THUIRollList::onTouchBegan(Touch *touch, Event *event) {
    Vec2 p = convertTouchToNodeSpaceAR(touch);
    Size s = getContentSize();
    
    if ((-s.width / 2) - p.x < 0
        && s.width / 2 + p.x > 0
        && (-s.height / 2) + p.y < 0
        && (s.height / 2) - p.y > 0) {
        
    }
    else {
        return false;
    }
    
    initialTouchPos[0] = touch->getLocation().x;
    initialTouchPos[1] = touch->getLocation().y;
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    _velocity = 0;
    
    
    _initial_list_pos = _item_container->getPosition();
    _swipe_timer = milliseconds_now();
    _touch_samples.clear();
    _touch_samples.push_back(touch->getLocation());
    
    isTouchDown = true;
    
    _touch_down_timer = milliseconds_now();
    
    return true;
}

void THUIRollList::onTouchMoved(Touch *touch, Event *event) {
    currentTouchPos[0] = touch->getLocation().x;
    currentTouchPos[1] = touch->getLocation().y;
    
    // take a sample each 50th millisecond
    if ((milliseconds_now() - _swipe_timer + 1) > 50 ) {
        _touch_samples.push_back(touch->getLocation());
        _swipe_timer = milliseconds_now();
        
        // we only need 3 samples
        if (_touch_samples.size() > 3) {
            _touch_samples.erase(_touch_samples.begin());
        }
    }
}

void THUIRollList::onTouchEnded(Touch *touch, Event *event) {
    isTouchDown = false;
    
    // detect tap
    if (fabs(initialTouchPos[1] - currentTouchPos[1]) < 5 && milliseconds_now() - _touch_down_timer < 300) {
        getItemTapped(initialTouchPos[1]);
        snapToCenter();
        return;
    }
    else if (milliseconds_now() - _touch_down_timer > 500) {
        snapToCenter();
        return;
    }
    
    if (_touch_samples.size() < 3) {
        snapToCenter();
        return;
    }
    
    float v = (_touch_samples[0].y - _touch_samples[1].y) / 50.0f;
    float v2 = (_touch_samples[1].y - _touch_samples[2].y) / 50.0f;
        
    v = (v + v2) / 2.0f;
    
    if (fabs(v) < 0.1f) {
        snapToCenter();
        return;
    }
    
    v *= 800.0f;
    
    _velocity = v * _roll;
    
    _roll_duration_timer = milliseconds_now();
    
    if (initialTouchPos[1] > currentTouchPos[1]) {
        // swiped down
        _roll_vector.y = -1.0f * _roll;
    }
    else if (initialTouchPos[1] < currentTouchPos[1]) {
        // swiped up
        _roll_vector.y = +1.0f * _roll;
    }
}

void THUIRollList::onTouchCancelled(Touch *touch, Event *event) {
    onTouchEnded(touch, event);
}

THUIRollListItem* THUIRollList::getItemTapped(float pos) {
    CCLOG("getItemTapped: %f %f", pos, _item_container->getPosition().y);
    
    return NULL;
}

void THUIRollList::update(float dt) {
    
    if (isTouchDown) {
        followFinger();
    }
    else if (_velocity != 0){
        rollInDirection(_roll_duration_timer, milliseconds_now(), dt);
    }
}

void THUIRollList::setItems(const std::vector<THUIRollListItem*>& items) {
    _items = items;
}

void THUIRollList::setRollDirection(RollDirection dir) {
    _roll = dir;
}

void THUIRollList::followFinger() {
    float delta = initialTouchPos[1] - currentTouchPos[1];
    
    Vec2 move = _initial_list_pos + Graf::v2fmult(Vec2(0,delta), _roll);
    
    if (constrainRoll(move)) {
        //CCLOG("followFinger has been constrained");
        snapToCenter();
        return;
    }
    
    _item_container->setPosition(move);
}

bool THUIRollList::constrainRoll(Vec2& out_pos) {
    // roll list 0, container 0 means the container is at the top, container should not be less than 0
    if (out_pos.y + getContentSize().height/2 < getContentSize().height) {
        out_pos.y = getContentSize().height/2;
        return true;
    }
    else if (out_pos.y - (_item_container->getContentSize().height + getContentSize().height/2) > 0) {
        out_pos.y = _item_container->getContentSize().height + getContentSize().height/2;
        return true;
    }
    
    return false;
}

void THUIRollList::setAnchorPoint(const Vec2& no) {
    // we only allow anchor point (0,1) atm
}

void THUIRollList::rollInDirection(std::uint64_t start, std::uint64_t now, float dt) {
    float v = _velocity * dt;
    bool end = false;
    
    // when should the animation end? always 1 second?
    if (now - start > 1000 && now - start < 2000) {
        float mul = 1.0f - (powf(((now - start) - 1000.0f) / 1000.000f, 1.0f / 3.0f));
        v = v * mul;
    }
    else if (now - start > 2000) {
        _velocity = 0;
        v = 0;
        
        end = true;
    }
    
    //CCLOG("v: %f", v);
    
    //first second, we move at the start velocity
    Vec2 next_pos =
        _item_container->getPosition()
      + Vec2(0, v);
    
    if (constrainRoll(next_pos)) {
        _velocity = 0;
        end = true;
    }
    
    _item_container->setPosition(next_pos);
    
    if (end)
        snapToCenter();
}

float THUIRollList::getMaxWidth() {
    float max = 0;
    
    for (auto i : _items) {
        max = max < i->getWidth() ? i->getWidth() : max;
    }
    
    return max;
}

void THUIRollList::setCenter(const Vec2& pos) {
    _center = pos;
}

void THUIRollList::snapToCenter() {
    
    //find the item we need to center on
    Vec2 list_pos = _item_container->getPosition();

    Size frame_size = getContentSize();
    float hidden = list_pos.y - frame_size.height;
    float item_at_pos = -(hidden + frame_size.height/2);
    
    THUIRollListItem* snap_to_item;
    
    for (auto i : _item_container->getChildren()) {
        if (i->getTag() == TH_ROLL_DRAWSMOOTH_TAG)
            continue;
        
        THUIRollListItem* item = &(*(THUIRollListItem*)i);
        
        snap_to_item = item;
        
        if (item->getPosition().y < item_at_pos + (_label_height-1) && item->getPosition().y > item_at_pos - (_label_height/2 +1) ) {
            
            break;
        }
    }
    
    _item_container->setPosition(list_pos.x, (snap_to_item->getPosition().y - frame_size.height/2) * -1 + (_label_height/2-2.25f));
    
    _selected_item = snap_to_item;
}

bool THUIRollList::make() {
    auto smooth_drawer = THDrawSmooth::create();
    
    _roll_vector = Vec2(0,0);
    _item_container = Layer::create();
    _item_container->setAnchorPoint(Vec2(0,1));

    _item_container->setPosition(0, getContentSize().height);
    
    Size content(0,0);
    Vec2 label_pos(10, 0);
    
    for (auto label : _items) {
        label->setPosition(label_pos);
        _item_container->addChild(label);

        float height = label->getContentSize().height;
        
        label_pos.y -= height + 5; // TODO: set padding
        
        content.height += height + 5;
        content.width = content.width < label->getContentSize().width ? label->getContentSize().width : content.width;
        
        smooth_drawer->drawLine(
                Vec2(5, label_pos.y+2.5f),
                Vec2(getContentSize().width - 5, label_pos.y+2.5f), Color4B(189,189,189,255), 0.8f);
        
        _label_height = height + 5;
    }
    _item_container->setContentSize(Size(getContentSize().width, content.height));
    _item_container->addChild(smooth_drawer);
    smooth_drawer->setTag(TH_ROLL_DRAWSMOOTH_TAG);
    
    _background = Sprite::create("combo_background.png");
    // scale background to fit
    _background->setScale( ( getContentSize().width / _background->getContentSize().width ), ( getContentSize().height / _background->getContentSize().height ) );
    _background->setAnchorPoint(Vec2(0,0));
    
    addChild(_item_container);
    addChild(_background);
    
    snapToCenter();
    
    return true;
}

void THUIRollList::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    
    _beforeCmd.init(_globalZOrder);
    _beforeCmd.func = CC_CALLBACK_0(THUIRollList::onBeforeVisitScissor, this);
    renderer->addCommand(&_beforeCmd);
    
    LayerColor::visit(renderer, parentTransform, parentFlags);
    
    _afterCmd.init(_globalZOrder);
    _afterCmd.func = CC_CALLBACK_0(THUIRollList::onAfterVisitScissor, this);
    renderer->addCommand(&_afterCmd);
}

void THUIRollList::onBeforeVisitScissor() {
    Size size = getContentSize();
    Vec2 pos = convertToWorldSpace(Vec2(0,0));
    
    
    glEnable(GL_SCISSOR_TEST);
    
    Director::getInstance()->getOpenGLView()->
    setScissorInPoints(pos.x, pos.y, size.width, size.height);
}

void THUIRollList::onAfterVisitScissor() {
    glDisable(GL_SCISSOR_TEST);
}