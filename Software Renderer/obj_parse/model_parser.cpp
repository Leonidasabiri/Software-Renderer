
#pragma once
#include "obj_parse_tools.h"

mesh_t *extract_meshes(const char* model_path)
{
	const char *data = read_data_whole(model_path);
	size_t file_size = strlen(data);

	mesh_collector_t mesh_collect = mesh_collect_setup();

	for ( ; mesh_collect.iterator < file_size; mesh_collect.iterator++)
	{
		switch (data[mesh_collect.iterator])
		{
			case '#': // comment
				while (data[mesh_collect.iterator] != '\n' && mesh_collect.iterator < file_size) 
					mesh_collect.iterator++;
				break;
			case 'v': // vertex
				switch (data[mesh_collect.iterator + 1])
				{
					case ' ': // vertex in itself
						mesh_collect.vertecies = extract_vertex_data(&mesh_collect, data, file_size, VERTEX);
						break;
					case 't': // vertex uvs
						mesh_collect.uvs = extract_vertex_data(&mesh_collect, data, file_size, UV);
						break;
					case 'n': // vertex normal
						mesh_collect.iterator += 2;
						break;
					default:
						break;
				}
				break;

			case 's':	// smooth shading on/off
				while (data[mesh_collect.iterator] != '\n' && data[mesh_collect.iterator + 1] != '#' && mesh_collect.iterator < file_size)
				{
					mesh_collect.iterator++;
				}
				break ;
			case 'f':
				mesh_collect = extract_faces(mesh_collect, data, file_size);
			default:
				break;
		}
	}

	mesh_t *meshes = meshes_construct(mesh_collect);

	free((char*)data);
	free(mesh_collect.vertecies);
	free(mesh_collect.uvs);
	free(mesh_collect.vt_indecies);
	free(mesh_collect.v_indecies);

	return meshes;
}
