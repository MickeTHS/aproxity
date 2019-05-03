#include "DrawSmooth.h"
#include <math.h>
#include "fw_drawfuncs.h"
#include <algorithm>
#include "THShader.h"

const char* DrawSmooth::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR_SMOOTH = "ShaderPositionColorLengthTextureSmooth";

DrawSmooth::DrawSmooth() : DrawNode() {}

DrawSmooth::~DrawSmooth() {
    
}

bool DrawSmooth::init() {
    CCLOG("DrawSmooth::init");
    
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

    auto program = THShader::createWithDefines(
        "ccPositionColorLengthTextureSmooth.vert",
        "ccPositionColorLengthTextureSmooth.frag",
        "",
        "#ifdef GL_ES\n#extension GL_OES_standard_derivatives : enable\n#endif\n");

    CCASSERT(program, "shader not created");
    
    GLProgramCache::getInstance()->addGLProgram(program, SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR_SMOOTH);
    
    setGLProgramState(
        GLProgramState::getOrCreateWithGLProgramName(DrawSmooth::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR_SMOOTH));
    
    ensureCapacity(1024);
    ensureCapacityGLPoint(64);
    ensureCapacityGLLine(256);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        glGenVertexArrays(1, &_vao);
        GL::bindVAO(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        glGenVertexArrays(1, &_vaoGLLine);
        GL::bindVAO(_vaoGLLine);
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        glGenVertexArrays(1, &_vaoGLPoint);
        GL::bindVAO(_vaoGLPoint);
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        // vertex
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // Texture coord as pointsize
        glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
        
        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }
    else
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLLine);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        
        glGenBuffers(1, &_vboGLPoint);
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    _dirty = true;
    _dirtyGLLine = true;
    _dirtyGLPoint = true;
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    // Need to listen the event only when not use batchnode, because it will use VBO
    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
        /** listen the event that renderer was recreated on Android/WP8 */
        this->init();
    });
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
    return true;
}

void DrawSmooth::drawCircle(const Vec2& center, float radius, const Color4B& color, float line_width, int segments) {
    Vec2 a(0,0);
    std::vector<Vec2> path;
    
    float deg_seg = 360.0f / (float)segments;
    
    for (int i = 0; i < segments+1; ++i) {
        float deg_in_rad = degrees_to_radians(i * deg_seg);
        
        a.x = cosf(deg_in_rad)*radius;
        a.y = sinf(deg_in_rad)*radius;
        
        path.push_back(center + a);
    }

    drawLinePath(path, line_width, color, false, true);
}

void DrawSmooth::drawFillCircle(const Vec2& center, float radius, const Color4B color, int segments) {
    Vec2 a(0,0), b(0,0);
    
    float deg_seg = 360.0f / (float)segments;
    
    for (int i = 0; i < segments+1; ++i) {
        float deg_in_rad = degrees_to_radians(i * deg_seg);
        
        a.x = cosf(deg_in_rad)*radius;
        a.y = sinf(deg_in_rad)*radius;
        
        drawFillTriangle(Vec2(250,0) + center + a, Vec2(250,0) + center + b, Vec2(250,0) + center, color);
        drawFillTriangleB(center + a, center + b, center, color);
        b = a;
    }
}

void DrawSmooth::drawFillTriangle(const Vec2& p0, const Vec2& p1, const Vec2& p2, const Color4B& color) {
    // using cocos2d draw, fix our own
    drawTriangle(p0,p1,p2,Color4F(color));
}

void DrawSmooth::drawFillTriangleB(const Vec2& a, const Vec2& b, const Vec2& c, const Color4B& color) {
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;
    
    V2F_C4B_T2F_Triangle tmp1 = {
        {a, color, Graf::__t(Vec2(1,0))},
        {b, color, Graf::__t(Vec2(1,0))},
        {c, color, Graf::__t(Vec2(0,0.5))}
    };
    *cursor++ = tmp1;
    
    _bufferCount += 3;
    _dirty = true;
}

