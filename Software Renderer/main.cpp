#include <SDL.h>
#include <math.h>
#include <stdio.h>


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

    return
    {
        {xx, xy, xz, xw },
        {yx, yy, yz, yw},
        {zx, zy, zz, zw },
        {wx, wy, wz, ww}
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
    return { vector1.x - vector2.x, vector1.y - vector2.y};
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

void draw_pixel(int x, int y, int *buffer, int width, int height, color_t color)
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

    screen_space.x = (pos.x /(pos.z) + 1) * width/2;
    screen_space.y = (pos.y/(-pos.z) + 1) * height/2;

    return screen_space;
}

typedef enum
{
    WIREFRAME,
    FILLED
}triangle_draw_mode_t;


void drawLine(vector2_t point1, vector2_t point2, int *buffer, int width, int height, color_t color)
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
        float d = -(starty - endy) / -(startx - endx);
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
        float d = -(startx - endx) / -(starty - endy);
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
    float d1 = dx1/dy1;
    float d2 = dx2/dy2;
    float full_surface = triangle_surface(point1, point3, point2);

    switch (triangle_mode)
    {
        case WIREFRAME:
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

                    float z_interpolate = 1.0/(surface1 * 1.0/z1 + surface3 * 1.0/z2 + surface2 * 1.0/z3) * 255;

                    int index = (int)((int)(y) * width + (x));

                    if (z_interpolate >= zbuffer[index])
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


                    int index = (int)((int)(y) * width + (x));

                    if (z_interpolate >= zbuffer[index])
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

                    int index = (int)((int)(y) * width + (x));

                    if (z_interpolate >= zbuffer[index])
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

                    int index = (int)((int)(y) * width + x);

                    if (z_interpolate >= zbuffer[index])
                    {
                        zbuffer[index] = z_interpolate;
                        draw_pixel(x, y, buffer, width, height, final_color);
                    }
                }
                startx += d1;
                endx += d2;
            }
            break ;
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

    void *pixels;
    int   pitch;
    float* zbuffer = (float*)malloc(width * height * sizeof(width * height));


    float triangle[] =
    {
        // front
        0.2f,  0.2f, 0.2f,  1.0f, 0.0f, 0.0f,   // top right
        0.2f, -0.2f, 0.2f,  0.0f, 1.0f, 0.0f,   // bottom right
       -0.2f, -0.2f, 0.2f,  0.0f, 0.0f, 1.0f, // bottom left
       -0.2f,  0.2f, 0.2f,  1.0f, 0.0f, 0.0f,  // top left 

        //back
        0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 0.0f,  // top right
        0.2f, -0.2f, -0.2f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.2f, -0.2f, -0.2f, 0.0f, 0.0f, 1.0f,// bottom left
       -0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 0.0f// top left 
    };

    int indecies[] = {
       0, 1, 3,
       1, 2, 3,
       // back
       4, 5, 7,
       5, 6, 7,
       // right
       0, 1, 4,
       1, 4, 5,
       // left
       2, 3, 7,
       2, 6, 7,
       // top
       0, 3, 4,
       3, 4, 7,
       // bottom
       1, 2, 5,
       2, 5, 6
    };

    float angle = 0;
    float anglez = 0;

    float stride = 6;

    SDL_LockTexture(surface, 0, &pixels, &pitch);

    float fov = 90.0;
    fov = rad_to_deg(fov/2);
    float near_plane = 0.01, far_plane = 1000.0;
    float s = 1 / tan(fov);

    matrix4_t perspective_matrix =
    {
        {s, 0, 0, 0},
        {0, s, 0, 0},
        {0, 0, -far_plane/(far_plane - near_plane), -1},
        {0, 0, -far_plane * near_plane / (far_plane - near_plane), 0}
    };

    matrix4_t view_matrix =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0,-2, 0},
        {0, 0, 0, 1}
    };

    while(1)
    {
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
            {sin(rad_to_deg(anglez)),                          0,  cos(rad_to_deg(anglez)), 0},
            {                     0,                          1,                       0, 0},
            {cos(rad_to_deg(anglez)),                          0, -sin(rad_to_deg(anglez)), 0},
            {                     0,                          0,                       0, 1}
        };

        matrix4_t rotation = multiply_matrix_matrix(rotationx, rotationy);


        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                return 0;
        }
        {

            int* pixs = (int*)pixels;

            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    zbuffer[i * width + j] = -1000000;

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    draw_pixel(j, i, pixs, width, height, { 0, 0, 0, 255 });
                }
            }
            for (int i = 0 ; i < sizeof(indecies)/sizeof(indecies[0]); i += 3)
            {
                int ind1x = indecies[i] * stride, ind1y = indecies[i] * stride + 1, ind1z = indecies[i] * stride + 2;
                int ind2x = indecies[i + 1] * stride, ind2y = indecies[i + 1] * stride + 1, ind2z = indecies[i + 1] * stride + 2;
                int ind3x = indecies[i + 2] * stride, ind3y = indecies[i + 2] * stride + 1, ind3z = indecies[i + 2] * stride + 2;

                vector4_t vertex1 = multiply_matrix_vector(rotation, { triangle[ind1x], triangle[ind1y], triangle[ind1z], 1});
                vector4_t vertex2 = multiply_matrix_vector(rotation, { triangle[ind2x], triangle[ind2y], triangle[ind2z], 1 });
                vector4_t vertex3 = multiply_matrix_vector(rotation, { triangle[ind3x], triangle[ind3y], triangle[ind3z], 1 });

                vertex1 = multiply_matrix_vector(perspective_matrix, vertex1);
                vertex2 = multiply_matrix_vector(perspective_matrix, vertex2);
                vertex3 = multiply_matrix_vector(perspective_matrix, vertex3);

                vertex1 = multiply_matrix_vector(view_matrix, vertex1);
                vertex2 = multiply_matrix_vector(view_matrix, vertex2);
                vertex3 = multiply_matrix_vector(view_matrix, vertex3);

                vector2_t point1 = convert_to_screen_space(vertex1, width, height);
                vector2_t point2 = convert_to_screen_space(vertex2, width, height);
                vector2_t point3 = convert_to_screen_space(vertex3, width, height);

                point1.x *= height / width;
                point2.x *= height / width;
                point3.x *= height / width;

                color_t col[3] =
                {
                    {triangle[ind1x + 3] * 255, triangle[ind1y + 3] * 255, triangle[ind1z + 3] * 255, 255},
                    {triangle[ind2x + 3] * 255, triangle[ind2y + 3] * 255, triangle[ind2z + 3] * 255, 255},
                    {triangle[ind3x + 3] * 255, triangle[ind3y + 3] * 255, triangle[ind3z + 3] * 255, 255 }
                };

                drawTriangle(point1, point2, point3, pixs, width,
                            height, col, FILLED, zbuffer, 
                            -vertex1.z, -vertex2.z, -vertex3.z);
                //drawTriangle(point1, point2, point3, pixs, width, height, col, 
                //            WIREFRAME, zbuffer, vertex1.z, vertex2.z, vertex3.z);
            }
        }
        SDL_UnlockTexture(surface);

        SDL_RenderCopy(renderer, surface, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
}
