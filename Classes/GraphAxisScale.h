#pragma once

#include <string>
#include "cocos2d.h"

using namespace cocos2d;

enum GraphAxisDir {
    X_AXIS = 1,
    Y_AXIS = 2
};


template<class T>
class GraphAxisScale {
public:
    GraphAxisScale(std::string label_string, T val) {
        name = label_string;
        value = val;
    }
    
    
    GraphAxisScale() {
        _line_color = Color4B::BLACK;
        _text_color = Color4B::BLACK;
        _background_color = Color4B::WHITE;
    }
    
    
    virtual ~GraphAxisScale() {}
    
    
    void setLineOutsideLength(int length) {
        _line_outside_length = length;
    }
    
    
    void setLineOutsideWidth(int width) {
        _line_outside_width = width;
    }
    
    
    void setLineColor(Color4B color) {
        _line_color = color;
    }
    
    
    void setBackgroundColor(Color4B color) {
        _background_color = color;
    }
    
    
    void setTextColor(Color4B color) {
        _text_color = color;
    }
    
    
    void setTextMargin(int margin) {
        _text_margin = margin;
    }
    
    
    std::string name;
    T           value;
    
private:
    int _text_margin;
    
    int _line_outside_width;
    int _line_outside_length;
    
    int _line_inside_width;
    int _line_inside_length;
    
    Color4B _background_color;
    Color4B _text_color;
    Color4B _line_color;
    
    
    Label   _text_label;
};