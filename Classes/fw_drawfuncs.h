#pragma once

#include <math.h>
#include "cocos2d.h"
#include <cmath>
#include "2d/CCDrawNode.h"

#define degrees_to_radians(angle_degrees) (angle_degrees * M_PI / 180.00000f)
#define radians_to_degrees(angle_radians) (angle_radians * 180.00000f / M_PI)

#define FLOAT_EPSILON 1e-2

using namespace cocos2d;

namespace Graf {
    static Vec2 v2fzero(0.0f,0.0f);
    
    static inline Vec2 v2f(float x, float y)
    {
        Vec2 ret(x, y);
        return ret;
    }
    
    static inline Vec2 v2fadd(const Vec2 &v0, const Vec2 &v1)
    {
        return v2f(v0.x+v1.x, v0.y+v1.y);
    }
    
    static inline Vec2 v2fsub(const Vec2 &v0, const Vec2 &v1)
    {
        return v2f(v0.x-v1.x, v0.y-v1.y);
    }
    
    static inline Vec2 v2fmult(const Vec2 &v, float s)
    {
        return v2f(v.x * s, v.y * s);
    }
    
    static inline Vec2 v2fperp(const Vec2 &p0)
    {
        return v2f(-p0.y, p0.x);
    }
    
    static inline Vec2 v2fneg(const Vec2 &p0)
    {
        return v2f(-p0.x, - p0.y);
    }
    
    static inline float v2fdot(const Vec2 &p0, const Vec2 &p1)
    {
        return  p0.x * p1.x + p0.y * p1.y;
    }
    
    static inline Vec2 v2fforangle(float _a_)
    {
        return v2f(cosf(_a_), sinf(_a_));
    }
    
    static inline Vec2 v2fnormalize(const Vec2 &p)
    {
        Vec2 r(p.x, p.y);
        r.normalize();
        return v2f(r.x, r.y);
    }
    
    static inline Vec2 __v2f(const Vec2 &v)
    {
        //#ifdef __LP64__
        return v2f(v.x, v.y);
        // #else
        //     return * ((Vec2*) &v);
        // #endif
    }
    
    static inline Tex2F __t(const Vec2 &v)
    {
        return *(Tex2F*)&v;
    }
    
    
    static Vec2 add_sub(Vec2 v1, Vec2 v2, Vec2 table) {
        Vec2 result;
        
        result.x = (table.x < 0 ? v1.x - v2.x : v1.x + v2.x);
        result.y = (table.y < 0 ? v1.y - v2.y : v1.y + v2.y);
        
        return result;
    }
    
    static Vec2 dia_vec(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3) {
        Vec2 r(1,1);
        
        if(p0.x < p2.x) {
            r.x *= -1;
        }
        if (p0.y < p2.y) {
            r.y *= -1;
        }
        
        return r;
    }
    
    static float lerp(float a, float b, float alpha) {
        return a * (1.f - alpha) + b * alpha;
    };

    
    
    static bool are_same(float a, float b) {
        return fabsf(a - b) < FLOAT_EPSILON;
    }

    static bool axis_same(const Vec2& a, const Vec2& b) {
        return are_same(a.x, b.x) || are_same(a.y, b.y);
    }
    
    static bool are_same(const Vec2& a, const Vec2& b) {
        return are_same(a.x, b.x) && are_same(a.y, b.y);
    }

    
    static float find_angle(const Vec2& p0, const Vec2& p1, const Vec2& c) {
        float p0c = sqrtf(powf(c.x-p0.x,2)+
                          powf(c.y-p0.y,2));
        
        float p1c = sqrtf(powf(c.x-p1.x,2)+
                          powf(c.y-p1.y,2));
        
        float p0p1 = sqrtf(powf(p1.x-p0.x,2)+
                           powf(p1.y-p0.y,2));
        
        return acos((p1c * p1c + p0c * p0c - p0p1 * p0p1) / (2 * p1c * p0c));
    }
    
    static float vec_distancef(const Vec2& p0, const Vec2& p1) {
        return sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
    }
    
    static Vec2 vec_neg0_add(const Vec2& org, const Vec2& add) {
        
        return Vec2(
                    std::signbit(add.x) == 0 ? org.x + add.x : org.x - add.x,
                    std::signbit(add.y) == 0 ? org.y + add.y : org.y - add.y);
    }
    
    static float CalcTheta( const Vec2 Point1, const Vec2 Point2 )
    {
        float Theta;
        if ( Point2.x - Point1.x == 0 )
            if ( Point2.y > Point1.y )
                Theta = 0;
            else
                Theta = static_cast<float>( M_PI );
            else
            {
                Theta = std::atan( (Point2.y - Point1.y) / (Point2.x - Point1.x) );
                if ( Point2.x > Point1.x )
                    Theta = static_cast<float>( M_PI ) / 2.0f - Theta;
                else
                    Theta = static_cast<float>( M_PI ) * 1.5f - Theta;
            };
        return Theta;
    }
    
    static bool linearly_dependent(const Vec2 a, const Vec2 b) {
        return a.x*b.y - a.y*b.x < 1e-4f;
    }
    
    template <typename T>
    T clamp(T min, T val, T max) {
        if (val < min) return min;
        else if (val > max) return max;
        else return val;
    }
    
    static Vec2 nlerp(const Vec2 a, const Vec2 b, float t) {
        
        return v2fnormalize(a.lerp(b, t));
    }
    
    // a and b should be unit vectors
    // t is a value in the range [0, 1]
    // when t = 0, slerp returns a
    // when t = 1, slerp returns b
    // any value between 1 and 0 will be
    // vector between a and b
    static Vec2 slerp(Vec2 vectorA, Vec2 vectorB, float t) {
        float cosAngle = vectorA.dot(vectorB);
        
        float angle = acos(cosAngle);
        
        Vec2 v = v2fadd(
                        v2fmult(vectorA, sinf((1.0f - t) * angle)),
                        v2fmult(vectorB, sinf(t * angle)));
        
        return Vec2(v.x / sinf(angle), v.y / sinf(angle));
    }

    static Vec2 pos_slerp(Vec2 a, Vec2 b, Vec2 c, float r, float t) {
        Vec2 d_a = v2fsub(a, c);
        Vec2 d_b = v2fsub(b, c);
        
        Vec2 ret_n = slerp(v2fnormalize(d_a), v2fnormalize(d_b), t);
        
        Vec2 final = Vec2((c.x) + (ret_n.x * r), (c.y) + (ret_n.y * r));
        
        return final;
    }
    
    static Vec2 rotate_v2f(Vec2 v, float radians) {
        float x = v.x * cosf(radians) - v.y * sinf(radians);
        float y = v.x * sinf(radians) + v.y * cosf(radians);
        
        return Vec2(x,y);
    }

};

