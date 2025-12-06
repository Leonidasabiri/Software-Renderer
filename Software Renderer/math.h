#pragma once
typedef struct
{
    float x, y;
} vector2_t;

typedef struct
{
    float x, y, z, w;
} vector4_t;

typedef struct
{
    vector4_t x, y, z, w;
} matrix4_t;

matrix4_t multiply_matrix_matrix(matrix4_t matrix4_1, matrix4_t matrix4_2)
{
    float xx = matrix4_1.x.x * matrix4_2.x.x + matrix4_1.x.y * matrix4_2.y.x + matrix4_1.x.z * matrix4_2.z.x + matrix4_1.x.w * matrix4_2.w.x;
    float xy = matrix4_1.x.x * matrix4_2.x.y + matrix4_1.x.y * matrix4_2.y.y + matrix4_1.x.z * matrix4_2.z.y + matrix4_1.x.w * matrix4_2.w.y;
    float xz = matrix4_1.x.x * matrix4_2.x.z + matrix4_1.x.y * matrix4_2.y.z + matrix4_1.x.z * matrix4_2.z.z + matrix4_1.x.w * matrix4_2.w.z;
    float xw = matrix4_1.x.x * matrix4_2.x.w + matrix4_1.x.y * matrix4_2.y.w + matrix4_1.x.z * matrix4_2.z.w + matrix4_1.x.w * matrix4_2.w.w;

    float yx = matrix4_1.y.x * matrix4_2.x.x + matrix4_1.y.y * matrix4_2.y.x + matrix4_1.y.z * matrix4_2.z.x + matrix4_1.y.w * matrix4_2.w.x;
    float yy = matrix4_1.y.x * matrix4_2.x.y + matrix4_1.y.y * matrix4_2.y.y + matrix4_1.y.z * matrix4_2.z.y + matrix4_1.y.w * matrix4_2.w.y;
    float yz = matrix4_1.y.x * matrix4_2.x.z + matrix4_1.y.y * matrix4_2.y.z + matrix4_1.y.z * matrix4_2.z.z + matrix4_1.y.w * matrix4_2.w.z;
    float yw = matrix4_1.y.x * matrix4_2.x.w + matrix4_1.y.y * matrix4_2.y.w + matrix4_1.y.z * matrix4_2.z.w + matrix4_1.y.w * matrix4_2.w.w;

    float zx = matrix4_1.z.x * matrix4_2.x.x + matrix4_1.z.y * matrix4_2.y.x + matrix4_1.z.z * matrix4_2.z.x + matrix4_1.z.w * matrix4_2.w.x;
    float zy = matrix4_1.z.x * matrix4_2.x.y + matrix4_1.z.y * matrix4_2.y.y + matrix4_1.z.z * matrix4_2.z.y + matrix4_1.z.w * matrix4_2.w.y;
    float zz = matrix4_1.z.x * matrix4_2.x.z + matrix4_1.z.y * matrix4_2.y.z + matrix4_1.z.z * matrix4_2.z.z + matrix4_1.z.w * matrix4_2.w.z;
    float zw = matrix4_1.z.x * matrix4_2.x.w + matrix4_1.z.y * matrix4_2.y.x + matrix4_1.z.z * matrix4_2.z.w + matrix4_1.z.w * matrix4_2.w.w;

    float wx = matrix4_1.w.x * matrix4_2.x.x + matrix4_1.w.y * matrix4_2.y.x + matrix4_1.w.z * matrix4_2.z.x + matrix4_1.w.w * matrix4_2.w.x;
    float wy = matrix4_1.w.x * matrix4_2.x.y + matrix4_1.w.y * matrix4_2.y.y + matrix4_1.w.z * matrix4_2.z.y + matrix4_1.w.w * matrix4_2.w.y;
    float wz = matrix4_1.w.x * matrix4_2.x.z + matrix4_1.w.y * matrix4_2.y.z + matrix4_1.w.z * matrix4_2.z.z + matrix4_1.w.w * matrix4_2.w.z;
    float ww = matrix4_1.w.x * matrix4_2.x.w + matrix4_1.w.y * matrix4_2.y.w + matrix4_1.w.z * matrix4_2.z.w + matrix4_1.w.w * matrix4_2.w.w;

    vector4_t x = { xx, xy, xz, xw };
    vector4_t y = { yx, yy, yz, yw };
    vector4_t z = { zx, zy, zz, zw };
    vector4_t w = { wx, wy, wz, ww };

    return
    {
        x,
        y,
        z,
        w
    };
}

