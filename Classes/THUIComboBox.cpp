#include "THUIComboBox.h"
#include "THUITouchableSprite.h"
#include "THUIRollList.h"
#include "THUISpriteBorderFrame.h"
#include "THDrawSmooth.h"

THUIComboBoxFrame::THUIComboBoxFrame() {
    onClosed = NULL;
}

THUIComboBoxFrame::~THUIComboBoxFrame() {}

THUIComboBoxFrame* THUIComboBoxFrame::create(const std::vector<THUICmbCol>& col_widths, Size size) {
    auto ret = new (std::nothrow) THUIComboBoxFrame();
    if(ret && ret->init(col_widths, size)) {
        ret->autorelease();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool THUIComboBoxFrame::init(const std::vector<THUICmbCol>& col_widths, Size size) {
    Node::init();
    _column_widths = col_widths;
    setContentSize(size);

    /** add the frame to cover the rolllists **/
    _sprite_frame = THUISpriteBorderFrame::create(size);

    addChild(_sprite_frame);
    
    return true;
}

void THUIComboBoxFrame::cancelButton(Ref* btn) {
    if (onClosed == NULL)
        return;

    std::vector<THUIRollListItem*> items;
    
    for (auto r : _roll_lists) {
        items.push_back(r->getItemSelected());
    }
    
    onClosed(items, TH_CMB_EventType::TH_CMB_Event_Cancel);
}

void THUIComboBoxFrame::doneButton(Ref* btn) {
    CCLOG("THUIComboBoxFrame: doneButton");
    
    if (onClosed == NULL)
        return;
    
    CCLOG("doneButton");
    
    std::vector<THUIRollListItem*> items;
    
    for (auto r : _roll_lists) {
        CCLOG("selected item: %d", r->getItemSelected() == NULL ? 0 : 1);
        items.push_back(r->getItemSelected());
    }
    
    CCLOG("items: %lu", items.size());
    
    onClosed(items, TH_CMB_EventType::TH_CMB_Event_Done);
}

void THUIComboBoxFrame::generate(const std::vector<THCBColumn>& columns) {
    float x = 0;
    float margin = 1;
    
    THUIRollList* tmp;
    _roll_lists.clear();
    
    for (auto column : columns) {
        float width = 0;
        std::vector<THUIRollListItem*> ui_items;
        
        for (auto item : column.items) {
            
            auto ui = THUIRollListItem::create(column.id, item.id, 0, item.val);
            width = width < ui->getWidth() ? ui->getWidth() : width;
            
            ui_items.push_back(ui);
        }
        
        // check if there are column widths defined
        // if so, use the user defined column width
        if (_column_widths.size() != 0) {
            for (auto cw : _column_widths) {
                CCLOG("column widths: %s, %f", cw._name.c_str(), cw._width);
                
                if (cw._name.compare(column.id) == 0) {
                    width = cw._width;
                    
                    
                    break;
                }
            }
        }
        else {
            width += 10;
        }
        
        auto roll = THUIRollList::create(ui_items, Color4B::WHITE, Size(width, getContentSize().height-(_bottomleft.y + _topright.y)));

        roll->setPosition(Vec2(_bottomleft.x + x, _bottomleft.y));
        roll->make();
        _roll_lists.push_back(roll);
        x += width + margin;
        addChild(roll);
        tmp = roll;
    }
    
    
    Size final_size = Size(_bottomleft.x + _topright.x + x - margin*2, _sprite_frame->getContentSize().height);
    
    _sprite_frame->setContentSize(final_size);
    _sprite_frame->redraw();
    _sprite_frame->setPosition(Vec2(0,0));
    
    setContentSize(final_size);
    
    auto cancel = Button::create("cmb_button.png", "cmb_button_down.png", "cmb_button.png");
    cancel->setAnchorPoint(Vec2(0,1));
    cancel->setPosition(Vec2(9, getContentSize().height - 3));
    cancel->setTitleFontName("fonts/FRAMD.TTF");
    cancel->setTitleFontSize(10.0f);
    cancel->setTitleColor(Color3B::BLACK);
    cancel->setTitleText("Cancel");
    cancel->addClickEventListener(CC_CALLBACK_0(THUIComboBoxFrame::cancelButton, this, cancel));
    addChild(cancel);
    
    auto done = Button::create("cmb_button.png", "cmb_button_down.png", "cmb_button.png");
    done->setPosition(Vec2(getContentSize().width - 9, getContentSize().height - 3));
    done->setAnchorPoint(Vec2(1,1));
    done->setTitleFontName("fonts/FRAMD.TTF");
    done->setTitleFontSize(10.0f);
    done->setTitleColor(Color3B::BLACK);
    done->setTitleText("Done");
    done->addClickEventListener(CC_CALLBACK_0(THUIComboBoxFrame::doneButton, this, done));
    addChild(done);
    
    
    /*if (columns.size() <= 1) {
        return;
    }*/
    
    auto selector = Sprite::create("cmb_frame_selector.png");
    selector->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    float scale = (getContentSize().width-10) / selector->getContentSize().width;
    
    selector->setScale(scale, scale);
    selector->setPosition((getContentSize().width / 2), _bottomleft.y + tmp->getContentSize().height/2);
    addChild(selector);
}

void THUIComboBoxFrame::setFrameSpriteRect(Vec2 bottomleft, Vec2 topright) {
    _bottomleft = bottomleft;
    _topright = topright;
}

/*void THUIComboBoxFrame::setMinimumWidth(float width) {
    _minimum_width = width;
}*/

THUIComboBox::THUIComboBox() {}

THUIComboBox::~THUIComboBox() {
    if (_frame != nullptr)
        _frame->release();
}

THUIComboBox* THUIComboBox::create(const std::string& text, const std::string& button_image, const std::vector<THUICmbCol>& col_widths) {
    auto ret = new (std::nothrow) THUIComboBox();
    if(ret && ret->init(col_widths, text, button_image)) {
        ret->autorelease();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}


THUIComboBox* THUIComboBox::create(const std::string& text, const std::string& button_image) {
    auto ret = new (std::nothrow) THUIComboBox();
    if(ret && ret->init(text, button_image)) {
        ret->autorelease();
        
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void THUIComboBox::onFrameClosed(std::vector<THUIRollListItem*> items, TH_CMB_EventType event) {
    getScene()->removeChild(_frame);
    
    if (onClosed == NULL)
        return;
    
    CCLOG("onFrameClosed: %lu", items.size());

    std::vector<THCBSelection> selection;
    
    if (event == TH_CMB_EventType::TH_CMB_Event_Done) {
        std::string str;
        std::string delimiter = "";
        _label->setString("");
        
        for (auto i : items) {
            THCBSelection sel;
            
            sel.column_id = i->getListId();
            sel.item.id = i->getId();
            sel.item.val = i->getText();
            
            selection.push_back(sel);
            
            str += delimiter + i->getText();
            delimiter = "-";
        }
        
        CCLOG("the string: %s", str.c_str());
        
        _label->setString(str);
    }
    
    onClosed(selection, event);
}

bool THUIComboBox::init(const std::vector<THUICmbCol>& col_widths, const std::string& text, const std::string& button_image) {
    _column_widths = col_widths;
    return init(text, button_image);
}


bool THUIComboBox::init(const std::string& text, const std::string& button_image) {
    if ( !Node::init() ) {
        return false;
    }
    
    _changed = true;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 btn_pos(origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height - 70);
    
    _dropdown_button = THUITouchableSprite::create(button_image);
    _dropdown_button->setPosition(Vec2(0, 0));
    _dropdown_button->setAnchorPoint(Vec2(0.5, 0.5));
    _dropdown_button->onClicked = CC_CALLBACK_0(THUIComboBox::onClicked, this);
    
    _label = Label::createWithTTF(text, "fonts/FRAMD.TTF", 12);
    _label->setColor(Color3B(147, 147, 147));
    _label->setPosition(Vec2(-_dropdown_button->getContentSize().width/2 + 10, 0));
    _label->setAnchorPoint(Vec2(0, 0.5));
    
    
    addChild(_dropdown_button);
    addChild(_label);
    
    _frame = THUIComboBoxFrame::create(_column_widths, Size(200, 200));
    _frame->setAnchorPoint(Vec2(0.5f, 0.0f));
    _frame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y));
    _frame->setFrameSpriteRect(Vec2(11,7), Vec2(11,22));
    _frame->retain();
    
    _frame->onClosed = CC_CALLBACK_2(THUIComboBox::onFrameClosed, this);
    
    return true;
}

void THUIComboBox::addItem(const std::string& column_id, const std::string& id, const std::string& value) {
    // find column
    for (int i = 0; i < columns.size(); ++i) {
        if (columns[i].id.compare(column_id) == 0) {
            columns[i].items.push_back(THCBItem(id, value));
            return;
        }
    }
    
    THCBColumn col(column_id);
    col.items.push_back(THCBItem(id, value));
    
    columns.push_back(col);
}

void THUIComboBox::onClicked() {
    if (!isVisible())
        return;
    
    _frame->generate(columns);
    
    getScene()->addChild(_frame);
}
