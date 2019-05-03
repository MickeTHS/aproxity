#pragma once

class GraphPoint {
public:
    GraphPoint() {}
    virtual ~GraphPoint() {}
    
    void setXValue(float value) {
        x_val = value;
    }
    
    void setYValue(float value) {
        y_val = value;
    }
    
    float getXFloat() {
        return x_val;
    }
    
    float getYFloat() {
        return y_val;
    }
    
    float x_val;
    float y_val;
};