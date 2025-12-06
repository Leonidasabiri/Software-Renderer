#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include "model_parser.c"
#include "stb_image.h"
#include "renderer.h"
#include "math.h"

vector2_t convert_to_screen_space(vector4_t pos, int width, int height)
{
    vector2_t screen_space;

    screen_space.x =  (pos.x + 1) * width/2;
    screen_space.y = (-pos.y + 1) * height/2;

    return screen_space;
}

void draw_pixel(int x, int y, int* buffer, int width, int height, color_t color)
{
    if (x < 0 || y < 0 || x > width || y > height)
        return;
    int pos = (x + y * width);
    buffer[pos] = (int)color.r << 24 | (int)color.g << 16 | (int)color.b << 8 | (int)color.a;
}

void drawLine(vector2_t point1, vector2_t point2, int* buffer, int width, int height, color_t color)
{
    int dx = point2.x - point1.x;
    int dy = point2.y - point1.y;

    float startx = point1.x, endx = point2.x;
    float starty = point1.y, endy = point2.y;

    if (abs(dx) > abs(dy))
    {
        if (point1.x > point2.x)
        {
            startx = point2.x, endx = point1.x;
            starty = point2.y, endy = point1.y;
        }
        float d = 0;

        if (-(startx - endx) != 0)
            d = -(starty - endy) / -(startx - endx);
        float yy = starty;
        for (int x = startx; x < endx; x++)
        {
            draw_pixel(x, yy, buffer, width, height, color);
            yy += d;
        }
    }
    else
    {
        if (point1.y > point2.y)
        {
            startx = point2.x, endx = point1.x;
            starty = point2.y, endy = point1.y;
        }
        float d = 0;
        if (-(startx - endx) != 0)
            d = -(startx - endx) / -(starty - endy);

        float xx = startx;
        for (int y = starty; y < endy; y++)
        {
            draw_pixel(xx, y, buffer, width, height, color);
            xx += d;
        }
    }
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

void pixel_shading(renderer_t software_renderer, vector4_t pixel_world_pos)
{
    // do any kind of shading for any pixel here
}

void drawTriangle(mesh_t triangle, renderer_t software_renderer, triangle_draw_mode_t triangle_mode, texture_t model_texture, vector4_t normal)
{
    color_t color[3] = {255, 255, 255, 255};

    vector4_t vertex1 = { triangle.vertecies[0], triangle.vertecies[1], triangle.vertecies[2], triangle.vertecies[3] };
    vector4_t vertex2 = { triangle.vertecies[4], triangle.vertecies[5], triangle.vertecies[6], triangle.vertecies[7] };
    vector4_t vertex3 = { triangle.vertecies[8], triangle.vertecies[9], triangle.vertecies[10], triangle.vertecies[11] };

    float z1 = vertex1.z, z2 = vertex2.z, z3 = vertex3.z;

    vector2_t point1 = convert_to_screen_space(vertex1, software_renderer.frame_buffer.width, software_renderer.frame_buffer.height);
    vector2_t point2 = convert_to_screen_space(vertex2, software_renderer.frame_buffer.width, software_renderer.frame_buffer.height);
    vector2_t point3 = convert_to_screen_space(vertex3, software_renderer.frame_buffer.width, software_renderer.frame_buffer.height);

    vector2_t points[3] = {point1, point2, point3};

    {
        if (point1.y <= point2.y && point1.y <= point3.y)
        {
            points[0] = point1;
            if (point2.y <= point3.y)
            {
                points[1] = point2;
                points[2] = point3;
            }
            else
            {
                points[1] = point3;
                points[2] = point2;
            }
        }
        if (point2.y <= point3.y && point2.y <= point1.y)
        {
            points[0] = point2;
            if (point1.y <= point3.y)
            {
                points[1] = point1;
                points[2] = point3;
            }
            else
            {
                points[1] = point3;
                points[2] = point1;
            }
        }
        if (point3.y <= point1.y && point3.y <= point2.y)
        {
            points[0] = point3;
            if (point2.y <= point1.y)
            {
                points[1] = point2;
                points[2] = point1;
            }
            else
            {
                points[1] = point1;
                points[2] = point2;
            }
        }
    }

    float xintersect = (points[0].x + (points[1].y - points[0].y)/(points[2].y - points[0].y) * (points[2].x - points[0].x));

    float startx = points[0].x, endx = points[0].x;

    float dx1 = points[0].x - points[1].x, dy1 = points[0].y - points[1].y;
    float dx2 = points[0].x - xintersect, dy2 = points[0].y - points[1].y;
    float d1 = dx1 / dy1;
    float d2 = dx2 / dy2;
    float full_surface = triangle_surface(point1, point3, point2);

    int width = software_renderer.frame_buffer.width;
    int height = software_renderer.frame_buffer.height;

    switch (triangle_mode)
    {
    case WIREFRAME:
        drawLine(point1, point2, software_renderer.frame_buffer.pixels, width, height, color[0]);
        drawLine(point2, point3, software_renderer.frame_buffer.pixels, width, height, color[0]);
        drawLine(point3, point1, software_renderer.frame_buffer.pixels, width, height, color[0]);
        break;
    case FILLED:
        for (float y = points[0].y; y <= points[1].y; y++)
        {
            int x = startx;
            if (startx > endx) x = endx;
            for (int i = 0; i <= fabs(startx - endx); i++)
            {
                vector2_t point = { x, y };
                float surface1 = triangle_surface(point, point3, point2)/full_surface;
                float surface2 = triangle_surface(point, point2, point1)/full_surface;
                float surface3 = triangle_surface(point, point1, point3)/full_surface;

                float z_interpolate = 1.0 / (surface1/z1 + surface3/z2 + surface2/z3);

                float uvx_interpolate = (surface1 * triangle.uvs[1] + surface2 * triangle.uvs[5] + surface3 * triangle.uvs[3]) * z_interpolate * model_texture.width;
                float uvy_interpolate = (surface1 * triangle.uvs[0] + surface2 * triangle.uvs[4] + surface3 * triangle.uvs[2]) * z_interpolate * model_texture.height;

                color_t final_colort = fetch_pixel(model_texture.texture,
                    uvx_interpolate,
                    uvy_interpolate,
                    model_texture.width, model_texture.height);


                vector4_t pixel_world_pos = {
                    point.x/(software_renderer.frame_buffer.width/2) - 1,
                    -point.y/(software_renderer.frame_buffer.height/2) + 1,
                    z_interpolate,
                    1
                }; 
                
                color_t normal_color;

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

                final_colort.r *= 0.5;
                final_colort.g *= 0.5;
                final_colort.b *= 0.5;

                for (int iter = 0; iter < software_renderer.light_count; iter++)
                {
                    light_t light = software_renderer.light_sources[iter];
                    vector4_t lightdir = normalize_vector(sub_vector4(pixel_world_pos, light.position));
                    float diffuse = dot_product(normal, lightdir);

                    if (diffuse < 0) diffuse = 0;

                    final_colort.r = final_colort.r + diffuse * light.color.r * light.intensity;
                    final_colort.g = final_colort.g + diffuse * light.color.g * light.intensity;
                    final_colort.b = final_colort.b + diffuse * light.color.b * light.intensity;

                    //final_colort.r = pixel_world_pos.x * 255 < 0 ? 0 : pixel_world_pos.x * 255;
                    //final_colort.g = pixel_world_pos.y * 255 < 0 ? 0 : pixel_world_pos.y * 255;
                    //final_colort.b = pixel_world_pos.z * 255 < 0 ? 0 : pixel_world_pos.z * 255;
                }

                int index = (int)((int)(y)*width + (x));

                if (index >= 0 && index < width * height && z_interpolate <= software_renderer.depth_buffer.depth_pixels[index])
                {
                    software_renderer.depth_buffer.depth_pixels[index] = z_interpolate;

                    switch (software_renderer.pass_display)
                    {
                        case DIFFUSE_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, final_colort);
                            break;
                        case DEPTH_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, { (z_interpolate), (z_interpolate), (z_interpolate), 255 });
                            break;
                        case NORMALS_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, normal_color);
                            break;
                        default:
                            break;
                    }
                }
                x++;
            }
            startx += d1;
            endx += d2;
        }
        startx = points[1].x;
        endx = xintersect;
        dx1 = points[1].x - points[2].x, dy1 = points[1].y - points[2].y;
        dx2 = xintersect - points[2].x, dy2 = points[1].y - points[2].y;
        d1 = dx1 / dy1;
        d2 = dx2 / dy2;

        for (float y = points[1].y; y <= points[2].y; y++)
        {
            int x = startx;
            if (startx > endx) x = endx;
            for (int i = 0; i <= fabs(startx - endx); i++)
            {
                vector2_t point = { x, y };
                float surface1 = (triangle_surface(point, point3, point2) / full_surface);
                float surface2 = (triangle_surface(point, point2, point1) / full_surface);
                float surface3 = (triangle_surface(point, point1, point3) / full_surface);

                float z_interpolate = 1.0 / (surface1/z1 + surface3/z2 + surface2/z3);

                int index = (int)((int)(y)*width + (x));

                float uvx_interpolate = (surface1 * triangle.uvs[1] + surface2 * triangle.uvs[5] + surface3 * triangle.uvs[3]) * z_interpolate * model_texture.width;
                float uvy_interpolate = (surface1 * triangle.uvs[0] + surface2 * triangle.uvs[4] + surface3 * triangle.uvs[2]) * z_interpolate * model_texture.height;

                color_t final_colort = fetch_pixel(model_texture.texture,
                    uvx_interpolate,
                    uvy_interpolate,
                    model_texture.width, model_texture.height);
                vector4_t pixel_world_pos = {
                    point.x / (software_renderer.frame_buffer.width / 2) - 1,
                    -point.y / (software_renderer.frame_buffer.height / 2) + 1,
                    z_interpolate,
                    1
                };

                color_t normal_color;

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

                final_colort.r *= 0.5;
                final_colort.g *= 0.5;
                final_colort.b *= 0.5;

                for (int iter = 0; iter < software_renderer.light_count; iter++)
                {
                    light_t light = software_renderer.light_sources[iter];
                    vector4_t lightdir = normalize_vector(sub_vector4(pixel_world_pos, light.position));
                    float diffuse = dot_product(normal, lightdir);                    

                    if (diffuse < 0) diffuse = 0;                   

                    final_colort.r = final_colort.r + diffuse * light.color.r * light.intensity;
                    final_colort.g = final_colort.g + diffuse * light.color.g * light.intensity;
                    final_colort.b = final_colort.b + diffuse * light.color.b * light.intensity;

                    //final_colort.r = pixel_world_pos.x * 255 < 0 ? 0 : pixel_world_pos.x * 255;
                    //final_colort.g = pixel_world_pos.y * 255 < 0 ? 0 : pixel_world_pos.y * 255;
                    //final_colort.b = pixel_world_pos.z * 255 < 0 ? 0 : pixel_world_pos.z * 255;
                }

                if (index >= 0 && index < width * height && z_interpolate <= software_renderer.depth_buffer.depth_pixels[index])
                {
                    software_renderer.depth_buffer.depth_pixels[index] = z_interpolate;

                    switch (software_renderer.pass_display)
                    {
                        case DIFFUSE_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, final_colort);
                            break;
                        case DEPTH_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, { (z_interpolate),(z_interpolate), (z_interpolate), 255 });
                            break;
                        case NORMALS_COLOR:
                            draw_pixel(x, y, software_renderer.frame_buffer.pixels, width, height, normal_color);
                            break;
                        default:
                            break;
                    }
                }
                x++;
            }
            startx += d1;
            endx += d2;
        }
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    const int width = 720;
    const int height = 720;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* surface = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, width, height);

    void* pixels;
    int   pitch;

    float angle = 180;
    float anglez = 0;

    float stride = 6;

    SDL_LockTexture(surface, 0, &pixels, &pitch);

    matrix4_t view_matrix =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -23, 0},
        {0, 0, 0, 1}
    };

    matrix4_t identity_matrix =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    vector4_t transform = {0.0, -0.1, -2.9, 1.0};

    float fov = 90.0;

    float aspect_ratio = (float)width/(float)height;

    renderer_t  software_renderer;
    model_t     model;
    light_t     light = {
        {1, 0, 0, 1},
        {255, 0, 255, 255},
        0.2
    };
    light_t     light2 = {
        {-1, 0, 0, 1},
        {0, 255, 0, 255},
        0.2
    };

    software_renderer.frame_buffer.pixels = (int*)pixels;
    software_renderer.frame_buffer.width = width;
    software_renderer.frame_buffer.height = height;
    software_renderer.depth_buffer.depth_pixels = (float*)malloc(width * height * sizeof(width * height));
    software_renderer.pass_display = DEPTH_COLOR;
    software_renderer.light_count = 1;
    software_renderer.light_sources[0] = light;
    software_renderer.light_sources[1] = light2;

    model.meshes = extract_meshes("models/suzan.obj");
    int w, h, channels;
    model.model_texture.texture = stbi_load("models/uv1.png", &w, &h, &channels, 4);
    model.model_texture.width = w;
    model.model_texture.height = h;

    if (!model.model_texture.texture) printf("bad texture\n");

    if (!model.meshes) { return 1; }

    float o = 0;
    while (1)
    {
        software_renderer.light_sources[0].position.x += sin(o);

        o += 1;

        float n = 0.01, f = 1000.0;
        float t = tan(rad_to_deg(fov/2)) * n, r = t * aspect_ratio;

        matrix4_t transform_matrix =
        {
            {1, 0, 0, transform.x},
            {0, 1, 0, transform.y},
            {0, 0, 1, transform.z},
            {0, 0, 0,           1}
        };

        matrix4_t perspective_matrix =
        {
            { n/r,   0,            0,                 0},
            {   0, n/t,            0,                 0},
            {   0,   0, -(f+n)/(f-n),    -2*(f*n)/(f-n)},
            {   0,   0,           -1,                 0}
        };

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        angle = 180; //;180 + mouseY;
        anglez += 1.5;//;mouseX;

        matrix4_t rotationx = {
            { 1,                       0,                        0, 0},
            { 0, cos(rad_to_deg(angle)),  -sin(rad_to_deg(angle)), 0},
            { 0, sin(rad_to_deg(angle)), cos(rad_to_deg(angle)), 0},
            { 0,                       0,                        0, 1}
        };
        matrix4_t rotationy = {
            {sin(rad_to_deg(anglez)),                         0, cos(rad_to_deg(anglez)), 0},
            {                     0,                          1,                       0, 0},
            {cos(rad_to_deg(anglez)),                         0,-sin(rad_to_deg(anglez)), 0},
            {                     0,                          0,                       0, 1}
        };
        matrix4_t rotation = multiply_matrix_matrix(rotationx, rotationy);

        matrix4_t model_matrix = multiply_matrix_matrix(transform_matrix, rotation);
        matrix4_t model_view_projection_matrix = multiply_matrix_matrix(perspective_matrix, model_matrix);

        SDL_Event ev;

        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                return 0;
            if (ev.type == SDL_KEYDOWN)
            {
                switch (ev.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                        transform.z += 0.03;
                        break;    
                    case SDL_SCANCODE_S:
                        transform.z -= 0.03;
                        break;    
                    case SDL_SCANCODE_A:
                        transform.x += 0.03;
                        break;
                    case SDL_SCANCODE_D:
                        transform.x -= 0.03;
                        break;
                    case SDL_SCANCODE_P:
                        software_renderer.pass_display = DIFFUSE_COLOR;
                        break;
                    case SDL_SCANCODE_O:
                        software_renderer.pass_display = DEPTH_COLOR;
                        break;
                    case SDL_SCANCODE_N:
                        software_renderer.pass_display = NORMALS_COLOR;
                        break;
                    default:
                        break;
                }
            }
        }
        {
            // reset depth buffer
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    software_renderer.depth_buffer.depth_pixels[i * width + j] = 100000; // depth buffer reinitialized to -100000

            // clear window
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    draw_pixel(j, i, software_renderer.frame_buffer.pixels, width, height, { 0, 0, 0, 255 });

            for (int i = 0; i < faces_numbers; i++)
            {                
                vector4_t vertex1 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[0], model.meshes[i].vertecies[1], model.meshes[i].vertecies[2], model.meshes[i].vertecies[3] });
                vector4_t vertex2 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[4], model.meshes[i].vertecies[5], model.meshes[i].vertecies[6], model.meshes[i].vertecies[7] });
                vector4_t vertex3 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[8], model.meshes[i].vertecies[9], model.meshes[i].vertecies[10], model.meshes[i].vertecies[11] });

                {
                    vertex1.x /= 2; vertex1.y /= 2; vertex1.z /= 2;
                    vertex2.x /= 2; vertex2.y /= 2; vertex2.z /= 2;
                    vertex3.x /= 2; vertex3.y /= 2; vertex3.z /= 2;

                    vertex1 = multiply_matrix_vector(model_view_projection_matrix, vertex1);
                    vertex2 = multiply_matrix_vector(model_view_projection_matrix, vertex2);
                    vertex3 = multiply_matrix_vector(model_view_projection_matrix, vertex3);
                }

                // calculate normals here for face culling
                vector4_t normal = normalize_vector(cross_product_4d(sub_vector4(vertex1, vertex3), sub_vector4(vertex1, vertex2)));

                float scalar_n = dot_product(normal, { 0.0, 0.0, -1.0, 1.0 });

                if (scalar_n > 0.0)  // 
                    continue;

                {
                    vertex1 = multiply_matrix_vector(view_matrix, vertex1);
                    vertex2 = multiply_matrix_vector(view_matrix, vertex2);
                    vertex3 = multiply_matrix_vector(view_matrix, vertex3);

                    vertex1.x /= -vertex1.w;
                    vertex1.y /= -vertex1.w;
                    vertex1.z /= -vertex1.w;

                    vertex2.x /= -vertex2.w;
                    vertex2.y /= -vertex2.w;
                    vertex2.z /= -vertex2.w;

                    vertex3.x /= -vertex3.w;
                    vertex3.y /= -vertex3.w;
                    vertex3.z /= -vertex3.w;

                }

                if (vertex3.w <= 1 || vertex2.w <= 1 || vertex1.w <= 1) continue;

                mesh_t triangle = {                                  
                        vertex1.x, vertex1.y, vertex1.z, vertex1.w,
                        vertex2.x, vertex2.y, vertex2.z, vertex2.w,
                        vertex3.x, vertex3.y, vertex3.z, vertex3.w
                        ,
                        model.meshes[i].uvs[0]/vertex1.z,
                        model.meshes[i].uvs[1]/vertex1.z,
                        model.meshes[i].uvs[2]/vertex2.z,
                        model.meshes[i].uvs[3]/vertex2.z,
                        model.meshes[i].uvs[4]/vertex3.z,
                        model.meshes[i].uvs[5]/vertex3.z
                };

                drawTriangle(triangle, software_renderer, FILLED, model.model_texture, normal);
            }
        }

        SDL_UnlockTexture(surface);

        SDL_RenderCopy(renderer, surface, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
}
