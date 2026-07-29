#include "renderer.h"
#include "math.h"
#include <math.h>
#include <stdio.h>

void draw_pixel(int x, int y, int* buffer, int width, int height, color_t color)
{
    if (x < 0 || y < 0 || x > width - 1 || y > height)
        return;
    int pos = (x + y * width);
    buffer[pos] = (int)color.r << 24 | (int)color.g << 16 | (int)color.b << 8 | (int)color.a;
}

color_t fetch_pixel(unsigned char *surface, int x, int y, int width, int height)
{

    if (x < 0 || y < 0 || y > height - 1 || x > width - 1) return { 0,0,0,255 };

    unsigned char r = surface[(x + y * width) * 4 + 0];
    unsigned char g = surface[(x + y * width) * 4 + 1];
    unsigned char b = surface[(x + y * width) * 4 + 2];

    return {
            (float)r,
            (float)g,
            (float)b,
            255
    };
}

void pixel_shading(renderer_t software_renderer, 
					vector4_t pixel_world_pos, 
					vector4_t normal,
					texture_t texture)
{
	float uv_x = software_renderer.current_uvs.x;
	float uv_y = software_renderer.current_uvs.y;

	float z_interpolate = software_renderer.current_z_value;
	float x = software_renderer.current_screen_pos.x;
	float y = software_renderer.current_screen_pos.y;

	int width = software_renderer.frame_buffer.width;
	int height = software_renderer.frame_buffer.height;

    color_t final_colort = fetch_pixel(texture.texture, uv_x, uv_y, texture.width, texture.height);
    color_t color_world_pos;
    color_t normal_color;

    final_colort.r *= 0.5;
    final_colort.g *= 0.5;
    final_colort.b *= 0.5;
    final_colort.a = 255.0;

    vector4_t light = {0.0, 0.0, 1000.0, 0.0};
    vector4_t view = {0.0, 0.0, -3.0, 0.0};
    vector4_t light_dir = normalize_vector(sub_vector4(light, pixel_world_pos));

    float col = dot_product(normal, normalize_vector(sub_vector4(light, {x, y, z_interpolate, 1}))) * 80.0;
    float col_s = powf(dot_product(reflection(light_dir, normal), view) * 0.6, 12) * 0.2;

    final_colort.r = final_colort.r + col > 255 ? 255 : final_colort.r + col < 0 ? 0 : final_colort.r + col;
    final_colort.g = final_colort.g + col > 255 ? 255 : final_colort.g + col < 0 ? 0 : final_colort.g + col;
    final_colort.b = final_colort.b + col > 255 ? 255 : final_colort.b + col < 0 ? 0 : final_colort.b + col;

    // final_colort.r = final_colort.r + col_s > 255 ? 255 : final_colort.r + col_s < 0 ? 0 : final_colort.r + col_s;
    // final_colort.g = final_colort.g + col_s > 255 ? 255 : final_colort.g + col_s < 0 ? 0 : final_colort.g + col_s;
    // final_colort.b = final_colort.b + col_s > 255 ? 255 : final_colort.b + col_s < 0 ? 0 : final_colort.b + col_s;

    color_world_pos.r = pixel_world_pos.x * 255;
    color_world_pos.g = pixel_world_pos.y * 255;
    color_world_pos.b = pixel_world_pos.z * 255;
    color_world_pos.a = 255;

    normal_color.r = 0;
    normal_color.g = 0;
    normal_color.b = 0;
    normal_color.a = 255;

    if (normal.x >= 0)
        normal_color.r = normal.x * 255;
    if (normal.y >= 0)
        normal_color.g = normal.y * 255;
    if (normal.z >= 0)
        normal_color.b = normal.z * 255;

    int index = (int)((int)y*software_renderer.frame_buffer.width + x);

    if (index >= 0 && index < software_renderer.frame_buffer.width * software_renderer.frame_buffer.height && z_interpolate <= software_renderer.depth_buffer.depth_pixels[index])
    {
        software_renderer.depth_buffer.depth_pixels[index] = z_interpolate;

        switch (software_renderer.pass_display)
        {
            case DIFFUSE_COLOR:
                draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, final_colort);
                break;
            case DEPTH_COLOR:
                draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, { z_interpolate, z_interpolate, z_interpolate, 255 });
                break;
            case NORMALS_COLOR:
                draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, normal_color);
                break;
            case WORLD_POS_COLOR:
                draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, color_world_pos);
                break;
            default:
                break;
        }
    }
}
