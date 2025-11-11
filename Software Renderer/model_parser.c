
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

int vertex_numbers = 0;
int faces_numbers = 0;

mesh_t *extract_meshes(const char* model_path)
{
	FILE* file_path;
	
	fopen_s(&file_path, model_path, "rb");

	fseek(file_path, 0, SEEK_END);
	int file_size = ftell(file_path);
	char* c = (char*)malloc((file_size + 1));
	c[file_size] = '\0';

	fseek(file_path, 0, SEEK_SET);

	fread(c, 1, file_size, file_path);

	int uv_numbers = 0;
	int normals_numbers = 0;

	int v_size = 3;
	int uv_size = 2;
	int normals_size = 3;
	int v_indecies_size = 3;
	int vt_indecies_size = 3;
	int vn_indecies_size = 3;

	float* vertecies = (float*)malloc(sizeof(float) * v_size);
	float* uvs = (float*)malloc(sizeof(float) * uv_size);
	float* normals = (float*)malloc(sizeof(float) * normals_size);

	int* v_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int* vt_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int* vn_indecies = (int*)malloc(sizeof(int) * v_indecies_size);
	int v_indecies_index = 0;
	int vt_indecies_index = 0;
	int vn_indecies_index = 0;

	static int index_v = 0;
	static int index_uv = 0;
	static int index_normal = 0;
	static int index_face = 0;

	static int mesh_v = 0;
	static int mesh_uv = 0;
	static int mesh_normal = 0;

	for (int i = 0; i < file_size; i++)
	{
		switch (c[i])
		{
			case '#': // comment
				while (c[i] != '\n' && i < file_size) i++;
				break;
			case 'v': // vertex
				switch (c[i + 1])
				{
					case ' ': // vertex in itself
						i += 2;
						while (c[i] != '\n' && c[i] != '#' && i < file_size - 1)
						{
							float v1 = 0.0;
							float vf = 0.0;
							int nega = 1;
							if (c[i] == '-') nega = -1, i++;
							if (c[i] >= '0' && c[i] <= '9')
							{
								while (c[i] != '\n' && c[i] != '.' &&
									c[i] != '#' && !isspace(c[i]) && i < file_size)
									v1 = v1 * 10 + (c[i] - 48), i++;
								int mark = i;
								i++;
								while (c[i] != '\n' &&
									c[i] != '#' &&
									!isspace(c[i]) &&
									i < file_size)
								{
									if (c[i] >= '0' && c[i] <= '9')
										vf += (c[i] - 48) / powf(10, i - mark);
									i++;
								}
								vertecies[index_v++] = (v1 + vf) * nega;
								if (index_v > 0 && index_v % 3 == 0)
								{
									v_size += 3;
									vertecies = (float*)realloc(vertecies, sizeof(float) * v_size);
								}
							}
							if (c[i] == '\n') break;
							i++;
						}
						break;
					case 't':
						i += 2;
						while (c[i] != '\n')i++;
						break;
					case 'n':
						i += 2;
						while (c[i] != '\n')i++;
						break;
					default:
						break;
				}
				break;

			case 's':	// smooth shading on/off
				while (c[i] != '\n' && c[i + 1] != '#') i++;
				break ;
			case 'f':
				{
					i += 2;
					int v1 = 0;
					int slash = 0;
					int last_space = 0;
					while (c[i] != '\n' && c[i + 1] != '#' && i < file_size )
					{
						last_space = 0;
						if (c[i] >= '0' && c[i] <= '9')
							v1 = v1 * 10 + (c[i] - 48);
						if (isspace(c[i]) || c[i] == '/')
						{
							if (c[i] == '/' && slash % 2 == 0)
								v_indecies[v_indecies_index++] = v1;
							if (isspace(c[i]) && slash <= 0)
								v_indecies[v_indecies_index++] = v1;
							if (c[i] == '/') slash++;
							v1 = 0;
							last_space = 1;
						}
						if (slash % 2 == 0 && v_indecies_index > 0 && v_indecies_index % 3 == 0)
						{
							v_indecies_size += 3;
							v_indecies = (int*)realloc(v_indecies, sizeof(int) * v_indecies_size);
						}
						i++;
					}
					if (slash <= 0 && !last_space) 	v_indecies[v_indecies_index++] = v1;
					break;
				}
			default:
				break;
		}
	}

	mesh_t* meshes = (mesh_t*)malloc(sizeof(mesh_t) * v_indecies_index/3);

	int ind = 0;

	faces_numbers = v_indecies_index / 3;

	//printf("%d\n", faces_numbers);

	//for (int i = 0; i < v_indecies_index; i += 3)
	//{
	//	printf("%d %d %d\n", v_indecies[i], v_indecies[i + 1], v_indecies[i + 2]);
	//}

	//for (int i = 0; i < index_v; i += 3)
	//{
	//	printf("%f %f %f\n", vertecies[i], vertecies[i + 1], vertecies[i + 2]);
	//}

	int i = 0;
	for (int vn = 0; vn < v_indecies_index; vn += 3)
	{
		printf("%d %d %d\n", v_indecies[vn + 0], v_indecies[vn + 1], v_indecies[vn + 2]);
		//continue;
		// v1
		meshes[i].vertecies[0] = vertecies[(v_indecies[vn + 0] - 1) * 3 + 0];
		meshes[i].vertecies[1] = vertecies[(v_indecies[vn + 0] - 1) * 3 + 1];
		meshes[i].vertecies[2] = vertecies[(v_indecies[vn + 0] - 1) * 3 + 2];

		//// v2
		meshes[i].vertecies[3] = vertecies[(v_indecies[vn + 1] - 1) * 3 + 0];
		meshes[i].vertecies[4] = vertecies[(v_indecies[vn + 1] - 1) * 3 + 1];
		meshes[i].vertecies[5] = vertecies[(v_indecies[vn + 1] - 1) * 3 + 2];

		//// v 3
		meshes[i].vertecies[6] = vertecies[(v_indecies[vn + 2] - 1) * 3 + 0];
		meshes[i].vertecies[7] = vertecies[(v_indecies[vn + 2] - 1) * 3 + 1];
		meshes[i].vertecies[8] = vertecies[(v_indecies[vn + 2] - 1) * 3 + 2];

		//printf("%d %d %d\n", v_indecies[vn + 0], v_indecies[vn + 1], v_indecies[vn + 2]);
		i++;
	}
	//printf("meshes: %d\n", index_v);

	for (int ii = 0; ii < i; ii++)
	{
	//	//continue;
	//	// v1
		printf("%f ",  meshes[ii].vertecies[0]);
		printf("%f ",  meshes[ii].vertecies[1]);
		printf("%f\n", meshes[ii].vertecies[2]);

	//	//// v2
		printf("%f ",  meshes[ii].vertecies[3]);
		printf("%f ",  meshes[ii].vertecies[4]);
		printf("%f\n", meshes[ii].vertecies[5]);

	//	//// v 3
		printf("%f ",  meshes[ii].vertecies[6]);
		printf("%f ",  meshes[ii].vertecies[7]);
		printf("%f\n", meshes[ii].vertecies[8]);
	}

	return meshes;
}

