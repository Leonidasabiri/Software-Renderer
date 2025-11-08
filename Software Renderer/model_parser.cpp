
#include <stdio.h>
#include <ctype.h>
#include <math.h>

typedef struct
{
	float vertecies[9];
	float normals[9];
	float uvs[6];
	float indecies[3];
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
		v 0.72 0.458 -0.921 #this is vertex 4\n\
		v 0.52 0.461 -0.91 #this is vertex 5\n\
		v 0.32 0.421 0.921 #this is vertex 6\n\
		vt 0.2 0.001         #this is vertex uv 1\n\
		vt 0.5 0.21          #this is vertex uv 2\n\
		vt -0.6 1.0          #this is vertex uv 3\n\
		vt 0.4 0.241         #this is vertex uv 1\n\
		vt 0.5 0.31          #this is vertex uv 2\n\
		vt 0.6 0.6          #this is vertex uv 3\n\
		vn 0.32 -0.131 0.131 #this is vertex normal 1\n\
		vn 0.42 0.631 0.131  #this is vertex normal 2\n\
		vn 0.12 0.831 0.131  #this is vertex normal 3\n\
		vn 0.72 0.31 0.231 #this is vertex normal 1\n\
		vn 0.62 0.671 0.531  #this is vertex normal 2\n\
		vn 0.52 0.131 0.831  #this is vertex normal 3\n\
		s off \n\
		f 1/1/1 2/4/5 3/2/1	  #this is mesh face\n";

	int vertex_numbers  = 0;
	int uv_numbers		= 0;
	int normals_numbers = 0;
	int faces_numbers	= 0;

	for (int i = 0 ; i < sizeof(c); i++)
	{
		static int index_v = 0;
		static int index_uv = 0;
		static int index_normal = 0;
		static int index_face = 0;

		float vertecies[9];
		float uvs[6];
		float normals[9];

		switch (c[i])
		{
			case '#':	// comment
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
										printf("uv 1 : x: %f, y: %f\n", uvs[0], uvs[1]);
										printf("uv 2 : x: %f, y: %f\n", uvs[2], uvs[3]);
										printf("uv 3 : x: %f, y: %f\n", uvs[4], uvs[5]);
										index_uv = 0;
										break;
									}
								}
								i++;
							}
							break;
						case 'n': // vertex normal
							normals_numbers++;
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
										printf("normal 1 : x: %f, y: %f, z: %f\n", normals[0], normals[1], normals[2]);
										printf("normal 2 : x: %f, y: %f, z: %f\n", normals[3], normals[4], normals[5]);
										printf("normal 3 : x: %f, y: %f, z: %f\n", normals[6], normals[7], normals[8]);
										index_normal = 0;
										break;
									}
								}
								i++;
							}
							break;
						case ' ': // vertex in itself
							vertex_numbers++;
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
										printf("vertex 1 : x: %f, y: %f , z: %f\n", vertecies[0], vertecies[1], vertecies[2]);
										printf("vertex 2 : x: %f, y: %f , z: %f\n", vertecies[3], vertecies[4], vertecies[5]);
										printf("vertex 3 : x: %f, y: %f , z: %f\n", vertecies[6], vertecies[7], vertecies[8]);
										index_v = 0;
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

				break;
			default:
				break;
		}

	}

	printf("vertecies numbers: %d\n", vertex_numbers);
	printf("uvs numbers: %d\n", uv_numbers);
	printf("normals numbers:%d\n", normals_numbers);
}
