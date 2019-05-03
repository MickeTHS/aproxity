#include "LineGraph.h"
#include "THDrawSmooth.h"
#include "fw_drawfuncs.h"

LineGraph::LineGraph() {
    _x_axis = NULL;
    _y_axis = NULL;
    
    _draw_texture = false;
}


LineGraph::~LineGraph() {
    if (_x_axis != NULL)
        delete _x_axis;
    if (_y_axis != NULL)
        delete _y_axis;
}

bool LineGraph::init(const Color4B& background_color) {
    if ( !LayerColor::initWithColor(background_color) ) {
        return false;
    }

    _smooth_drawer = THDrawSmooth::create();
    _smooth_drawer->retain();

    addChild(_smooth_drawer);
    
    return true;
}


void LineGraph::setPaddingHorizontal(int pixels) {
    _padding_h = pixels;
}


void LineGraph::setPaddingVertical(int pixels) {
    _padding_v = pixels;
}

void LineGraph::setTextureGraphRect(const Vec2& bottom_left, const Vec2& top_right) {
    _graph_rect.bl = bottom_left;
    _graph_rect.tr = top_right;

}

void LineGraph::setTextureDraw(const std::string& filename) {
    //Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filename);
    
    _sprite = Sprite::create(filename.c_str());
    _sprite->retain();
    _sprite->setAnchorPoint(Vec2(0,0));

    _draw_texture = true;
}

void LineGraph::setTextureDrawHelp(bool draw_help) {
    _draw_help = draw_help;
}

void LineGraph::setTextureOrigin(const Vec2& origin) {
    _sprite->setPosition(origin);
}

void LineGraph::setTextureScale(float x, float y) {
    _sprite->setScale(x, y);
}

Sprite* LineGraph::getTexture() {
    return _sprite;
}


void LineGraph::setAxisPixelOrigin(const Vec2& origin) {
    CCASSERT(_x_axis && _y_axis, "must create axis before altering axis");
    _x_axis->_origin = origin;
    _y_axis->_origin = origin;
}

void LineGraph::setAxisPixelLength(GraphAxisDir dir, float length) {
    CCASSERT(_x_axis && _y_axis, "must create axis before altering axis");
    if (dir == GraphAxisDir::X_AXIS) {
        _x_axis->_axis_length = length;
    }
    else {
        _y_axis->_axis_length = length;
    }
}

float LineGraph::translateXValueToXPixel(float value) {
    float x = _x_axis->translateValueInRange(value);
    
    return Graf::lerp(_graph_rect.bl.x, _graph_rect.tr.x, x);
}

float LineGraph::translateYValueToYPixel(float value) {
    float y = _y_axis->translateValueInRange(value);
    
    return Graf::lerp(_graph_rect.bl.y, _graph_rect.tr.y, y);
}

void LineGraph::redraw() {
    removeAllChildren();
    make();
}

void LineGraph::make() {
    
    
    if (_draw_texture) {
        // with this option, we dont draw any lines dynamically
        
        
        //_sprite->setPosition(_position);
        //_sprite->setScale(0.75f, 0.75f);
        _sprite->setZOrder(0);
        addChild(_sprite);
    }
    
    _smooth_drawer->setZOrder(99);
    // draw the lines
    drawLines();
    
    // if were not interested in the help lines, we quit
    if (_draw_texture && !_draw_help)
        return;
    
    Size sprite_size = _sprite->getContentSize();
    Vec2 sprite_origin = _sprite->getPosition();
    
    Vec2 p0 = sprite_origin;
    Vec2 p1 = Vec2(sprite_origin.x,                     sprite_origin.y + sprite_size.height);
    Vec2 p2 = Vec2(sprite_origin.x + sprite_size.width, sprite_origin.y + sprite_size.height);
    Vec2 p3 = Vec2(sprite_origin.x + sprite_size.width, sprite_origin.y);
    
    _smooth_drawer->drawLine(p0, p1, Color4B::BLUE, 1.0f);
    _smooth_drawer->drawLine(p1, p2, Color4B::BLUE, 1.0f);
    _smooth_drawer->drawLine(p2, p3, Color4B::BLUE, 1.0f);
    _smooth_drawer->drawLine(p3, p0, Color4B::BLUE, 1.0f);
    
    
    p0 = _graph_rect.bl;
    p1 = Vec2(_graph_rect.bl.x, _graph_rect.tr.y);
    p2 = _graph_rect.tr;
    p3 = Vec2(_graph_rect.tr.x, _graph_rect.bl.y);

    _smooth_drawer->drawLine(p0, p1, Color4B::RED, 1.0f);
    _smooth_drawer->drawLine(p1, p2, Color4B::RED, 1.0f);
    _smooth_drawer->drawLine(p2, p3, Color4B::RED, 1.0f);
    _smooth_drawer->drawLine(p3, p0, Color4B::RED, 1.0f);

    
    _smooth_drawer->drawLine(_x_axis->_origin,
                             Vec2(_x_axis->_origin.x + _x_axis->_axis_length, _x_axis->_origin.y),
                             Color4B::WHITE,
                             1.0f);
    
    _smooth_drawer->drawLine(_y_axis->_origin,
                             Vec2(_y_axis->_origin.x, _y_axis->_origin.y + _y_axis->_axis_length),
                             Color4B::WHITE,
                             1.0f);
    
    //addChild(_smooth_drawer);

}

void LineGraph::drawLines() {
    
    for (int i = 0; i < _lines.size(); ++i) {
        std::vector<Vec2> points;
        
        GraphLine* l = _lines[i];
        
        Vec2 p;
        
        for (int pn = 0; pn < l->_points.size(); ++pn) {
            p = Vec2(
                     translateXValueToXPixel(l->_points[pn]->getXFloat()),
                     translateYValueToYPixel(l->_points[pn]->getYFloat())
            );
            
            points.push_back(p);
        }
        
        _smooth_drawer->drawLinePath(points, l->_line_width, l->_line_color, true);
    }
}

void LineGraph::addLine(GraphLine* line) {
    _lines.push_back(line);
}


void LineGraph::setTTFConfig(TTFConfig config) {
    _ttf_config = config;
}
