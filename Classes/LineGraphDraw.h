#pragma once

#include "cocos2d.h"
#include "fw_drawfunc.h"

enum GraphCornerTypes {
    GCT_NOTHING = 0,
    GCT_BOX_NOBORDER,
    GCT_BOX_BORDER,
    GCT_DOT_NOBORDER,
    GCT_DOT_BORDER
};



struct LineGraphCorner {
    GraphCornerTypes type;
    
    std::vector<DrawCommand> draw_queue;
};