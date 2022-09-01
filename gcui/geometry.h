#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <math.h>

#ifndef PI
#define PI                      3.141592654
#endif

#define pow2(_x)                ((_x)*(_x))
#define deg2rad(_x)             ((_x)*PI)/180.0

#define point2d(_x, _y, _v)     {_v[0] = (float)(_x); _v[1] = (float)(_y); _v[2] = 1.0f;}
#define vector2d(_x, _y, _v)    {_v[0] = (float)(_x); _v[1] = (float)(_y); _v[2] = 0.0f;}

typedef float matrix2d_t[3 * 3];
typedef float vertex2d_t[3];

static void
mxm(matrix2d_t m, matrix2d_t c, matrix2d_t r)
{
    unsigned char i = 0;

    for(i = 0; i < 3; i++)
    {
        r[0 + i * 3] =   
            m[0]        * c[0 + i * 3] + 
            m[0 + 3]    * c[1 + i * 3] + 
            m[0 + 6]    * c[2 + i * 3];

        r[1 + i * 3] =   
            m[1]        * c[0 + i * 3] + 
            m[1 + 3]    * c[1 + i * 3] + 
            m[1 + 6]    * c[2 + i * 3];

        r[2 + i * 3] =   
            m[2]        * c[0 + i * 3] + 
            m[2 + 3]    * c[1 + i * 3] + 
            m[2 + 6]    * c[2 + i * 3];
    }
}

static inline float
vertex_magnitude(vertex2d_t v)
{
    return sqrtf(pow2(v[0]) + pow2(v[1]) + pow2(v[2]));
}

static void
normalize_vertex(vertex2d_t v, vertex2d_t r)
{
    float m = 0.0;

    m = vertex_magnitude(v);
    
    r[0] = v[0]/m;
    r[1] = v[1]/m;
    r[2] = v[2]/m;
}

static void
make_translation_matrix(float x, float y, matrix2d_t r)
{
    r[0] = 1.0f;
    r[0 + 3] = 0.0f;
    r[0 + 6] = 0.0f;

    r[1] = 0.0f;
    r[1 + 3] = 1.0f;
    r[1 + 6] = 0.0f;

    r[2] = x;
    r[2 + 3] = y;
    r[2 + 6] = 1.0f;
}

static void
make_scale_matrix(float w, float h, matrix2d_t r)
{
    r[0] = w;
    r[0 + 3] = 0.0f;
    r[0 + 6] = 0.0f;

    r[1] = 0.0f;
    r[1 + 3] = h;
    r[1 + 6] = 0.0f;

    r[2] = 0.0f;
    r[2 + 3] = 0.0f;
    r[2 + 6] = 1.0f;
}

static void
make_rotation_matrix(float t, matrix2d_t r)
{
    t = deg2rad(t);

    r[0] = cosf(t);
    r[0 + 3] = sinf(t);
    r[0 + 6] = 0.0f;

    r[1] = -sinf(t);
    r[1 + 3] = cos(t);
    r[1 + 6] = 0.0f;

    r[2] = 0.0f;
    r[2 + 3] = 0.0f;
    r[2 + 6] = 1.0f;
}

static void
transform_vertex(vertex2d_t v, matrix2d_t m, vertex2d_t r)
{
    r[0] = 
        m[0] *  v[0] + 
        m[1] *  v[1] + 
        m[2] *  v[2];

    r[1] = 
        m[3] *  v[0] + 
        m[4] *  v[1] + 
        m[5] *  v[2];

    r[2] = 
        m[6] *  v[0] + 
        m[7] *  v[1] + 
        m[8] *  v[2];
}

static inline void
translate_vertex(vertex2d_t v, float x, float y, vertex2d_t r)
{
    matrix2d_t m = {0.0f};

    make_translation_matrix(x, y, m);
    transform_vertex(v, m, r);
}

static inline void
rotate_vertex(vertex2d_t v, float t, vertex2d_t r)
{
    matrix2d_t m = {0.0f};

    make_rotation_matrix(t, m);
    transform_vertex(v, m, r);
}

static inline void
scale_vertex(vertex2d_t v, float w, float h, vertex2d_t r)
{
    matrix2d_t m = {0.0f};

    make_scale_matrix(w, h, m);
    transform_vertex(v, m, r);
}

#endif