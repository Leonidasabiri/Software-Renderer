#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include "model_parser.c"

typedef enum
{
    WIREFRAME,
    FILLED
}triangle_draw_mode_t;

typedef struct
{
    float r, g, b, a;
} color_t;

typedef struct
{
    float x, y;
} vector2_t;

typedef struct
{
    float x, y, z, w;
} vector4_t;

typedef struct
{
    vector4_t x, y, z, w;
} matrix4_t;

typedef struct
{
    int width;
    int height;
    int pixels_buffer;
    float zbuffer;
} renderer_t;

matrix4_t multiply_matrix_matrix(matrix4_t matrix4_1, matrix4_t matrix4_2)
{
    float xx = matrix4_1.x.x * matrix4_2.x.x + matrix4_1.x.y * matrix4_2.y.x + matrix4_1.x.z * matrix4_2.z.x + matrix4_1.x.w * matrix4_2.w.x;
    float xy = matrix4_1.x.x * matrix4_2.x.y + matrix4_1.x.y * matrix4_2.y.y + matrix4_1.x.z * matrix4_2.z.y + matrix4_1.x.w * matrix4_2.w.y;
    float xz = matrix4_1.x.x * matrix4_2.x.z + matrix4_1.x.y * matrix4_2.y.z + matrix4_1.x.z * matrix4_2.z.z + matrix4_1.x.w * matrix4_2.w.z;
    float xw = matrix4_1.x.x * matrix4_2.x.w + matrix4_1.x.y * matrix4_2.y.w + matrix4_1.x.z * matrix4_2.z.w + matrix4_1.x.w * matrix4_2.w.w;

    float yx = matrix4_1.y.x * matrix4_2.x.x + matrix4_1.y.y * matrix4_2.y.x + matrix4_1.y.z * matrix4_2.z.x + matrix4_1.y.w * matrix4_2.w.x;
    float yy = matrix4_1.y.x * matrix4_2.x.y + matrix4_1.y.y * matrix4_2.y.y + matrix4_1.y.z * matrix4_2.z.y + matrix4_1.y.w * matrix4_2.w.y;
    float yz = matrix4_1.y.x * matrix4_2.x.z + matrix4_1.y.y * matrix4_2.y.z + matrix4_1.y.z * matrix4_2.z.z + matrix4_1.y.w * matrix4_2.w.z;
    float yw = matrix4_1.y.x * matrix4_2.x.w + matrix4_1.y.y * matrix4_2.y.w + matrix4_1.y.z * matrix4_2.z.w + matrix4_1.y.w * matrix4_2.w.w;

    float zx = matrix4_1.z.x * matrix4_2.x.x + matrix4_1.z.y * matrix4_2.y.x + matrix4_1.z.z * matrix4_2.z.x + matrix4_1.z.w * matrix4_2.w.x;
    float zy = matrix4_1.z.x * matrix4_2.x.y + matrix4_1.z.y * matrix4_2.y.y + matrix4_1.z.z * matrix4_2.z.y + matrix4_1.z.w * matrix4_2.w.y;
    float zz = matrix4_1.z.x * matrix4_2.x.z + matrix4_1.z.y * matrix4_2.y.z + matrix4_1.z.z * matrix4_2.z.z + matrix4_1.z.w * matrix4_2.w.z;
    float zw = matrix4_1.z.x * matrix4_2.x.w + matrix4_1.z.y * matrix4_2.y.x + matrix4_1.z.z * matrix4_2.z.w + matrix4_1.z.w * matrix4_2.w.w;

    float wx = matrix4_1.w.x * matrix4_2.x.x + matrix4_1.w.y * matrix4_2.y.x + matrix4_1.w.z * matrix4_2.z.x + matrix4_1.w.w * matrix4_2.w.x;
    float wy = matrix4_1.w.x * matrix4_2.x.y + matrix4_1.w.y * matrix4_2.y.y + matrix4_1.w.z * matrix4_2.z.y + matrix4_1.w.w * matrix4_2.w.y;
    float wz = matrix4_1.w.x * matrix4_2.x.z + matrix4_1.w.y * matrix4_2.y.z + matrix4_1.w.z * matrix4_2.z.z + matrix4_1.w.w * matrix4_2.w.z;
    float ww = matrix4_1.w.x * matrix4_2.x.w + matrix4_1.w.y * matrix4_2.y.w + matrix4_1.w.z * matrix4_2.z.w + matrix4_1.w.w * matrix4_2.w.w;

    vector4_t x = { xx, xy, xz, xw };
    vector4_t y = { yx, yy, yz, yw };
    vector4_t z = { zx, zy, zz, zw };
    vector4_t w = { wx, wy, wz, ww };

    return
    {
        x,
        y,
        z,
        w
    };
}

