#pragma once

#include "cocos2d.h"
#include <vector>
#include <string>

#define TH_ROLL_DRAWSMOOTH_TAG 10111

enum RollDirection {
    NATURAL = -1,
    OPPOSITE = 1
};

using namespace cocos2d;
using namespace ui;

class THUIRollListItem : public Node {
public:
    static THUIRollListItem* create(const std::string& list_id, const std::string& id, int index, const std::string& text);
    
    virtual bool init(const std::string& list_id, const std::string& id, int index, const std::string& text);
    
    int getIndex();
    std::string getText();
    std::string getId();
    std::string getListId();

    float getWidth();
private:
    Label*      _label;
    int         _index;
    std::string _id;
    std::string _text;
    std::string _list_id;
    
};


class THUIRollList : public LayerColor {
public:
    THUIRollList();
    virtual ~THUIRollList();
    
    static THUIRollList* create(const std::vector<THUIRollListItem*>& items, Color4B color, Size size);
    virtual bool init(const std::vector<THUIRollListItem*>& items, Color4B color, Size size);
    bool make();
    Vec2 getWorldPosition() const;
    
    void setSnap(bool snap);
    
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void onBeforeVisitScissor();
    void onAfterVisitScissor();
    
    void setItems(const std::vector<THUIRollListItem*>& items);
    
    /* swipe implementation */
    bool onTouchBegan(Touch *touch, Event *event) override;
    void onTouchMoved(Touch *touch, Event *event) override;
    void onTouchEnded(Touch *touch, Event *event) override;
    void onTouchCancelled(Touch *touch, Event *event) override;
    void update(float dt) override;
    
    float getMaxWidth();
    void setRollDirection(RollDirection dir);
    bool constrainRoll(Vec2& out_pos);

    void rollInDirection(std::uint64_t start, std::uint64_t now, float dt);
    void followFinger();
    void setAnchorPoint(const Vec2& no) override;
    void snapToCenter();
    void setCenter(const Vec2& pos);
    
    THUIRollListItem* getItemTapped(float pos);
    THUIRollListItem* getItemSelected();
    
    std::string id;
private:
    bool    _snap;
    std::uint64_t _swipe_timer;
    std::uint64_t _touch_down_timer;
    std::uint64_t _roll_duration_timer;
    
    bool    isTouchDown;
    
    float   initialTouchPos[2];
    float   currentTouchPos[2];
    float   _velocity;
    
    Vec2    _initial_list_pos;
    
    CustomCommand _beforeCmd;
    CustomCommand _afterCmd;
    
    std::vector<THUIRollListItem*> _items;
    std::vector<Vec2> _touch_samples;
    
    Layer*  _item_container;
    Vec2    _roll_vector;
    
    RollDirection _roll;
    
    Sprite* _background;
    float   _label_height;
    Vec2    _center;
    
    THUIRollListItem* _selected_item;
    
};