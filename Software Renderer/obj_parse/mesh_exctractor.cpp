
#include "obj_parse_tools.h"

const char* read_data_whole(const char* model_path)
{
	FILE* file_path;

	#if defined (_WIN32)
		fopen_s(&file_path, model_path, "rb");
	#elif defined (__linux__)
		fopen(&file_path, model_path, "r");
	#endif

	if (!file_path)
	{
        assert("error");
		return 0;
	}

	fseek(file_path, 0, SEEK_END);
	int file_size = ftell(file_path);

	if (file_size == -1)
	{
		assert("error");
	}

	char* data = (char*)malloc((file_size + 1));
	data[file_size] = '\0';
	fseek(file_path, 0, SEEK_SET);
	fread(data, 1, file_size, file_path);

	fclose(file_path);

	return data;
}

int breaker_character(const char c, collecting_mode_t mode)
{
	const char custom_breakers[3] = {'\n', '#', '.'};

	switch (mode)
	{
		case FLOAT_LEFT_SIDE:
			return c != custom_breakers[0] && c != custom_breakers[1] && c != custom_breakers[2] && !isspace(c);
		case FLOAT_RIGHT_SIDE:
			return c != custom_breakers[0] && c != custom_breakers[1] && !isspace(c);
		case FACE:
			break;
		default:
			break;
	}
	return 0;
}

float *extract_vertex_data(mesh_collector_t *collector, const char* data, size_t file_size, collecting_mode_t mode)
{
	collector->iterator += 2;
	while (data[collector->iterator] != '\n' && data[collector->iterator] != '#' && collector->iterator < file_size - 1)
	{
		float v1 = 0.0;
		float vf = 0.0;
		int nega = 1;
		if (data[collector->iterator] == '-')
		{
			nega = -1;
			collector->iterator++;
		}
		if (data[collector->iterator] >= '0' && data[collector->iterator] <= '9')
		{
			while (breaker_character(data[collector->iterator], FLOAT_LEFT_SIDE) && collector->iterator < file_size)
			{
				v1 = v1 * 10 + (data[collector->iterator] - 48);
				collector->iterator++;
			}
			int mark = collector->iterator;
			collector->iterator++;
			while (breaker_character(data[collector->iterator], FLOAT_RIGHT_SIDE) && collector->iterator < file_size)
			{
				if (data[collector->iterator] >= '0' && data[collector->iterator] <= '9')
					vf += (data[collector->iterator] - 48) / powf(10, collector->iterator - mark);
				collector->iterator++;
			}

			switch (mode)
			{
				case VERTEX:
					collector->vertecies[collector->index_v++] = (v1 + vf) * nega;
					if (collector->index_v > 0 && collector->index_v % 3 == 0)
					{
						collector->v_size += 3;
						collector->vertecies = (float*)realloc(collector->vertecies, sizeof(float) * collector->v_size);
					}
					break;				
				case UV:
					collector->uvs[collector->index_uv++] = (v1 + vf) * nega;
					if (collector->index_uv > 0 && collector->index_uv % 2 == 0)
					{
						collector->uv_size += 2;
						collector->uvs = (float*)realloc(collector->uvs, sizeof(float) * collector->uv_size);
					}
					break;
				default:
					break;
			}
		}
		if (data[collector->iterator] == '\n') 
			break;

		collector->iterator++;
	}
	if (mode == VERTEX)
		return collector->vertecies;

	return collector->uvs;
}

mesh_collector_t mesh_collect_setup()
{
	mesh_collector_t mesh_collect;

	mesh_collect.v_size = 3;
	mesh_collect.uv_size = 2;
	mesh_collect.normals_size = 3;

	mesh_collect.v_indecies_size = 3;
	mesh_collect.vt_indecies_size = 3;
	mesh_collect.vn_indecies_size = 3;

	mesh_collect.vertecies = (float*)malloc(sizeof(float) * mesh_collect.v_size);
	mesh_collect.uvs = (float*)malloc(sizeof(float) * mesh_collect.uv_size);
	mesh_collect.normals = (float*)malloc(sizeof(float) * mesh_collect.normals_size);

	mesh_collect.v_indecies  = (int*)malloc(sizeof(int) * mesh_collect.v_indecies_size);
	mesh_collect.vt_indecies = (int*)malloc(sizeof(int) * mesh_collect.vt_indecies_size);
	mesh_collect.vn_indecies = (int*)malloc(sizeof(int) * mesh_collect.vn_indecies_size);

	mesh_collect.v_indecies_index = 0;
	mesh_collect.vt_indecies_index = 0;
	mesh_collect.vn_indecies_index = 0;

	mesh_collect.index_v = 0;
	mesh_collect.index_uv = 0;
	mesh_collect.index_normal = 0;
	mesh_collect.index_face = 0;

	mesh_collect.mesh_v = 0;
	mesh_collect.mesh_uv = 0;
	mesh_collect.mesh_normal = 0;

	mesh_collect.iterator = 0;

	return mesh_collect;
}