void DrawSmooth::drawRect(const Vec2& start, const Vec2& end, const Color4B& color, float line_width) {
    std::vector<Vec2> path;
    path.push_back(start);
    path.push_back(Vec2(end.x, start.y));
    path.push_back(end);
    path.push_back(Vec2(start.x, end.y));
    path.push_back(start);
    
    drawLinePath(path, line_width, color, false);

}

void DrawSmooth::drawFillRect(const Vec2& start, const Vec2& end, const Color4B &color) {
    drawTriangle(start, end, Vec2(start.x, end.y), Color4F(color));
    drawTriangle(start, end, Vec2(end.x, start.y), Color4F(color));
}

void DrawSmooth::drawLine(const Vec2& start, const Vec2& end, const Color4B &color, float width) {
    std::vector<Vec2> path;
    path.push_back(start);
    path.push_back(end);
    
    drawLinePath(path, width, color, true);
    
    _dirty = true;
}

void DrawSmooth::drawLinePath(const std::vector<Vec2>& path, float width, const Color4B& color, bool round_ends, bool close_path) {
    CCASSERT(path.size() >= 2, "Must have 2 points at least");
    CCASSERT(width > 0, "line width must be larger than 0");
    
    int triangle_count = ((int)path.size()-1) * 2;
    int vertex_count = 3*triangle_count;
    
    //ensureCapacity(vertex_count);
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;
    
    struct Corner {int num; Vec2 l1_norm, l1_offset, l2_norm, l2_offset, a, b, c;};
    struct Line { Vec2 offset, normal; Vec2 a1, a2, b1, b2, a, b;};
    
    std::vector<Corner> corners;
    std::vector<Line> lines;
    
    Corner corner;
    corner.num = 0;
    
    // for each line segment
    for (int i = 0, v = 0; i < path.size()-1; ++i) {
        
        Vec2 a = path[v];
        Vec2 b = path[++v];
        
        Vec2 norm = Graf::v2fnormalize(Graf::v2fperp(Graf::v2fsub(b, a)));
        Vec2 offset = Graf::v2fmult(norm, width/2);
        
        Vec2 a1 = Graf::v2fadd(a, offset);
        Vec2 a2 = Graf::v2fsub(a, offset);
        
        Vec2 b1 = Graf::v2fadd(b, offset);
        Vec2 b2 = Graf::v2fsub(b, offset);
        
        Line line;
        line.offset = offset;
        line.normal = norm;
        line.a1 = a1;
        line.a2 = a2;
        line.b1 = b1;
        line.b2 = b2;
        line.a = a;
        line.b = b;
        
        lines.push_back(line);
        
        V2F_C4B_T2F_Triangle tmp1 = {
            {a1, Color4B(color), Graf::__t(norm)},
            {a2, Color4B(color), Graf::__t(Graf::v2fneg(norm))},
            {b2, Color4B(color), Graf::__t(Graf::v2fneg(norm))}
        };
        *cursor++ = tmp1;
        
        
        V2F_C4B_T2F_Triangle tmp2 = {
            {b1, Color4B(color), Graf::__t(norm)},
            {b2, Color4B(color), Graf::__t(Graf::v2fneg(norm))},
            {a1, Color4B(color), Graf::__t(norm)}
            
        };
        *cursor++ = tmp2;
        
        //CCLOG("LINE: %d, norm: [%f,%f]", i, norm.x, norm.y);
    }
    
    for (int i = 1; i < lines.size() + (close_path ? 1 : 0); ++i) {
        int current = i;
        
        if (i == lines.size() && close_path) {
            current = 0;
        }
        
        Corner c;
        c.l1_offset = lines[i-1].offset;
        c.l2_offset = lines[current].offset;
        
        c.l1_norm = lines[i-1].normal;
        c.l2_norm = lines[current].normal;
        
        c.c = lines[i-1].b;
        
        float angle = radians_to_degrees(atan2f(lines[i-1].normal.y, lines[i-1].normal.x));
        float angle2 = radians_to_degrees(atan2f(lines[current].normal.y, lines[current].normal.x));
        
        if (angle > angle2) {
            //make line between l1_b1 and l2_a1
            c.a = lines[i-1].b1;
            c.b = lines[current].a1;
            
            //this makes no sense! but it works!
            if (i == lines.size() && close_path) {
                c.a = lines[i-1].b2;
                c.b = lines[current].a2;
            }
        }
        else {
            c.a = lines[i-1].b2;
            c.b = lines[current].a2;
            
            if (i == lines.size() && close_path) {
                c.a = lines[i-1].b1;
                c.b = lines[current].a1;
            }
        }
        
        corners.push_back(c);
        
    }
    
    _bufferCount += vertex_count;
    
    float tri_dist  = 1.3f;
    
    for (int co = 0; co < corners.size(); ++co) {
        cornerDraw(corners[co].a, corners[co].b, corners[co].c,
                   color, width / 2.0f, tri_dist, true);
        // do the corner/edge rounding
    }
    
    _dirty = true;
    
    // ------- BEGIN AND END PATH ROUND EDGES --------- //
    if (!round_ends)
        return;
    
    
    //pull the center point out in the normals direction rotated counter-clockwise
    Vec2 n = Graf::rotate_v2f(lines[0].normal, degrees_to_radians(90));
    Vec2 a = lines[0].a + Graf::v2fmult(n, width / 2.0f);
    
    // must be two halfcircles
    cornerDraw(a, lines[0].a2, lines[0].a, color, width / 2.0f, tri_dist, false);
    cornerDraw(a, lines[0].a1, lines[0].a, color, width / 2.0f, tri_dist, false);
    
    //pull the center point out in the normals direction rotated clockwise
    n = Graf::rotate_v2f(lines[lines.size()-1].normal, degrees_to_radians(-90));
    a = lines[lines.size()-1].b + Graf::v2fmult(n, width / 2.0f);
    // must be two halfcircles
    cornerDraw(a, lines[lines.size()-1].b1, lines[lines.size()-1].b, color, width / 2.0f, tri_dist, false);
    cornerDraw(a, lines[lines.size()-1].b2, lines[lines.size()-1].b, color, width / 2.0f, tri_dist, false);

    
    if (!close_path)
        return;
    
}

