#pragma once

#include "cocos2d.h"

typedef std::function<void()>    callbackFunc;
typedef std::function<void(int)> callbackFuncInt;


#define TH_DEVICE_ORIENTATION_LANDSCAPE 1
#define TH_DEVICE_ORIENTATION_PORTRAIT 2

class THGlobal {
public:
    static int getDeviceOrientation() {
        auto size = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
        
        if(size.width > size.height) {
            return TH_DEVICE_ORIENTATION_LANDSCAPE;
        }
        
        return TH_DEVICE_ORIENTATION_PORTRAIT;
    }
    
    static void swap(float& x, float& y) {
        float t = y;
        y = x;
        x = t;
    }
};

