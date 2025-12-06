#pragma once
#include "math.h"

typedef struct
{
    unsigned char* texture;
    int            width;
    int            height;
}texture_t;

typedef enum
{
    WIREFRAME,
    FILLED
}triangle_draw_mode_t;

typedef enum
{
    DIFFUSE_COLOR,
    DEPTH_COLOR,
    UVS_COLOR,
    NORMALS_COLOR,
    SPECULAR_COLOR,
    ALBEDO_COLOR,
    SHADED_COLOR
}render_pass_view_t;

typedef struct
{
    float r, g, b, a;
} color_t;


typedef struct
{
    vector4_t     position;
    color_t       color;
    float         intensity;
}light_t;

typedef struct
{
    int* pixels;
    int   width;
    int   height;
}frame_buffer_t;

typedef struct
{
    float* depth_pixels;
    int    width;
    int    height;
}depth_buffer_t;

typedef struct
{
    frame_buffer_t     frame_buffer;
    depth_buffer_t     depth_buffer;
    render_pass_view_t pass_display;
    light_t            light_sources[100];  // max light source predetermined
    int                light_count;
}renderer_t;

typedef struct
{
    mesh_t* meshes;
    texture_t   model_texture;
}model_t;