matrix4_t add_matrix_matrix(matrix4_t matrix4_1, matrix4_t matrix4_2)
{
    return
    {
       {matrix4_1.x.x + matrix4_2.x.x, matrix4_1.x.y + matrix4_2.x.y, matrix4_1.x.z + matrix4_2.x.z, matrix4_1.x.w + matrix4_2.x.w},
       {matrix4_1.y.x + matrix4_2.y.x, matrix4_1.y.y + matrix4_2.y.y, matrix4_1.y.z + matrix4_2.y.z, matrix4_1.y.w + matrix4_2.y.w},
       {matrix4_1.z.x + matrix4_2.z.x, matrix4_1.z.y + matrix4_2.z.y, matrix4_1.z.z + matrix4_2.z.z, matrix4_1.z.w + matrix4_2.z.w},
       {matrix4_1.w.x + matrix4_2.w.x, matrix4_1.w.y + matrix4_2.w.y, matrix4_1.w.z + matrix4_2.w.z, matrix4_1.w.w + matrix4_2.w.w}
    };
}

vector2_t sub_vector(vector2_t vector1, vector2_t vector2)
{
    return { vector1.x - vector2.x, vector1.y - vector2.y };
}

vector4_t multiply_matrix_vector(matrix4_t matrix4_1, vector4_t vector)
{
    return
    {
       matrix4_1.x.x * vector.x + matrix4_1.x.y * vector.y + matrix4_1.x.z * vector.z + matrix4_1.x.w * vector.w,
       matrix4_1.y.x * vector.x + matrix4_1.y.y * vector.y + matrix4_1.y.z * vector.z + matrix4_1.y.w * vector.w,
       matrix4_1.z.x * vector.x + matrix4_1.z.y * vector.y + matrix4_1.z.z * vector.z + matrix4_1.z.w * vector.w,
       matrix4_1.w.x * vector.x + matrix4_1.w.y * vector.y + matrix4_1.w.z * vector.z + matrix4_1.w.w * vector.w
    };
}

matrix4_t matrix4x4_transpose(matrix4_t matrix4_1)
{
    return
    {
       {matrix4_1.x.x, matrix4_1.y.x, matrix4_1.z.x, matrix4_1.w.x},
       {matrix4_1.x.y, matrix4_1.y.y, matrix4_1.z.y, matrix4_1.w.y},
       {matrix4_1.x.z, matrix4_1.y.z, matrix4_1.z.z, matrix4_1.w.z},
       {matrix4_1.x.w, matrix4_1.y.w, matrix4_1.z.w, matrix4_1.w.w}
    };
}

void draw_pixel(int x, int y, int* buffer, int width, int height, color_t color)
{
    if (x < 0 || y < 0 || x > width || y > height)
        return;
    int pos = (x + y * width);
    buffer[pos] = (int)color.r << 24 | (int)color.g << 16 | (int)color.b << 8 | (int)color.a;
}

float cross_product_2d(vector2_t vector1, vector2_t vector2)
{
    return vector1.x * vector2.y - vector1.y * vector2.x;
}

float triangle_surface(vector2_t pos1, vector2_t pos2, vector2_t pos3)
{
    return cross_product_2d(sub_vector(pos1, pos2), sub_vector(pos3, pos2)) * 1 / 2;
}

