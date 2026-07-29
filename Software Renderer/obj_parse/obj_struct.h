

typedef struct
{
	float vertecies[12];
	float uvs[6];
	float normals[9];
	int vertex_numbers;
	int faces_numbers;
}mesh_t;

typedef struct
{
	float* vertecies;
	float* uvs;
	float* normals;

	int* v_indecies;
	int* vt_indecies;
	int* vn_indecies;

	int v_size;
	int uv_size;
	int normals_size;

	int v_indecies_size;
	int vt_indecies_size;
	int vn_indecies_size;

	int vt_indecies_index;
	int v_indecies_index;
	int vn_indecies_index;

	int index_v;
	int index_uv;
	int index_normal;
	int index_face;

	int mesh_v;
	int mesh_uv;
	int mesh_normal;

	int	iterator;
}mesh_collector_t;

typedef enum
{
	VERTEX,
	FLOAT_LEFT_SIDE,
	FLOAT_RIGHT_SIDE,
	UV,
	FACE
}collecting_mode_t;