matrix4_t add_matrix_matrix(matrix4_t matrix4_1, matrix4_t matrix4_2)
{
    return
    {
       {matrix4_1.x.x + matrix4_2.x.x, matrix4_1.x.y + matrix4_2.x.y, matrix4_1.x.z + matrix4_2.x.z, matrix4_1.x.w + matrix4_2.x.w},
       {matrix4_1.y.x + matrix4_2.y.x, matrix4_1.y.y + matrix4_2.y.y, matrix4_1.y.z + matrix4_2.y.z, matrix4_1.y.w + matrix4_2.y.w},
       {matrix4_1.z.x + matrix4_2.z.x, matrix4_1.z.y + matrix4_2.z.y, matrix4_1.z.z + matrix4_2.z.z, matrix4_1.z.w + matrix4_2.z.w},
       {matrix4_1.w.x + matrix4_2.w.x, matrix4_1.w.y + matrix4_2.w.y, matrix4_1.w.z + matrix4_2.w.z, matrix4_1.w.w + matrix4_2.w.w}
    };
}

vector2_t sub_vector(vector2_t vector1, vector2_t vector2)
{
    return { vector1.x - vector2.x, vector1.y - vector2.y };
}

vector4_t sub_vector4(vector4_t vector1, vector4_t vector2)
{
    return { vector1.x - vector2.x,
             vector1.y - vector2.y,
             vector1.z - vector2.z,
             1.0 };
}

float dot_product(vector4_t vector1, vector4_t vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

vector4_t multiply_matrix_vector(matrix4_t matrix4_1, vector4_t vector)
{
    return
    {
       matrix4_1.x.x * vector.x + matrix4_1.x.y * vector.y + matrix4_1.x.z * vector.z + matrix4_1.x.w * vector.w,
       matrix4_1.y.x * vector.x + matrix4_1.y.y * vector.y + matrix4_1.y.z * vector.z + matrix4_1.y.w * vector.w,
       matrix4_1.z.x * vector.x + matrix4_1.z.y * vector.y + matrix4_1.z.z * vector.z + matrix4_1.z.w * vector.w,
       matrix4_1.w.x * vector.x + matrix4_1.w.y * vector.y + matrix4_1.w.z * vector.z + matrix4_1.w.w * vector.w
    };
}

matrix4_t matrix4x4_transpose(matrix4_t matrix4_1)
{
    return
    {
       {matrix4_1.x.x, matrix4_1.y.x, matrix4_1.z.x, matrix4_1.w.x},
       {matrix4_1.x.y, matrix4_1.y.y, matrix4_1.z.y, matrix4_1.w.y},
       {matrix4_1.x.z, matrix4_1.y.z, matrix4_1.z.z, matrix4_1.w.z},
       {matrix4_1.x.w, matrix4_1.y.w, matrix4_1.z.w, matrix4_1.w.w}
    };
}

float cross_product_2d(vector2_t vector1, vector2_t vector2)
{
    return vector1.x * vector2.y - vector1.y * vector2.x;
}

vector4_t cross_product_4d(vector4_t vector1, vector4_t vector2)
{
    return
    {
        vector1.y * vector2.z - vector1.z * vector2.y,
        vector1.z * vector2.x - vector1.x * vector2.z,
        vector1.x * vector2.y - vector1.y * vector2.x,
        1.0
    };
}

float rad_to_deg(float angle)
{
    return 3.14 / 180 * angle;
}

vector4_t normalize_vector(vector4_t vector)
{
    float length = sqrt(dot_product(vector, vector));

    return { vector.x / length,vector.y / length, vector.z / length, 1 };
}

float triangle_surface(vector2_t pos1, vector2_t pos2, vector2_t pos3)
{
    return cross_product_2d(sub_vector(pos1, pos2), sub_vector(pos3, pos2)) * 1 / 2;
}

