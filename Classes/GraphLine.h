#pragma once

#include <vector>
#include "cocos2d.h"
#include "GraphPoint.h"

class GraphLine {
public:
    GraphLine() : _line_width(1.0f), _line_color(Color4B::WHITE), _z_order(0) { }
    ~GraphLine() {}
    
    void setLineColor(const Color4B& color) {
        _line_color = color;
    }
    
    void setLineWidth(float width) {
        _line_width = width;
    }
    
    void addPoint(float xval, float yval) {
        GraphPoint* p = new GraphPoint();
        p->setXValue(xval);
        p->setYValue(yval);
        
        _points.push_back(p);
    }
    
    
    void setZOrder(int z) {
        _z_order = z;
    }
    
    float    _line_width;
    Color4B  _line_color;
    int      _z_order;
    
    std::vector<GraphPoint*> _points;
};