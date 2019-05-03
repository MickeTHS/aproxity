#pragma once

#include <vector>
#include <string>

#include "GraphAxisScale.h"
#include "AxisSort.h"

using namespace cocos2d;

class Axis {
public:
    Axis() {}
    virtual ~Axis() {}
    
    virtual std::string getName() = 0;
    virtual int         getNumLabels() = 0;
    virtual std::string getLabelTextAt(int index) = 0;
    virtual void        sort() = 0;
    virtual void        sort(bool (*func)(const void*, const void*)) = 0;
    virtual void        setCustomSort(bool (*func)(const void*, const void*)) = 0;
    virtual float       translateValueInRange(float value) = 0;
//    virtual void        setAxisLength(float) = 0;
//    virtual void        setAxisOrigin(const Vec2&) = 0;
    
    
    float _axis_length;
    Vec2 _origin;
};


template<class T>
class GraphAxis : public Axis {
public:
    GraphAxis() {}
    GraphAxis(GraphAxisDir axis_dir, std::string axis_name, T val_lowest, T val_highest) {
        dir     = axis_dir;
        name    = axis_name;
        lowest  = val_lowest;
        highest = val_highest;
        _sort_func = NULL;
    }
    
    virtual ~GraphAxis() {
        labels.erase(labels.begin(), labels.end());
    }
    
    void addAxisLabel(GraphAxisScale<T>* label) {}
    
    virtual std::string getName() {
        return name;
    }
    
    virtual int         getNumLabels() { return (int)labels.size(); }
    
    virtual std::string getLabelTextAt(int index) { return ""; }
    
    void insertLabel(std::string label_str, T value) {
        GraphAxisScale<T>* label = new GraphAxisScale<T>(label_str, value);
        labels.push_back(label);
    }
    
    virtual void setCustomSort(bool (*func)(const void*, const void*)) {
        _sort_func = func;
    }
    
    virtual void sort() {
        if (_sort_func != NULL) {
            sort(_sort_func);
            return;
        }
        
        // no custom sort, default to a basic float sort
        std::sort( labels.begin(), labels.end(), SortFloatAsc() );
    }
    
    virtual void sort(bool (*func)(const void*, const void*)) {
        std::sort( labels.begin(), labels.end(), func );
    }

    
    virtual float translateValueInRange(float value) {
        return (lowest - value) / (lowest - highest);
    }

    
    GraphAxisDir dir;
    std::string name;
    T lowest;
    T highest;
    
    std::vector<GraphAxisScale<T>*> labels;
private:
    bool (*_sort_func)(const void*, const void*);
    float _axis_length;
    Vec2  _axis_origin;
};
