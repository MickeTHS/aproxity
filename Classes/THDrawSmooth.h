#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include <vector>

using namespace cocos2d;

enum DrawCommandType {
    DCT_NOTHING = 0,
    DCT_RECT,
    DCT_FILLRECT,
    DCT_CICLE,
    DCT_FILLCIRCLE
};

struct DrawCommand {
    DrawCommandType type;
    Vec2            center;
    Color4B         color;
};

class THDrawSmooth : public DrawNode {
public:
    static const char* SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR_SMOOTH;
    
    THDrawSmooth();
    virtual ~THDrawSmooth();
    
    CREATE_FUNC(THDrawSmooth);
    
    //void drawPolygon(const Vec2 *verts, int count, const Color4B &fillColor, float borderWidth, const Color4B &borderColor);
    
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawGLLine(const Mat4 &transform, uint32_t flags);
    void onDrawGLPoint(const Mat4 &transform, uint32_t flags);
    
    virtual bool init() override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    void drawCircle(const Vec2& center, float radius, const Color4B& color, float line_width, int segments);
    void drawFillCircle(const Vec2& center, float radius, const Color4B color, int segments);
    
    void drawRect(const Vec2& start, const Vec2& end, const Color4B& color, float line_width);
    void drawFillRect(const Vec2& start, const Vec2& end, const Color4B &color);
    
    void drawLine(const Vec2& start, const Vec2& end, const Color4B &color, float width);
    void drawLinePath(const std::vector<Vec2>& path, float width, const Color4B& color, bool round_ends);
    void drawLinePath(const std::vector<Vec2>& path, float width, const Color4B& color, bool round_ends, bool close_path);
    
    void drawFillTriangle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Color4B& color);
    void drawFillTriangleB(const Vec2& a, const Vec2& b, const Vec2& c, const Color4B& color);
    
    void drawCommand(const DrawCommand& command);
    void drawCommandQueue(const std::vector<DrawCommand>& command);
private:
    void cornerDraw(const Vec2& a, const Vec2& b, const Vec2& c, const Color4B& color, float radius, float tri_dist, bool force_one);
};
