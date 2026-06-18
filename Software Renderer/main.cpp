#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include "model_parser.c"
#include "renderer.h"
#include "math.h"
#include "stb_image.h"
#include "triangle.cpp"

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
        {0, 0, -17, 0},
        {0, 0, 0, 1}
    };

    matrix4_t identity_matrix =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    vector4_t transform = {0.0, 0.0, -7.0, 1.0};

    float fov = 90.0;

    float aspect_ratio = (float)width/(float)height;

    renderer_t  software_renderer;
    model_t     model;

    software_renderer.frame_buffer.pixels = (int*)pixels;
    software_renderer.frame_buffer.width = width;
    software_renderer.frame_buffer.height = height;
    software_renderer.depth_buffer.depth_pixels = (float*)malloc(width * height * sizeof(float));
    software_renderer.pass_display = DIFFUSE_COLOR;

    model.meshes = extract_meshes("models/jack.obj");
    int w, h, channels;
    model.model_texture.texture = stbi_load("models/uv1.png", &w, &h, &channels, 4);
    model.model_texture.width = w;
    model.model_texture.height = h;

    if (!model.model_texture.texture) printf("bad texture\n");
    if (!model.meshes) { return 1; }

    float o = 0;
    float n = 0.01, f = 1000.0;
    float t = tan(rad_to_deg(fov/2)) * n, r = t * aspect_ratio;

    software_renderer.r = r;
    software_renderer.t = t;    
    software_renderer.near_plane = n;
    software_renderer.far_plane = f;

    while (1)
    {
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

		angle = 180;
		anglez = mouseX;

		matrix4_t rotationx = {
			{ 1,                       0,                        0, 0},
			{ 0, (float)cos(rad_to_deg(angle)),  (float)-sin(rad_to_deg(angle)), 0},
			{ 0, (float)sin(rad_to_deg(angle)), (float)cos(rad_to_deg(angle)), 0},
			{ 0,                       0,                        0, 1}
		};
		matrix4_t rotationy = {
			{(float)sin(rad_to_deg(anglez)),                         0, (float)cos(rad_to_deg(anglez)), 0},
			{                     0,                          1,                       0, 0},
			{(float)cos(rad_to_deg(anglez)),                         0,(float)-sin(rad_to_deg(anglez)), 0},
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
					case SDL_SCANCODE_H:
						fov += 0.03;
						break;   
					case SDL_SCANCODE_R:
						fov -= 0.03;
						break;    
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
					case SDL_SCANCODE_Z:
						software_renderer.pass_display = WORLD_POS_COLOR;
						break;
					default:
						break;
				}
			}
		}
		{
			// clear window & and reset depth buffer
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)				
				{
					software_renderer.depth_buffer.depth_pixels[i * width + j] = 100000;
					draw_pixel(j, i, software_renderer.frame_buffer.pixels, width, height, { 0, 0, 0, 255 });
				}


			for (int i = 0; i < faces_numbers; i++)
			{                
				vector4_t vertex1 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[0], model.meshes[i].vertecies[1], model.meshes[i].vertecies[2], model.meshes[i].vertecies[3] });
				vector4_t vertex2 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[4], model.meshes[i].vertecies[5], model.meshes[i].vertecies[6], model.meshes[i].vertecies[7] });
				vector4_t vertex3 = multiply_matrix_vector(identity_matrix, { model.meshes[i].vertecies[8], model.meshes[i].vertecies[9], model.meshes[i].vertecies[10], model.meshes[i].vertecies[11]});

				{
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

					vertex2.x /= -vertex2.w;
					vertex2.y /= -vertex2.w;

					vertex3.x /= -vertex3.w;
					vertex3.y /= -vertex3.w;
                }

                if (vertex3.w <= 1 || vertex2.w <= 1 || vertex1.w <= 1) continue;

                mesh_t triangle = {                                  
						vertex1.x, vertex1.y, vertex1.z, vertex1.w,
						vertex2.x, vertex2.y, vertex2.z, vertex2.w,
						vertex3.x, vertex3.y, vertex3.z, vertex3.w
						,
						model.meshes[i].uvs[0]/-vertex1.z,
						model.meshes[i].uvs[1]/-vertex1.z,
						model.meshes[i].uvs[2]/-vertex2.z,
						model.meshes[i].uvs[3]/-vertex2.z,
						model.meshes[i].uvs[4]/-vertex3.z,
						model.meshes[i].uvs[5]/-vertex3.z
                };

                drawTriangle(triangle, software_renderer, FILLED, model.model_texture, normal);
            }
        }

		SDL_UnlockTexture(surface);

		SDL_RenderCopy(renderer, surface, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}
	return 0;
}