void DrawSmooth::drawLinePath(const std::vector<Vec2>& path, float width, const Color4B& color, bool round_ends) {
    drawLinePath(path, width, color, round_ends, false);
}

void DrawSmooth::cornerDraw(const Vec2& a, const Vec2& b, const Vec2& c, const Color4B& color,
                            float radius, float tri_dist, bool force_one) {
    
    V2F_C4B_T2F_Triangle* cursor = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    
    float distance  = Graf::vec_distancef(a, b);
    
    int num_triangles = (int)floor((distance / tri_dist));
    
    //if the distance is too small, we just skip this
    if (num_triangles == 0) {
        if (force_one) {
            num_triangles = 1;
        }
        else {
            return;
        }
    }
    
    float inc = 1.0f / (float)num_triangles;
    float t = 0.0f;
    
    Vec2 p0 = a;
    Vec2 p1 = b;
    Vec2 pc = c;
    
    CCASSERT(!Graf::are_same(a,b), "identical triangle vertices");

    for (int i = 0; i < num_triangles; ++i) {
        //ensureCapacity(3);
        
        t+= inc;
        
        p1 = Graf::pos_slerp(a, b, c, radius, t);
        
        CCASSERT(!Graf::are_same(p1, p0) || !Graf::are_same(p1, pc) || !Graf::are_same(p0, pc), "identical triangle vertices");
        
        V2F_C4B_T2F_Triangle tmp = {
            {p0, Color4B(color), Graf::__t(Vec2(1,0))},
            {p1, Color4B(color), Graf::__t(Vec2(1,0))},
            {pc, Color4B(color), Graf::__t(Vec2(0,0.5))}
        };
        *cursor++ = tmp;
        _bufferCount += 3;
        
        p0 = p1;
    }
    
    CCASSERT(t == 1.0f, "theres a gap somewhere!");
}

