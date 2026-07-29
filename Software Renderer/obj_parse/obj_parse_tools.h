
#include "obj_struct.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

const char* read_data_whole(const char* model_path);
int breaker_character(const char c, collecting_mode_t mode);
float *extract_vertex_data(mesh_collector_t *collector, const char* data, size_t file_size, collecting_mode_t mode);
mesh_collector_t mesh_collect_setup();
mesh_t* meshes_construct(mesh_collector_t mesh_collect);
mesh_collector_t extract_faces(mesh_collector_t collector, const char* data, size_t file_size);
mesh_t *extract_meshes(const char* model_path);
