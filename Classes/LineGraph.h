#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "GraphAxis.h"
#include "GraphAxisScale.h"
#include "AxisSort.h"
#include "GraphLine.h"

class THDrawSmooth;

using namespace cocos2d;

struct GraphRect {
    Vec2 bl;
    Vec2 tr;
};

//template<class X, class Y>
class LineGraph : public LayerColor {
public:
    
    static LineGraph* create(const Color4B& background_color, const std::string& ttf_file, float font_size) {
        LineGraph* pRet = new(std::nothrow)LineGraph();
        if (pRet && pRet->init(background_color) && FileUtils::getInstance()->isFileExist(ttf_file)) {
            TTFConfig ttf_config(ttf_file.c_str(), font_size, GlyphCollection::DYNAMIC);
            
            pRet->setTTFConfig(ttf_config);
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }

    LineGraph();
    virtual ~LineGraph();
    
    virtual bool init(const Color4B& background_color);
    
    void setPaddingHorizontal(int pixels);
    void setPaddingVertical(int pixels);
    
    void setTTFConfig(TTFConfig config);
    
    /* make(), this will add all the children to the Layer and draw them */
    void make();
    void redraw();
    
    template<class T>
    void createXAxis(std::string name, T lowest, T highest) {
        if (_x_axis == NULL) {
            _x_axis = new GraphAxis<T>(GraphAxisDir::X_AXIS, name, lowest, highest);
            CCLOG("created axis: %s", _x_axis->getName().c_str());
        }
    }

    void setAxisPixelOrigin(const Vec2& origin);
    void setAxisPixelLength(GraphAxisDir dir, float length);
    
    template<class T>
    void createYAxis(std::string name, T lowest, T highest) {
        if (_y_axis == NULL) {
            _y_axis = new GraphAxis<T>(GraphAxisDir::Y_AXIS, name, lowest, highest);
            CCLOG("created axis: %s", _y_axis->getName().c_str());
        }
    }

    template<class T>
    void addXLabel(std::string name, T value) {
        if (_x_axis == NULL)
            return;
        
        ((GraphAxis<T>*)_x_axis)->insertLabel(name, value);
        
    }

    template<class T>
    void addYLabel(std::string name, T value) {
        if (_y_axis == NULL)
            return;
        
        ((GraphAxis<T>*)_y_axis)->insertLabel(name, value);
    }
    
    void addLine(GraphLine* line);
    

    /* this will override the dynamic drawing of axis and background and instead just use
       a texture for the entire graph */
    void setTextureDraw(const std::string& filename);
    void setTextureDrawHelp(bool draw_help);
    void setTextureOrigin(const Vec2& origin);
    void setTextureScale(float x, float y);
    void setTextureGraphRect(const Vec2& bottom_left, const Vec2& top_right);
    Sprite* getTexture();
    float translateXValueToXPixel(float value);
    float translateYValueToYPixel(float value);
private:
    void drawLines();
    
    int _padding_h;
    int _padding_v;
    
    Axis* _x_axis;
    Axis* _y_axis;
    
    TTFConfig   _ttf_config;
    bool        _draw_texture;
    bool        _draw_help;
    THDrawSmooth* _smooth_drawer;
    Sprite*     _sprite;

    GraphRect   _graph_rect;
    
    std::vector<GraphLine*> _lines;
};
