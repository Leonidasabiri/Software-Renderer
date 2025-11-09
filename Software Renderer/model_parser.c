
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
	FILE* file_path = fopen("utah.obj", "rb");

	if (!file_path)
		return 1;

	fseek(file_path, 0, SEEK_END);
	int file_size = ftell(file_path);
	char* c = (char*)malloc((file_size + 1));
	c[file_size] = '\0';

	fseek(file_path, 0, SEEK_SET);

	fread(c, 1, file_size, file_path);

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

	for (int i = 0 ; i < file_size; i++)
	{
		switch (c[i])
		{
			case '#': // comment
				while (c[i - 1] != '\n' && i < sizeof(c))
					i++;
				break;
			case 'v': // vertex
				//if (i + 1 < sizeof(c))
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
									//uvs[index_uv++] = (v1 + vf) * nega;
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
									//normals[index_normal++] = (v1 + vf) * nega;
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
									//vertecies[index_v++] = (v1 + vf) * nega;
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
			case 's':	// smooth shading on/off
				break;
			case 'f':	// mesh faces
				faces_numbers++;
				if (i + 1 < sizeof(c))
				while (c[i + 1] != '\n' && c[i + 1] != '#')
				{
					int v = 0, vt = 0, vn = 0, slash_number = 0;
					if (v_indecies_index % 3 == 0)
						v_indecies = (int*)realloc(v_indecies, (v_indecies_index + 3) * sizeof(int));
					if (vt_indecies_index % 3 == 0)
						vt_indecies = (int*)realloc(vt_indecies, (vt_indecies_index + 3) * sizeof(int));
					if (vn_indecies_index % 3 == 0)
						vn_indecies = (int*)realloc(vn_indecies, (vn_indecies_index + 3) * sizeof(int));

					if (c[i] >= '0' && c[i] <= '9')
					{
						while (c[i] != '\n' && c[i] != '/' &&
							c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
						{
							v = v * 10 + (c[i] - 48);
							i++;
						}
						if (c[i] == '/')
						{
							i++;
							while (c[i] != '\n' && c[i] != '/' &&
								c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
							{
								if (c[i] >= '0' && c[i] <= '9')
									vt = vt * 10 + (c[i] - 48);
								i++;
							}
							vt_indecies[vt_indecies_index++] = vt;
						}
						if (c[i] == '/')
						{
							i++;
							while (c[i] != '\n' && c[i] != '/' &&
								c[i] != '#' && !isspace(c[i]) && i < sizeof(c))
							{
								if (c[i] >= '0' && c[i] <= '9')
									vn = vn * 10 + (c[i] - 48);
								i++;
							}
							vn_indecies[vn_indecies_index++] = vn;
						}
						v_indecies[v_indecies_index++] = v;
					}
					i++;
				}
				break;
			default:
				break;
		}
	}

	//for (int i = 0 ; i < v_indecies_index; i++)
	//	printf("vertex x: %f vertex y: %f vertex z: %f \n",  vertecies[v_indecies[i] * 3 + 0], 
	//														 vertecies[v_indecies[i] * 3 + 1], vertecies[v_indecies[i] * 3 + 2]);
	//for (int i = 0; i < vt_indecies_index; i++)
	//	printf("uv x: %f uv y: %f \n", uvs[vt_indecies[i] * 2 + 0], uvs[vt_indecies[i] * 2 + 1]);
	//for (int i = 0; i < vn_indecies_index; i++)
	//	printf("normal x: %f normal y: %f normal z: %f\n",
	//		normals[vn_indecies[i] * 3 + 0],
	//		normals[vn_indecies[i] * 3 + 1],
	//		normals[vn_indecies[i] * 3 + 2]);

	printf("vertecies numbers: %d\n", vertex_numbers);
	printf("uvs numbers: %d\n", uv_numbers);
	printf("normals numbers:%d\n", normals_numbers);
	printf("faces numbers:%d\n", faces_numbers);
}

