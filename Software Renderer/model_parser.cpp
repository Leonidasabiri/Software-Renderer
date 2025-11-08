
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
	float vertecies[9];
	float normals[9];
	float uvs[6];
}mesh_t;

int main()
{
	// read file here
	const char* file_path;

	// parse 
	const char c[] =
		"# these are the vertecies\n \
		v 0.32 0.541 0.121   #this is vertex 1\n\
		v 0.52 0.121 -0.621  #this is vertex 2\n\
		v -0.12 0.451 -0.921 #this is vertex 3\n\
		v 0.72 0.458 -0.921  #this is vertex 4\n\
		v 0.52 0.461 -0.91   #this is vertex 5\n\
		v 0.32 0.421 0.921   #this is vertex 6\n\
		vt 0.2 0.001         #this is vertex uv 1\n\
		vt 0.5 0.21          #this is vertex uv 2\n\
		vt -0.6 1.0          #this is vertex uv 3\n\
		vt 0.4 0.241         #this is vertex uv 1\n\
		vt 0.5 0.31          #this is vertex uv 2\n\
		vt 0.6 0.6           #this is vertex uv 3\n\
		vn 0.32 -0.131 0.131 #this is vertex normal 1\n\
		vn 0.42 0.631 0.131  #this is vertex normal 2\n\
		vn 0.12 0.831 0.131  #this is vertex normal 3\n\
		vn 0.72 0.31 0.231   #this is vertex normal 1\n\
		vn 0.62 0.671 0.531  #this is vertex normal 2\n\
		vn 0.52 0.131 0.831  #this is vertex normal 3\n\
		s off \n\
		f 2/1/1 0/0/2 1/2/0	  #this is mesh face\n";

	int vertex_numbers  = 0;
	int uv_numbers		= 0;
	int normals_numbers = 0;
	int faces_numbers	= 0;

	mesh_t meshes[2];

	int v_size = 9;
	int uv_size = 6;
	int normals_size = 9;
	int v_indecies_size = 3;
	int vt_indecies_size = 3;
	int vn_indecies_size = 3;

	float *vertecies = (float*)malloc(sizeof(float) * v_size);
	float *uvs		 = (float*)malloc(sizeof(float) * uv_size);
	float *normals	 = (float*)malloc(sizeof(float) * normals_size);

	int* v_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int* vt_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int* vn_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int v_indecies_index = 0;
	int vt_indecies_index = 0;
	int vn_indecies_index = 0;

	int realloc_v = 0;
	int realloc_uv = 0;
	int realloc_normal = 0;

	static int index_v = 0;
	static int index_uv = 0;
	static int index_normal = 0;
	static int index_face = 0;

	static int mesh_v = 0;
	static int mesh_uv = 0;
	static int mesh_normal = 0;
	for (int i = 0 ; i < sizeof(c); i++)
	{

		switch (c[i])
		{
			case '#': // comment
				while (c[i - 1] != '\n' && i < sizeof(c))
					i++;
				break;
			case 'v':	// vertex
				if (i + 1 < sizeof(c))
				{
					switch (c[i + 1])
					{
						case 't': // vertex uv
							uv_numbers++;
							if (realloc_uv)
							{
								uv_size += 6;
								uvs = (float*)realloc(uvs, sizeof(float) * uv_size);
								realloc_uv = 0;
							}
							while (c[i + 1] != '\n' && c[i + 1] != '#' && i < sizeof(c))
							{
								float v1 = 0.0;
								float vf = 0.0;
								int nega = 1;
								if (c[i] == '-') nega = -1, i++;
								if (c[i] >= '0' && c[i] <= '9')
								{
									while (c[i] != '\n' && c[i] != '.' &&
										c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
										v1 = v1 * 10 + (c[i] - 48), i++;
									int mark = i;
									while (c[i] != '\n' &&
										c[i] != '#' &&
										!isspace(c[i]) &&
										i < sizeof(c))
									{
										if (c[i] >= '0' && c[i] <= '9')
											vf += (c[i] - 48) / powf(10, i - mark);
										i++;
									}
									uvs[index_uv++] = (v1 + vf) * nega;
									if (index_uv % 6 == 0)
									{
										realloc_uv = 1;
										break;
									}
								}
								i++;
							}
							break;
						case 'n': // vertex normal
							normals_numbers++;
							if (realloc_normal)
							{
								normals_size += 9;
								normals = (float*)realloc(normals, sizeof(float) * normals_size);
								realloc_normal = 0;
							}
							while (c[i + 1] != '\n' && c[i + 1] != '#' && i < sizeof(c))
							{
								float v1 = 0.0;
								float vf = 0.0;
								int nega = 1;
								if (c[i] == '-') nega = -1, i++;
								if (c[i] >= '0' && c[i] <= '9')
								{
									while (c[i] != '\n' && c[i] != '.' &&
										c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
										v1 = v1 * 10 + (c[i] - 48), i++;
									int mark = i;
									while (c[i] != '\n' &&
										c[i] != '#' &&
										!isspace(c[i]) &&
										i < sizeof(c))
									{
										if (c[i] >= '0' && c[i] <= '9')
											vf += (c[i] - 48) / powf(10, i - mark);
										i++;
									}
									normals[index_normal++] = (v1 + vf) * nega;
									if (index_normal % 9 == 0)
									{
										realloc_normal = 1;
										break;
									}
								}
								i++;
							}
							break;
						case ' ': // vertex in itself
							vertex_numbers++;
							if (realloc_v)
							{
								v_size += 9;
								vertecies = (float*)realloc(vertecies, sizeof(float) * v_size);
								realloc_v = 0;
							}
							while (c[i + 1] != '\n' && c[i + 1] != '#' && i < sizeof(c))
							{
								float v1 = 0.0;
								float vf = 0.0;
								int nega = 1;
								if (c[i] == '-') nega = -1, i++;
								if (c[i] >= '0' && c[i] <= '9')
								{
									while (c[i] != '\n' && c[i] != '.' &&
											c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
										v1 = v1 * 10 + (c[i] - 48), i++;
									int mark = i;
									while (c[i] != '\n' &&
										c[i] != '#' &&
										!isspace(c[i]) &&
										i < sizeof(c))
									{
										if (c[i] >= '0' && c[i] <= '9')
											vf += (c[i] - 48)/powf(10, i - mark);
										i++;
									}
									vertecies[index_v++] = (v1 + vf) * nega;
									if (index_v % 9 == 0)
									{
										realloc_v = 1;
										break;
									}
								}
								i++;
							}
							break;
						default:
							break;
					}
				}
				break;
			case 's':		// smooth shading on/off
				break;
			case 'f':		// mesh faces
				while (c[i + 1] != '\n' && c[i + 1] != '#')
				{
					int v = 0, vt = 0, vn = 0, slash_number = 0;
					if (c[i] >= '0' && c[i] <= '9')
					{
						while (c[i] != '\n' && c[i] != '/' &&
							c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
						{
							v = v * 10 + (c[i] - 48);
							i++;
						}
						i++;
						while (c[i] != '\n' && c[i] != '/' &&
							c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
						{
							if (c[i] >= '0' && c[i] <= '9')
								vt = vt * 10 + (c[i] - 48);
							i++;
						}
						i++;
						while (c[i] != '\n' && c[i] != '/' &&
							c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
						{
							if (c[i] >= '0' && c[i] <= '9')
								vn = vn * 10 + (c[i] - 48);
							i++;
						}
						v_indecies[v_indecies_index++] = v;
						vt_indecies[vt_indecies_index++] = vt;
						vn_indecies[vn_indecies_index++] = vn;
					}
					i++;
				}
				break;
			default:
				break;
		}
	}

	for (int i = 0 ; i < v_indecies_index; i++)
		printf("vertex x: %f vertex y: %f vertex z: %f \n",  vertecies[v_indecies[i] * 3 + 0], vertecies[v_indecies[i] * 3 + 1], vertecies[v_indecies[i] * 3 + 2]);

	for (int i = 0; i < vt_indecies_index; i++)
		printf("uv x: %f uv y: %f \n", uvs[vt_indecies[i] * 2 + 0], uvs[vt_indecies[i] * 2 + 1]);

	for (int i = 0; i < vn_indecies_index; i++)
		printf("normal x: %f normal y: %f normal z: %f\n",
			normals[vn_indecies[i] * 3 + 0],
			normals[vn_indecies[i] * 3 + 1],
			normals[vn_indecies[i] * 3 + 2]);

	printf("vertecies numbers: %d\n", vertex_numbers);
	printf("uvs numbers: %d\n", uv_numbers);
	printf("normals numbers:%d\n", normals_numbers);


}
