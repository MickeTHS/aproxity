#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include <vector>
#include "THUIRollList.h"
#include "THGlobal.h"

// we reserve these tags for roll lists
#define ROLL_LIST_TAG 10050
#define ROLL_LIST_END_TAG 10100

class THUITouchableSprite;
class THUIRollListItem;
class THUISpriteBorderFrame;

using namespace cocos2d;
using namespace ui;

enum TH_SelectionType {
    TH_CMB_MultiSelect = 1,
    TH_CMB_SingleSelect = 2
};

enum TH_CMB_EventType {
    TH_CMB_Event_Cancel = 0,
    TH_CMB_Event_Done = 1
};

struct THCBItem {
    THCBItem() {}
    THCBItem(const std::string& i_id, const std::string& i_val) : id(i_id), val(i_val) {}
    virtual ~THCBItem() {}
    
    std::string id;
    std::string val;
};

struct THCBColumn {
    THCBColumn(const std::string& c_id) : id(c_id) {}
    virtual ~THCBColumn() {}
    
    std::string id;
    std::vector<THCBItem> items;
};

struct THCBSelection {
    THCBSelection() {}
    virtual ~THCBSelection() {}
    
    std::string column_id;
    THCBItem item;
};

struct THUICmbCol {
    THUICmbCol(const std::string& name, float width) {
        _name = name;
        _width = width;
    }
    
    virtual ~THUICmbCol() {}
    
    std::string _name;
    float _width;
};

class THUIComboBoxFrame : public Node {
public:
    static THUIComboBoxFrame* create(const std::vector<THUICmbCol>& col_widths, Size size);
    
    THUIComboBoxFrame();
    virtual ~THUIComboBoxFrame();
    
    virtual bool init(const std::vector<THUICmbCol>& col_widths, Size size);
    
    void generate(const std::vector<THCBColumn>& columns);
    
    void setFrameSpriteRect(Vec2 bottomleft, Vec2 topright);
    
    void cancelButton(Ref* btn);
    void doneButton(Ref* btn);
    //void setMinimumWidth(float width);
    
    std::function<void(std::vector<THUIRollListItem*>, TH_CMB_EventType)> onClosed;
    
    THUISpriteBorderFrame* _sprite_frame;
    std::vector<THUIRollList*> _roll_lists;
    
    Vec2 _bottomleft;
    Vec2 _topright;
    
    std::vector<THUICmbCol> _column_widths;
    
    //float _minimum_width;
};

class THUIComboBox : public Node {
public:
    THUIComboBox();
    virtual ~THUIComboBox();
    
    static THUIComboBox* create(const std::string& text, const std::string& button_image);
    static THUIComboBox* create(const std::string& text, const std::string& button_image, const std::vector<THUICmbCol>& col_widths);
    
    virtual bool init(const std::vector<THUICmbCol>& col_widths, const std::string& text, const std::string& button_image);
    virtual bool init(const std::string& text, const std::string& button_image);
    void addItem(const std::string& column_id, const std::string& id, const std::string& value);
    
    std::function<void(std::vector<THCBSelection>, TH_CMB_EventType)> onClosed;
    
    std::vector<THCBColumn> columns;
private:
    void onClicked();
    void onFrameClosed(std::vector<THUIRollListItem*>, TH_CMB_EventType);
    
    std::vector<THUICmbCol> _column_widths;
    
    THUITouchableSprite* _dropdown_button;
    THUIComboBoxFrame* _frame;
    
    Label* _label;
    
    bool _changed;
};