/*
void DrawSmooth::drawPolygon(const Vec2 *verts, int count, const Color4B &fillColor, float borderWidth, const Color4B &borderColor)
{
    CCASSERT(count >= 0, "invalid count value");
    
    auto triangle_count = 2;
    auto vertex_count = 3*triangle_count;
    
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;

    int offset = 20;
    
    Vec2 v1(100+offset,100);
    Vec2 v2(50,150+offset);
    Vec2 v3(100-offset,200);
    Vec2 v4(150,150-offset);
    
    V2F_C4B_T2F_Triangle tmp2 = {
        {v1, Color4B(borderColor), __t(v2fneg(v2fnormalize(v2fperp(v2fsub(v2, v1)))))},
        {v2, Color4B(borderColor), __t(v2fnormalize(v2fperp(v2fsub(v2, v1))))},
        {v3, Color4B(borderColor), __t(v2fnormalize(v2fperp(v2fsub(v2, v1))))}
    };
    *cursor++ = tmp2;

    V2F_C4B_T2F_Triangle tmp3 = {
        {v1, Color4B(borderColor), __t(v2fneg(v2fnormalize(v2fperp(v2fsub(v4, v1)))))},
        {v4, Color4B(borderColor), __t(v2fneg(v2fnormalize(v2fperp(v2fsub(v4, v1)))))},
        {v3, Color4B(borderColor), __t(v2fnormalize(v2fperp(v2fsub(v4, v1))))}
    };
    *cursor++ = tmp3;

    
    _bufferCount += vertex_count;
    
    _dirty = true;
    
    CCLOG("drawPolygon: done");
}*/

void DrawSmooth::drawCommand(const DrawCommand& command) {
    switch(command.type) {
        case DCT_NOTHING:
            return;
        default:
            break;
    }
}

void DrawSmooth::drawCommandQueue(const std::vector<DrawCommand>& command) {
    for (int i = 0; i < command.size(); ++i) {
        drawCommand(command[i]);
    }
}


void DrawSmooth::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if(_bufferCount)
    {
        _customCommand.init(_globalZOrder, transform, flags);
        _customCommand.func = CC_CALLBACK_0(DrawSmooth::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
    
    if(_bufferCountGLPoint)
    {
        _customCommandGLPoint.init(_globalZOrder, transform, flags);
        _customCommandGLPoint.func = CC_CALLBACK_0(DrawSmooth::onDrawGLPoint, this, transform, flags);
        renderer->addCommand(&_customCommandGLPoint);
    }
    
    if(_bufferCountGLLine)
    {
        _customCommandGLLine.init(_globalZOrder, transform, flags);
        _customCommandGLLine.func = CC_CALLBACK_0(DrawSmooth::onDrawGLLine, this, transform, flags);
        renderer->addCommand(&_customCommandGLLine);
    }
}

void DrawSmooth::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto glProgram = getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        _dirty = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
    CHECK_GL_ERROR_DEBUG();
}

void DrawSmooth::onDrawGLLine(const Mat4 &transform, uint32_t flags)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(DrawSmooth::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR_SMOOTH);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirtyGLLine)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
        _dirtyGLLine = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLLine);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    glLineWidth(_lineWidth);
    glDrawArrays(GL_LINES, 0, _bufferCountGLLine);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLLine);
    CHECK_GL_ERROR_DEBUG();
}

void DrawSmooth::onDrawGLPoint(const Mat4 &transform, uint32_t flags)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR_TEXASPOINTSIZE);
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirtyGLPoint)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
        
        _dirtyGLPoint = false;
    }
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vaoGLPoint);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_POINTS, 0, _bufferCountGLPoint);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLPoint);
    CHECK_GL_ERROR_DEBUG();
}
