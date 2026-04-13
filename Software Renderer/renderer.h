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
    SHADED_COLOR,
    WORLD_POS_COLOR,
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
    float              far_plane, near_plane;
    float              r, t;
    float              fov;
    float              current_z_value;
    light_t            light_sources[100];  // max light source predetermined
    int                light_count;
    vector4_t          view;
    vector2_t          current_screen_pos;
    vector2_t          current_uvs;
}renderer_t;

typedef struct
{
    mesh_t* meshes;
    texture_t   model_texture;
}model_t;

vector4_t screen_to_world_pos(renderer_t software_renderer, vector4_t screen_pos)
{
    // screen to ndc space
    screen_pos.x =  screen_pos.x * 2/software_renderer.frame_buffer.width - 1;
    screen_pos.y = -screen_pos.y * 2/software_renderer.frame_buffer.height + 1;

    // ndc space to clip space
    vector4_t world_pos;
    world_pos.x = screen_pos.x * screen_pos.w;
    world_pos.y = screen_pos.y * screen_pos.w;
    world_pos.z = screen_pos.w;
    world_pos.w = 1;

    // clip space space to view space
    world_pos.x = world_pos.x * (software_renderer.r/software_renderer.near_plane);
    world_pos.y = world_pos.y * (software_renderer.t/software_renderer.near_plane);

    // view space to world space

    return world_pos;
}

