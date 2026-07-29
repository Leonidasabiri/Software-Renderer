#include "shading.cpp"

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

void drawTriangle(mesh_t triangle, renderer_t software_renderer, 
                  triangle_draw_mode_t triangle_mode, 
                  texture_t model_texture, vector4_t normal)
{
	color_t color[3] = {255, 255, 255, 255};

	vector4_t vertex1 = { triangle.vertecies[0], triangle.vertecies[1], triangle.vertecies[2], triangle.vertecies[3] };
	vector4_t vertex2 = { triangle.vertecies[4], triangle.vertecies[5], triangle.vertecies[6], triangle.vertecies[7] };
	vector4_t vertex3 = { triangle.vertecies[8], triangle.vertecies[9], triangle.vertecies[10], triangle.vertecies[11] };

	float z1 = -vertex1.z, z2 = -vertex2.z, z3 = -vertex3.z;

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

	// NOTE: This is a dumb way to fix the problem of gaps between shared edges
	points[0].x = float(int(points[0].x));
	points[0].y = float(int(points[0].y));
	points[1].x = float(int(points[1].x));
	points[1].y = float(int(points[1].y));
	points[2].x = float(int(points[2].x));
	points[2].y = float(int(points[2].y));

	float xintersect = (points[0].x + (points[1].y - points[0].y)/(points[2].y - points[0].y) * (points[2].x - points[0].x));

	float startx = points[0].x, endx = points[0].x;

	float dx1 = points[0].x - points[1].x, dy1 = points[0].y - points[1].y;
	float dx2 = points[0].x - xintersect, dy2 = points[0].y - points[1].y;
	float d1 = dx1 / dy1;
	float d2 = dx2 / dy2;
	float full_surface = triangle_surface(point1, point3, point2);

	int width = software_renderer.frame_buffer.width;
	int height = software_renderer.frame_buffer.height;

	float scanline_start_y = points[0].y;
	float scanline_end_y = points[1].y;

	switch (triangle_mode)
	{
		case WIREFRAME:
			drawLine(point1, point2, software_renderer.frame_buffer.pixels, width, height, color[0]);
			drawLine(point2, point3, software_renderer.frame_buffer.pixels, width, height, color[0]);
			drawLine(point3, point1, software_renderer.frame_buffer.pixels, width, height, color[0]);
			break;
		case FILLED:
			for (float y = (scanline_start_y); y < scanline_end_y; y++)
			{
				float x = startx;
				if (startx > endx) x = endx;
				for (int i = 0; i < fabs(startx - endx); i++)
				{
					vector2_t point = { x, y };
					float surface1 = triangle_surface(point, point3, point2)/full_surface;
					float surface2 = triangle_surface(point, point2, point1)/full_surface;
					float surface3 = triangle_surface(point, point1, point3)/full_surface;

					float z_interpolate = 1.0 / (surface1/z1 + surface3/z2 + surface2/z3);
					float z_pos = (surface1*vertex1.w + surface2*vertex3.w + surface3*vertex2.w);

					float uvx_interpolate = (surface1 * triangle.uvs[1] + surface2 * triangle.uvs[5] + surface3 * triangle.uvs[3]) * z_interpolate * model_texture.width;
					float uvy_interpolate = (surface1 * triangle.uvs[0] + surface2 * triangle.uvs[4] + surface3 * triangle.uvs[2]) * z_interpolate * model_texture.height;

					software_renderer.current_screen_pos = {x, y};
					software_renderer.current_uvs = {uvx_interpolate, uvy_interpolate};
					software_renderer.current_z_value = z_interpolate;

					color_t final_colort = fetch_pixel(model_texture.texture,
						uvx_interpolate,
						uvy_interpolate,
						model_texture.width, model_texture.height);

					vector4_t pixel_world_pos = screen_to_world_pos(software_renderer, {point.x, point.y, 0, z_pos});

					pixel_shading(software_renderer, pixel_world_pos, normal, model_texture);
					x++;
				}
				startx += d1;
				endx += d2;
			}
			{
				scanline_start_y = scanline_end_y;
				scanline_end_y = points[2].y;
				startx = points[1].x;
				endx = xintersect;
				dx1 = points[1].x - points[2].x, dy1 = points[1].y - points[2].y;
				dx2 = xintersect - points[2].x, dy2 = points[1].y - points[2].y;
				d1 = dx1 / dy1;
				d2 = dx2 / dy2;
			}
			for (float y = scanline_start_y; y < scanline_end_y; y++)
			{
				float x = startx;
				if (startx > endx) x = endx;
				for (int i = 0; i < fabs(startx - endx); i++)
				{
					vector2_t point = { x, y };
					float surface1 = triangle_surface(point, point3, point2)/full_surface;
					float surface2 = triangle_surface(point, point2, point1)/full_surface;
					float surface3 = triangle_surface(point, point1, point3)/full_surface;

					float z_interpolate = 1.0 / (surface1/z1 + surface3/z2 + surface2/z3);
					float z_pos = (surface1*vertex1.w + surface2*vertex3.w + surface3*vertex2.w);

					float uvx_interpolate = (surface1 * triangle.uvs[1] + surface2 * triangle.uvs[5] + surface3 * triangle.uvs[3]) * z_interpolate * model_texture.width;
					float uvy_interpolate = (surface1 * triangle.uvs[0] + surface2 * triangle.uvs[4] + surface3 * triangle.uvs[2]) * z_interpolate * model_texture.height;

					software_renderer.current_screen_pos = {x, y};
					software_renderer.current_uvs = {uvx_interpolate, uvy_interpolate};
					software_renderer.current_z_value = z_interpolate;

					color_t final_colort = fetch_pixel(model_texture.texture,
						uvx_interpolate,
						uvy_interpolate,
						model_texture.width, model_texture.height);

					vector4_t pixel_world_pos = screen_to_world_pos(software_renderer, {point.x, point.y, 0, z_pos});

					pixel_shading(software_renderer, pixel_world_pos, normal, model_texture);
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