vector2_t convert_to_screen_space(vector4_t pos, int width, int height)
{
    vector2_t screen_space;

    screen_space.x = (pos.x + 1) * width/2;
    screen_space.y = (-pos.y + 1) * height/2;

    return screen_space;
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

void drawTriangle(vector2_t point1, vector2_t point2, vector2_t point3,
    int* buffer, int width, int height, color_t color[3],
    triangle_draw_mode_t triangle_mode, float* zbuffer, float z1, float z2, float z3)
{

    vector2_t points[3];

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

    int xintersect = (points[0].x + (points[1].y - points[0].y) / (points[2].y - points[0].y) * (points[2].x - points[0].x));

    float startx = points[0].x, endx = points[0].x;

    float dx1 = points[0].x - points[1].x, dy1 = points[0].y - points[1].y;
    float dx2 = points[0].x - xintersect, dy2 = points[0].y - points[1].y;
    float d1 = dx1 / dy1;
    float d2 = dx2 / dy2;
    float full_surface = triangle_surface(point1, point3, point2);

    switch (triangle_mode)
    {
    case WIREFRAME:
        printf("color line\n");
        drawLine(point1, point2, buffer, width, height, color[0]);
        drawLine(point2, point3, buffer, width, height, color[0]);
        drawLine(point3, point1, buffer, width, height, color[0]);
        break;
    case FILLED:
        for (float y = points[0].y; y <= points[1].y; y++)
        {
            for (int x = startx; x <= endx; x++)
            {
                vector2_t point = { x, y };
                float surface1 = triangle_surface(point, point3, point2) / full_surface;
                float surface2 = triangle_surface(point, point2, point1) / full_surface;
                float surface3 = triangle_surface(point, point1, point3) / full_surface;

                color_t final_color = {
                    (int)(surface1 * color[0].r + surface2 * color[1].r + surface3 * color[2].r),
                    (int)(surface1 * color[0].g + surface2 * color[1].g + surface3 * color[2].g),
                    (int)(surface1 * color[0].b + surface2 * color[1].b + surface3 * color[2].b)
                };

                float z_interpolate = 1.0 / (surface1 * 1.0 / z1 + surface3 * 1.0 / z2 + surface2 * 1.0 / z3) * 255;

                int index = (int)((int)(y)*width + (x));

                if (index >= 0 && index < width * height && z_interpolate >= zbuffer[index])
                {
                    zbuffer[index] = z_interpolate;
                    draw_pixel(x, y, buffer, width, height, final_color);
                }
            }
            for (int x = endx; x <= startx; x++)
            {
                vector2_t point = { x, y };
                float surface1 = triangle_surface(point, point3, point2) / full_surface;
                float surface2 = triangle_surface(point, point2, point1) / full_surface;
                float surface3 = triangle_surface(point, point1, point3) / full_surface;

                color_t final_color = {
                    (int)(surface1 * color[0].r + surface2 * color[1].r + surface3 * color[2].r),
                    (int)(surface1 * color[0].g + surface2 * color[1].g + surface3 * color[2].g),
                    (int)(surface1 * color[0].b + surface2 * color[1].b + surface3 * color[2].b)
                };
                float z_interpolate = 1.0 / (surface1 * 1.0 / z1 + surface3 * 1.0 / z2 + surface2 * 1.0 / z3) * 255;


                int index = (int)((int)(y)*width + (x));

                if (index >= 0 && index < width * height && z_interpolate >= zbuffer[index])
                {
                    zbuffer[index] = z_interpolate;
                    draw_pixel(x, y, buffer, width, height, final_color);
                }

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

        for (float y = points[1].y; y < points[2].y; y++)
        {
            for (int x = startx; x < endx; x++)
            {
                vector2_t point = { x, y };
                float surface1 = triangle_surface(point, point3, point2) / full_surface;
                float surface2 = triangle_surface(point, point2, point1) / full_surface;
                float surface3 = triangle_surface(point, point1, point3) / full_surface;

                color_t final_color = {
                    (int)(surface1 * color[0].r + surface2 * color[1].r + surface3 * color[2].r) ,
                    (int)(surface1 * color[0].g + surface2 * color[1].g + surface3 * color[2].g) ,
                    (int)(surface1 * color[0].b + surface2 * color[1].b + surface3 * color[2].b)
                };
                float z_interpolate = 1.0 / (surface1 * 1.0 / z1 + surface3 * 1.0 / z2 + surface2 * 1.0 / z3) * 255;

                int index = (int)((int)(y)*width + (x));

                if (index >= 0 && index < width * height && z_interpolate >= zbuffer[index])
                {
                    zbuffer[index] = z_interpolate;
                    draw_pixel(x, y, buffer, width, height, final_color);
                }

            }
            for (int x = endx; x < startx; x++)
            {
                vector2_t point = { x, y };
                float surface1 = triangle_surface(point, point3, point2) / full_surface;
                float surface2 = triangle_surface(point, point2, point1) / full_surface;
                float surface3 = triangle_surface(point, point1, point3) / full_surface;

                color_t final_color = {
                    (surface1 * color[0].r + surface2 * color[1].r + surface3 * color[2].r),
                    (surface1 * color[0].g + surface2 * color[1].g + surface3 * color[2].g),
                    (surface1 * color[0].b + surface2 * color[1].b + surface3 * color[2].b)
                };
                float z_interpolate = 1.0 / (surface1 * 1.0 / z1 + surface3 * 1.0 / z2 + surface2 * 1.0 / z3) * 255;

                int index = (int)((int)(y)*width + x);

                if (index >= 0 && index < width * height && z_interpolate >= zbuffer[index])
                {
                    zbuffer[index] = z_interpolate;
                    draw_pixel(x, y, buffer, width, height, final_color);
                }
            }
            startx += d1;
            endx += d2;
        }
        break;
    default:
        break;
    }
}

float rad_to_deg(float angle)
{
    return 3.14 / 180 * angle;
}

int main(int argc, char* argv[])
{
    const int width = 580;
    const int height = 720;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* surface = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, width, height);

    void* pixels;
    int   pitch;
    float* zbuffer = (float*)malloc(width * height * sizeof(width * height));

    float angle = 0;
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

    vector4_t transform = {0.0, 0.0, 1.0, 1.0};

    matrix4_t transform_matrix =
    {
        {1, 0, 0, transform.x},
        {0, 1, 0, transform.y},
        {0, 0, 1, transform.z},
        {0, 0, 0,           1}
    };

    float fov = 90.0;
   
    float aspect_ratio = (float)width/(float)height;
    
    mesh_t* meshes = extract_meshes("utah.obj");

    //return 0;
    while (1)
    {
        float n = 1.0, f = 100.0;
        float t = tan(rad_to_deg(fov/2)) * n, r = t * aspect_ratio;

        matrix4_t perspective_matrix =
        {
            { n/r,   0,            0,                 0},
            {   0, n/t,            0,                 0},
            {   0,   0, -(f+n)/(f-n),    -2*(f*n)/(f-n)},
            {   0,   0,           -1,                 1}
        };

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (-mouseY <= -250 || -mouseY <= 250)
            angle = -(mouseY);
        anglez = -mouseX;

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

        matrix4_t model_view_projection_matrix;

        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                return 0;
            if (ev.type == SDL_KEYDOWN)
            {
                fov++;
            }
        }
        {
            int* pixs = (int*)pixels;

            // reset depth buffer
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    zbuffer[i * width + j] = -1000000; // depth buffer reinitialized to -100000

            // clear window
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    draw_pixel(j, i, pixs, width, height, { 0, 0, 0, 255 });

            for (int i = 0; i < faces_numbers; i++)
            {
                //continue;


                vector4_t vertex1 = multiply_matrix_vector(identity_matrix, { meshes[i].vertecies[0]/10, meshes[i].vertecies[1] / 10, meshes[i].vertecies[2] / 10, 1 });
                vector4_t vertex2 = multiply_matrix_vector(identity_matrix, { meshes[i].vertecies[3] / 10, meshes[i].vertecies[4] / 10, meshes[i].vertecies[5] / 10, 1 });
                vector4_t vertex3 = multiply_matrix_vector(identity_matrix, { meshes[i].vertecies[6] / 10, meshes[i].vertecies[7] / 10, meshes[i].vertecies[8] / 10, 1 });

                //vertex1 = multiply_matrix_vector(perspective_matrix, vertex1);
                //vertex2 = multiply_matrix_vector(perspective_matrix, vertex2);
                //vertex3 = multiply_matrix_vector(perspective_matrix, vertex3);

                //vertex1 = multiply_matrix_vector(transform_matrix, vertex1);
                //vertex2 = multiply_matrix_vector(transform_matrix, vertex2);
                //vertex3 = multiply_matrix_vector(transform_matrix, vertex3);

                vertex1 = multiply_matrix_vector(rotation, vertex1);
                vertex2 = multiply_matrix_vector(rotation, vertex2);
                vertex3 = multiply_matrix_vector(rotation, vertex3);

                //vertex1 = multiply_matrix_vector(view_matrix, vertex1);
                //vertex2 = multiply_matrix_vector(view_matrix, vertex2);
                //vertex3 = multiply_matrix_vector(view_matrix, vertex3);


                //vertex1.x /= vertex1.w;
                //vertex1.y /= vertex1.w;
                //vertex1.z /= vertex1.w;

                //vertex2.x /= vertex2.w;
                //vertex2.y /= vertex2.w;
                //vertex2.z /= vertex2.w;

                //vertex3.x /= vertex3.w;
                //vertex3.y /= vertex3.w;
                //vertex3.z /= vertex3.w;

                vector2_t point1 = convert_to_screen_space(vertex1, width, height);
                vector2_t point2 = convert_to_screen_space(vertex2, width, height);
                vector2_t point3 = convert_to_screen_space(vertex3, width, height);

                color_t col[3] =
                {
                    {255, 0, 0, 255},
                    {0, 255, 0, 255},
                    {0, 0, 255, 255}
                };

                draw_pixel(point1.x, point1.y, pixs, width, height, col[0]);
                draw_pixel(point2.x, point2.y, pixs, width, height, col[0]);
                draw_pixel(point3.x, point3.y, pixs, width, height, col[0]);
                drawTriangle(point1, point2, point3, pixs, width, height, col, WIREFRAME, zbuffer, vertex1.z, vertex2.z, vertex3.z);
            }
        }
        SDL_UnlockTexture(surface);

        SDL_RenderCopy(renderer, surface, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
}