mesh_t* meshes_construct(mesh_collector_t mesh_collect)
{
	mesh_t* meshes = (mesh_t*)malloc(sizeof(mesh_t) * mesh_collect.v_indecies_index/3);
	meshes->faces_numbers = mesh_collect.v_indecies_index/3;

	int i = 0;
	for (int vn = 0; vn < mesh_collect.v_indecies_index; vn += 3)
	{
		// v1
		meshes[i].vertecies[0] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 0] - 1) * 3 + 0];
		meshes[i].vertecies[1] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 0] - 1) * 3 + 1];
		meshes[i].vertecies[2] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 0] - 1) * 3 + 2];
		meshes[i].vertecies[3] = 1;	// w

		//// v2
		meshes[i].vertecies[4] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 1] - 1) * 3 + 0];
		meshes[i].vertecies[5] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 1] - 1) * 3 + 1];
		meshes[i].vertecies[6] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 1] - 1) * 3 + 2];
		meshes[i].vertecies[7] = 1;	// w

		//// v 3
		meshes[i].vertecies[8]  = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 2] - 1) * 3 + 0];
		meshes[i].vertecies[9]  = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 2] - 1) * 3 + 1];
		meshes[i].vertecies[10] = mesh_collect.vertecies[(mesh_collect.v_indecies[vn + 2] - 1) * 3 + 2];
		meshes[i].vertecies[11] = 1; // w

		i++;
	}
	i = 0;
	for (int vn = 0; vn < mesh_collect.vt_indecies_index; vn += 3)
	{
		// v1
		meshes[i].uvs[0] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 0] - 1) * 2 + 0];
		meshes[i].uvs[1] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 0] - 1) * 2 + 1];

		// v2
		meshes[i].uvs[2] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 1] - 1) * 2 + 0];
		meshes[i].uvs[3] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 1] - 1) * 2 + 1];

		// v3
		meshes[i].uvs[4] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 2] - 1) * 2 + 0];
		meshes[i].uvs[5] = mesh_collect.uvs[(mesh_collect.vt_indecies[vn + 2] - 1) * 2 + 1];

		i++;
	}
	return meshes;
}

mesh_collector_t extract_faces(mesh_collector_t collector, const char* data, size_t file_size)
{
    collector.iterator += 2;

    while (data[collector.iterator] != '\n' && data[collector.iterator + 1] != '#' && collector.iterator < file_size - 1)
    {
        int v_index  = 0;
        int vt_index = 0;
        int vn_index = 0;
        while (data[collector.iterator] != '/')
        {
            v_index = (data[collector.iterator] - 48) + v_index * 10;
            collector.iterator++;
        }
        if (data[collector.iterator] == '/')
            collector.iterator++;
        while (data[collector.iterator] != '/')
        {
            vt_index = (data[collector.iterator] - 48) + vt_index * 10;
            collector.iterator++;
        }
        if (data[collector.iterator] == '/')
            collector.iterator++;
        while (data[collector.iterator] && data[collector.iterator] != ' ' && !isspace(data[collector.iterator]) && data[collector.iterator] != '\n' && data[collector.iterator] != '#')
        {
            vn_index = (data[collector.iterator] - 48) + vn_index * 10;
            collector.iterator++;
        }

        if (v_index > 0)
        {
            collector.v_indecies[collector.v_indecies_index++] = v_index;
			collector.v_indecies_size++;
            collector.v_indecies = (int*)realloc(collector.v_indecies, sizeof(int) * collector.v_indecies_size);
        }

        if (vt_index > 0)
        {
            collector.vt_indecies[collector.vt_indecies_index++] = vt_index;
			collector.vt_indecies_size++;
            collector.vt_indecies = (int*)realloc(collector.vt_indecies, sizeof(int) * collector.vt_indecies_size);
        }

        if (vn_index > 0)
        {
            collector.vn_indecies[collector.vn_indecies_index++] = vn_index;
			collector.vn_indecies_size++;
            collector.vn_indecies = (int*)realloc(collector.vn_indecies, sizeof(int) *collector.vn_indecies_size);
        }

        if (data[collector.iterator] == '\n')
            break;

        collector.iterator++;
    }

    return collector;
}
