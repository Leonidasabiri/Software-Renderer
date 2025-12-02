#pragma once

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
    ALBEDO_COLOR
}render_pass_view_t;

typedef struct
{
    float r, g, b, a;
} color_t;

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
}renderer_t;

