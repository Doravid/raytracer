#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>

#include "raylib.h"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define NUM_THREADS (8)

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#define UNUSED(x) ((void)x)

#define EPSILON (0.0001)
#define MAX_INTERSECTIONS 128

typedef struct
{
    alignas(32) float m[16];
} Matrix4x4;
typedef struct
{
    float r, g, b;
} PackedColor;

typedef struct tuple
{
    float x, y, z, w;
} __attribute__((aligned(16))) Tuple;

typedef struct canvas
{
    PackedColor *pixels;
    int width;
    int height;
} Canvas;

struct ray
{
    Tuple position;
    Tuple direction;
};
struct material
{
    Tuple color;
    float ambient, diffuse, specular, shininess, reflective;
};
typedef struct sphere
{
    Tuple position;
    float radius;
    float transform[4 * 4];
    float inverse_transform[4 * 4];
    float transpose[4 * 4];
    struct material material;
} Sphere;
typedef struct plane
{
    float transform[4 * 4];
    float inverse_transform[4 * 4];
    float transpose[4 * 4];
    struct material material;
} Plane;

typedef enum
{
    ObjectSphere,
    ObjectPlane,
} PrimitiveType;

typedef struct
{
    PrimitiveType primitiveType;
    union
    {
        Sphere s;
        Plane pl;
    } primitive;
} Primitive;

Primitive sphere_1 = {.primitiveType = ObjectSphere, .primitive.s = {}};

typedef struct intersection_tuple
{
    int count;
    float distances[2];
} Intersection_tuple;

typedef struct intersection
{
    float time;
    Primitive *object;
} Intersection;

typedef struct intersections
{
    int count;
    Intersection intersections[MAX_INTERSECTIONS];
} Intersections;

typedef struct point_light
{
    Tuple intensity;
    Tuple position;
} PointLight;

typedef struct world
{
    Primitive *objects;
    int num_objects;

    PointLight light;
} world_t;

typedef struct computation
{
    Primitive *object;
    float time;
    Tuple point, eyev, normalv, reflectv;
    bool inside;
} Computation;

struct camera
{
    int hsize, vsize;
    float field_of_view, pixel_size, half_width, half_height;
    float inverse_transform[16];
    float transform[16];
};

struct material
material(Tuple color, float ambient, float diffuse, float specular, float shininess);
Tuple point(float x, float y, float z);
Tuple vector(float x, float y, float z);
bool equal(float x, float y);
bool tuple_equal(Tuple a, Tuple b);
Tuple tuple_add(Tuple a, Tuple b);
Tuple tuple_sub(Tuple a, Tuple b);
Tuple tuple_negate(Tuple a);
Tuple tuple(float x, float y, float z, float w);
Tuple tuple_scale(Tuple a, float scaler);
Tuple tuple_divide(Tuple a, float divisor);
float tuple_magnitude(Tuple a);
Tuple tuple_normalize(Tuple a);
float tuple_dot(Tuple a, Tuple b);
Tuple tuple_cross(Tuple a, Tuple b);
Canvas new_canvas(int width, int height);
Tuple color(float red, float green, float blue);
void canvas_to_ppm(Canvas *can, char *file_name);
void mat_identity_4x4(float *out);
bool mat_equal(int size, float *mat_a, float *mat_b);
void mat_mult(int size, float *mat_a, float *mat_b, float *mat_out);
void mat_transpose(int size, float *mat_a, float *mat_out);
void submatrix(int size, float *mat_a, float *mat_out, int remove_row, int remove_col);
float mat_det(int size, float *mat_a);
float mat_minor(int size, float *mat, int row, int col);
float mat_cofactor(int size, float *mat, int row, int col);
void mat_inverse(int size, float *mat, float *out_mat);
Tuple mat_tuple_mult(float *mat, Tuple tup);
void mat_translate(float x, float y, float z, float *out);
void mat_rotate_x(float r, float *out);
void set_transform(Primitive *object, float *m);
void mat_rotate_y(float r, float *out);
void mat_rotate_z(float r, float *out);
void mat_scale(float x, float y, float z, float *out);
void mat_sheer(float a, float b, float c, float d, float e, float f, float *out);
Tuple ray_position(struct ray r, double time);
struct ray ray_transform(struct ray r, float *m);
Sphere sphere();
Primitive sphere_primitive();
Plane plane();
Primitive plane_primitive();
void intersect(Primitive *s, struct ray r1, Intersections *inters);
Intersection intersection(float time, Primitive *object);
Intersections intersections(int count, ...);
Intersection hit(Intersections inters);
void set_sphere_transform(Sphere *object, float *m);
void set_camera_transform(struct camera *object, float *m);
Tuple sphere_normal_at(Sphere s, Tuple p);
Tuple plane_normal_at(Plane s, Tuple p);
Tuple reflect(Tuple incoming, Tuple normal);
bool materials_equal(struct material mat1, struct material mat2);
Tuple lighting(struct material material, PointLight light, Tuple position, Tuple eye_vector, Tuple normal_vector, bool in_shadow);
PointLight point_light(Tuple position, Tuple intensity);
void render_circle();
void render_sphere(float height, char *file_name);
Canvas render_scene();
Canvas render(struct camera c, world_t w);
world_t new_world();
world_t default_world();
void intersect_world(world_t world, struct ray r, Intersections *out);
Computation prepare_computations(Intersection inter, struct ray r1);
Tuple shade_hit(world_t w, Computation comps);
Tuple color_at(world_t w, struct ray r);
void view_transform(Tuple from, Tuple to, Tuple up, float output_matrix[4 * 4]);
struct camera camera(int hsize, int vsize, float field_of_view);
struct ray ray_for_pixel(struct camera c, int x, int y);
bool is_shadowed(world_t world, Tuple point);

/**
 * @brief Reads a pixel from a canvas at a particular x,y location.
 *
 * @param[in] canvas the canvas that gets its pixel read from.
 * @param x x-pos of the pixel
 * @param y y-pos of the pixel
 * @return Tuple The colour that was at that position
 */
inline Tuple read_pixel(Canvas *canvas, int x, int y)
{
    int index = y * canvas->width + x;
    return color(canvas->pixels[index].r, canvas->pixels[index].g, canvas->pixels[index].b);
}
/**
 * @brief writes a pixel to a canvas.
 *
 * @param[out, in] canvas the canvas that gets its pixel written.
 * @param x x-pos of the pixel
 * @param y y-pos of the pixel
 * @param colour the colour that gets written to the canvas.
 */
inline void write_pixel(Canvas *canvas, int x, int y, Tuple colour)
{
    int index = y * canvas->width + x;
    canvas->pixels[index] = (PackedColor){colour.x, colour.y, colour.z};
}

/**
 * @brief It renders a cool circle (.ppm)
 */
void render_circle()
{
    const int SIZE = 480;
    Canvas my_canvas = new_canvas(SIZE, SIZE);
    Tuple red = color(.95, 0.2, 0.2);
    Tuple camera_origin = point(0, 0, 0);
    Tuple pixel_point, camera_vector;
    struct ray color_ray;
    Primitive cool_red_sphere = sphere_primitive();
    Tuple black = color(0, 0, 0);
    float sphere_mat[16];

    mat_translate(0, 0, 2.0, sphere_mat);
    set_sphere_transform(&cool_red_sphere.primitive.s, sphere_mat);

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            pixel_point = point((float)(x - (SIZE / 2)) / SIZE, (float)(y - (SIZE / 2)) / SIZE, 0.75);
            camera_vector = tuple_sub(pixel_point, camera_origin);
            color_ray = (struct ray){camera_origin, camera_vector};

            Intersections sphere_intersects;
            intersect(&cool_red_sphere, color_ray, &sphere_intersects);
            if (sphere_intersects.count > 0)
                write_pixel(&my_canvas, x, y, red);
            else
                write_pixel(&my_canvas, x, y, black);
        }
    }
    canvas_to_ppm(&my_canvas, "cool_circle.ppm");
}

// Create The Objects

Tuple tuple(float x, float y, float z, float w)
{
    return (Tuple){.x = x, .y = y, .z = z, .w = w};
}
Tuple point(float x, float y, float z)
{
    return (Tuple){.x = x, .y = y, .z = z, .w = 1};
}
Tuple vector(float x, float y, float z)
{
    return (Tuple){.x = x, .y = y, .z = z, .w = 0};
}
Tuple color(float red, float green, float blue)
{
    return (Tuple){.x = red, .y = green, .z = blue, .w = 0};
}

/**
 * @brief Creates a new canvas. THIS ALLOCATES MEMORY.
 *
 * @param width the height of the canvas.
 * @param height the width of the canvas
 * @return Canvas with its allocated frame buffer.
 */
Canvas new_canvas(int width, int height)
{
    PackedColor *pixels = calloc(width * height, sizeof(PackedColor));
    return (Canvas){.pixels = pixels, .height = height, .width = width};
}

// Equality Checks
bool equal(float x, float y)
{
    return (fabsf(x - y) < EPSILON);
}
bool tuple_equal(Tuple a, Tuple b)
{
    return (equal(a.x, b.x) && equal(a.y, b.y) && equal(a.z, b.z) &&
            equal(a.w, b.w));
}
// Helper Functions

Tuple tuple_add(Tuple a, Tuple b)
{
    return (Tuple){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
}
Tuple tuple_sub(Tuple a, Tuple b)
{
    return (Tuple){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
}
Tuple tuple_negate(Tuple a)
{
    return (Tuple){.x = -a.x, .y = -a.y, .z = -a.z, .w = -a.w};
}
Tuple tuple_scale(Tuple a, float scaler)
{
    return (Tuple){.x = a.x * scaler,
                   .y = a.y * scaler,
                   .z = a.z * scaler,
                   .w = a.w * scaler};
}

Tuple tuple_divide(Tuple a, float divisor)
{
    return (Tuple){.x = a.x / divisor,
                   .y = a.y / divisor,
                   .z = a.z / divisor,
                   .w = a.w / divisor};
}

float tuple_magnitude(Tuple a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

/**
 * @brief Finds the normalized version of a tuple. E.g the tuple that has a magnitude / length of 1.
 *
 * @param a The tuple to be normalized.
 * @return Tuple The normalized tuple.
 */
Tuple tuple_normalize(Tuple a)
{
    return tuple_divide(a, tuple_magnitude(a));
}

/**
 * @brief Takes the cross product of two tuples
 *
 * @param a The first tuple
 * @param b The second tuple
 * @return Tuple the resulting cross product.
 */
Tuple tuple_cross(Tuple a, Tuple b)
{
    return vector(a.y * b.z - (a.z * b.y), a.z * b.x - (a.x * b.z),
                  (a.x * b.y) - (a.y * b.x));
}
/**
 * @brief Takes the dot product of two tuples
 *
 * @param a The first tuple
 * @param b The second tuple
 * @return float the result of the dot product
 */
float tuple_dot(Tuple a, Tuple b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

Tuple tuple_hadamard_product(Tuple a, Tuple b)
{
    return color(a.x * b.x, a.y * b.y, a.z * b.z);
}
/**
 * @brief Writes a Canvas struct as a PPM to a file.
 *
 * @param canvas the canvas that holds the image data. CAN NOT BE NULL.
 * @param file_name the name of the image file that will be written. Should include .ppm at the end as best practice.
 */
void canvas_to_ppm(Canvas *canvas, char *file_name)
{
    if (canvas == NULL || file_name == NULL)
        fprintf(stderr, "Failed to write canvas to file, NULL filename or canvas.");

    char *ppm_data = malloc(sizeof(char) * 3 * canvas->width * canvas->height + 1);
    char *pointer = ppm_data;

    for (int y = 0; y < canvas->height; y++)
    {
        for (int x = 0; x < canvas->width; x++)
        {
            int index = y * canvas->width + x;
            int red = (int)(canvas->pixels[index].r * 255);
            int green = (int)(canvas->pixels[index].g * 255);
            int blue = (int)(canvas->pixels[index].b * 255);
            if (red > 255)
                red = 255;
            if (red < 0)
                red = 0;
            if (green > 255)
                green = 255;
            if (green < 0)
                green = 0;
            if (blue > 255)
                blue = 255;
            if (blue < 0)
                blue = 0;

            unsigned char r = (unsigned char)red;
            unsigned char g = (unsigned char)green;
            unsigned char b = (unsigned char)blue;

            pointer[0] = r;
            pointer[1] = g;
            pointer[2] = b;
            pointer += 3;
        }
    }
    pointer[0] = '\n';
    FILE *file = fopen(file_name, "wb");
    char temp[50];
    int header_len = sprintf(temp, "P6\n%d %d\n255\n", canvas->width, canvas->height);

    fwrite(temp, 1, header_len, file);
    fwrite(ppm_data, 1, (canvas->width * canvas->height * 3) + 1, file);
    fclose(file);

    free(ppm_data);
}
/**
 * @brief takes a 4x4 matrix and sets it to the identity matrix.
 */
void mat_identity_4x4(float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1;
    out[5] = 1;
    out[10] = 1;
    out[15] = 1;
}
/**
 * @brief Checks if two matrices are equal.
 *
 * @param size The size of the matrices. size * size matrix is assumed.
 * @param mat_a The first matrix
 * @param mat_b The second matrix
 * @return true if they are equal, false if they are not equal.
 */
bool mat_equal(int size, float *mat_a, float *mat_b)
{
    for (int i = 0; i < size * size; i++)
    {
        if (!equal(mat_a[i], mat_b[i]))
        {
            return false;
        }
    }
    return true;
}
/**
 * @brief multiplies two matrices.
 *
 * @param size The size of the matrices. size * size matrix is assumed.
 * @param[in] mat_a The first matrix
 * @param[in] mat_b The second matrix
 * @param[out] mat_out the output of the matrix multiplication.
 */
void mat_mult(int size, float *mat_a, float *mat_b, float *mat_out)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mat_out[i * size + j] = 0.0f;
            for (int k = 0; k < size; k++)
            {
                mat_out[i * size + j] += mat_a[i * size + k] * mat_b[k * size + j];
            }
        }
    }
}

/**
 * @brief transposes a matrix.
 *
 * @param[in] size The size of the matrix. size * size matrix is assumed.
 * @param[in] mat The first matrix
 * @param[out] mat_out where the transposed matrix is stored.
 */
void mat_transpose(int size, float *mat, float *mat_out)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            mat_out[row * size + col] = mat[col * size + row];
        }
    }
}

float mat_minor(int size, float *mat, int row, int col)
{
    float sub[(size - 1) * (size - 1)];
    submatrix(size, mat, sub, row, col);
    return mat_det(size - 1, sub);
}

float mat_cofactor(int size, float *mat, int row, int col)
{
    float minor = mat_minor(size, mat, row, col);
    return ((row + col) % 2 != 0) ? -minor : minor;
}

void submatrix(int size, float *mat_a, float *mat_out, int remove_row, int remove_col)
{
    int new_row = 0;
    for (int row = 0; row < size; row++)
    {
        if (row == remove_row)
            continue;
        int new_col = 0;
        for (int col = 0; col < size; col++)
        {
            if (col == remove_col)
                continue;
            mat_out[new_row * (size - 1) + new_col] = mat_a[row * size + col];
            new_col++;
        }
        new_row++;
    }
}
/**
 * @brief Finds the determinant of a 4x4 matrix. This exists as a special case since it is done so often. It helps the compiler optimize (hopefully!).
 *
 * @param[in] m the matrix that has its determinant taken.
 * @return float the determinant.
 */
inline float mat_det_4x4(float *m)
{
    return m[3] * m[6] * m[9] * m[12] - m[2] * m[7] * m[9] * m[12] -
           m[3] * m[5] * m[10] * m[12] + m[1] * m[7] * m[10] * m[12] +
           m[2] * m[5] * m[11] * m[12] - m[1] * m[6] * m[11] * m[12] -
           m[3] * m[6] * m[8] * m[13] + m[2] * m[7] * m[8] * m[13] +
           m[3] * m[4] * m[10] * m[13] - m[0] * m[7] * m[10] * m[13] -
           m[2] * m[4] * m[11] * m[13] + m[0] * m[6] * m[11] * m[13] +
           m[3] * m[5] * m[8] * m[14] - m[1] * m[7] * m[8] * m[14] -
           m[3] * m[4] * m[9] * m[14] + m[0] * m[7] * m[9] * m[14] +
           m[1] * m[4] * m[11] * m[14] - m[0] * m[5] * m[11] * m[14] -
           m[2] * m[5] * m[8] * m[15] + m[1] * m[6] * m[8] * m[15] +
           m[2] * m[4] * m[9] * m[15] - m[0] * m[6] * m[9] * m[15] -
           m[1] * m[4] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
}
/**
 * @brief Finds the determinant of a 3x3 matrix. This exists as a special case since it is done so often. It helps the compiler optimize (hopefully!).
 *
 * @param[in] m the matrix that has its determinant taken.
 * @return float the determinant.
 */
inline float mat_det_3x3(float *m)
{
    return m[0] * (m[4] * m[8] - m[5] * m[7]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}
/**
 * @brief Function that takes the determinant of a matrix.
 *
 * @param size the size of the matrix
 * @param[in] mat_a the matrix
 * @return float the determinant.
 */
float mat_det(int size, float *mat_a)
{
    if (size == 2)
    {
        return mat_a[0] * mat_a[3] - mat_a[1] * mat_a[2];
    }
    if (size == 3)
        return mat_det_3x3(mat_a);
    if (size == 4)
        return mat_det_4x4(mat_a);

    float det = 0;
    float sub[(size - 1) * (size - 1)];
    int sign = 1;

    for (int i = 0; i < size; i++)
    {
        submatrix(size, mat_a, sub, 0, i);
        det += sign * mat_a[i] * mat_det(size - 1, sub);
        sign = -sign;
    }
    return det;
}
/**
 * @brief Checks if a matrix can be inverted.
 *
 * @param size The size of the matrix being checked
 * @param[in] mat the matrix
 * @return true if it is invertible, false otherwise
 */
bool mat_is_invertible(int size, float *mat)
{
    return !equal(mat_det(size, mat), 0);
}

/**
 * @brief Inverts a 4x4 matrix. Please don't ask. It's in the hot path.
 *
 * @param[in] m the matrix to be inverted.
 * @param[out] out where the inverted matrix is stored
 */
void mat_inverse_4x4(float *m, float *out)
{
    float inv[16];

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;
    det = 1.0f / det;

    for (int i = 0; i < 16; i++)
    {
        out[i] = inv[i] * det;
    }
}
/**
 * @brief Find the inverse of the matrix
 *
 * @param size the size of the matrix.
 * @param[in] mat the matrix that will be inverted
 * @param[out] out_mat the matrix where the inverse is stored
 */
void mat_inverse(int size, float *mat, float *out_mat)
{
    if (size == 4)
    {
        mat_inverse_4x4(mat, out_mat);
        return;
    }

    float det = mat_det(size, mat);
    if (det == 0)
        return;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            float c = mat_cofactor(size, mat, row, col);
            out_mat[col * size + row] = c / det;
        }
    }
}
/**
 * @brief Multiples a 4x4 matrix by a tuple.
 *
 * @param[in] mat the matrix being multiplied
 * @param tup the tuple being multiplied
 * @return Tuple the output of the multiplication.
 */
Tuple mat_tuple_mult(float *mat, Tuple tup)
{
    Tuple temp;
    temp.x = mat[0] * tup.x + mat[1] * tup.y + mat[2] * tup.z + mat[3] * tup.w;
    temp.y = mat[4] * tup.x + mat[5] * tup.y + mat[6] * tup.z + mat[7] * tup.w;
    temp.z = mat[8] * tup.x + mat[9] * tup.y + mat[10] * tup.z + mat[11] * tup.w;
    temp.w = mat[12] * tup.x + mat[13] * tup.y + mat[14] * tup.z + mat[15] * tup.w;
    return temp;
}
/**
 * @brief creates a translation matrix based on x, y, and z values.
 *
 * @param x how much to translate in the x direction.
 * @param y how much to translate in the y direction.
 * @param z how much to translate in the z direction.
 * @param[out] out the output translation matrix.
 */
void mat_translate(float x, float y, float z, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1;
    out[5] = 1;
    out[10] = 1;
    out[15] = 1;
    out[3] = x;
    out[7] = y;
    out[11] = z;
}
/**
 * @brief creates a scaling matrix based on x, y, and z values.
 *
 * @param x how much to scale in the x direction.
 * @param y how much to scale in the y direction.
 * @param z how much to scale in the z direction.
 * @param[out] out the output scaling matrix.
 */
void mat_scale(float x, float y, float z, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = x;
    out[5] = y;
    out[10] = z;
    out[15] = 1.0f;
}

/**
 * @brief creates a matrix to rotate about the x axis, r radians.
 *
 * @param radians the number of radians to rotate.
 * @param[out] out the rotation matrix.
 */
void mat_rotate_x(float radians, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1.0f;
    out[5] = cosf(radians);
    out[10] = cosf(radians);
    out[6] = -sinf(radians);
    out[9] = sinf(radians);
    out[15] = 1.0f;
}

/**
 * @brief creates a matrix to rotate about the y axis, r radians.
 *
 * @param radians the number of radians to rotate.
 * @param[out] out the rotation matrix.
 */
void mat_rotate_y(float radians, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = cosf(radians);
    out[5] = 1.0;
    out[10] = cosf(radians);
    out[15] = 1.0f;
    out[8] = -sinf(radians);
    out[2] = sinf(radians);
}

/**
 * @brief creates a matrix to rotate about the z axis, r radians.
 *
 * @param radians the number of radians to rotate.
 * @param[out] out the rotation matrix.
 */
void mat_rotate_z(float radians, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = cosf(radians);
    out[5] = cosf(radians);
    out[10] = 1.0f;
    out[15] = 1.0f;
    out[1] = -sinf(radians);
    out[4] = sinf(radians);
}
/**
 * @brief creates a matrix to sheer. I have no clue how it works or what the parameters are. Look up matrix sheering algorithm.
 *
 * @param a I
 * @param b Dont
 * @param c Know
 * @param d Lol
 * @param e please ignore
 * @param f look it up
 * @param[out] out the output sheering matrix.
 */
void mat_sheer(float a, float b, float c, float d, float e, float f, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1;
    out[5] = 1;
    out[10] = 1;
    out[15] = 1;
    out[1] = a;
    out[2] = b;
    out[4] = c;
    out[6] = d;
    out[8] = e;
    out[9] = f;
}

Tuple ray_position(struct ray r, double time)
{
    return tuple_add(r.position, tuple_scale(r.direction, time));
}
Sphere sphere()
{
    return (Sphere){
        .position = point(0, 0, 0),
        .radius = 1,
        .transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .inverse_transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .transpose = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .material = material(color(1, 1, 1), 0.1, 0.9, 0.9, 200.0),
    };
}
Primitive sphere_primitive()
{
    return (Primitive){
        .primitive.s = sphere(),
        .primitiveType = ObjectSphere,
    };
}

Plane plane()
{
    return (Plane){
        .transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .inverse_transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .transpose = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        .material = material(color(1, 1, 1), 0.1, 0.9, 0.9, 200.0),
    };
}
Primitive plane_primitive()
{
    return (Primitive){
        .primitive.pl = plane(),
        .primitiveType = ObjectPlane,
    };
}

static void sphere_intersect(Primitive *s, struct ray r1, Intersections *inters)
{
    struct ray ray = ray_transform(r1, s->primitive.s.inverse_transform);
    Intersections ret;
    ret.count = 2;
    Tuple sphere_to_ray = tuple_sub(ray.position, s->primitive.s.position);

    float a = tuple_dot(ray.direction, ray.direction);
    float b = 2 * tuple_dot(ray.direction, sphere_to_ray);
    float c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;

    float discriminant = b * b - (4 * a * c);

    if (discriminant < 0)
    {
        ret.count = 0;
        *inters = ret;
        return;
    }

    float t1 = (-1 * b - sqrt(discriminant)) / (2 * a);
    float t2 = (-1 * b + sqrt(discriminant)) / (2 * a);
    Intersection inter1 = intersection(t1, s);
    Intersection inter2 = intersection(t2, s);

    if (t1 > t2)
        *inters = intersections(2, inter2, inter1);
    *inters = intersections(2, inter1, inter2);
}

static void plane_intersect(Primitive *s, struct ray r1, Intersections *inters)
{
    struct ray ray = ray_transform(r1, s->primitive.pl.inverse_transform);

    if (fabsf(ray.direction.y) < EPSILON)
    {
        *inters = (Intersections){.count = 0};
        return;
    }

    float t = -ray.position.y / ray.direction.y;
    *inters = intersections(1, intersection(t, s));
}

void intersect(Primitive *s, struct ray r1, Intersections *inters)
{
    switch (s->primitiveType)
    {
    case ObjectSphere:
        sphere_intersect(s, r1, inters);
        break;
    case ObjectPlane:
        plane_intersect(s, r1, inters);
        break;
    default:
        break;
    }
}

Intersection intersection(float time, Primitive *object)
{
    return (Intersection){.time = time, .object = object};
}
int comp(const void *a, const void *b)
{
    float time_a = ((Intersection *)a)->time;
    float time_b = ((Intersection *)b)->time;
    return (time_a > time_b) - (time_a < time_b);
}

// This function ensures that the intersections are sorted by their time variable.
Intersections intersections(int count, ...)
{
    Intersections ret;
    ret.count = count;
    if (count <= 0)
        return ret;
    if (count > MAX_INTERSECTIONS)
    {
        fprintf(stderr, "ERROR: Exceeded MAX_INTERSECTIONS (%d). Truncating.\n", MAX_INTERSECTIONS);
        count = MAX_INTERSECTIONS;
    }

    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        ret.intersections[i] = va_arg(args, Intersection);
    }
    va_end(args);
    qsort(ret.intersections, count, sizeof(Intersection), comp);
    return ret;
}

Intersection hit(Intersections inters)
{
    for (int i = 0; i < inters.count; i++)
    {
        if (inters.intersections[i].time >= 0)
            return inters.intersections[i];
    }
    return intersection(0, NULL);
}

struct ray ray_transform(struct ray r, float *m)
{
    r.position = mat_tuple_mult(m, r.position);
    r.direction = mat_tuple_mult(m, r.direction);
    return r;
}
void set_sphere_transform(Sphere *object, float *m)
{
    memcpy((((Sphere *)object)->transform), m, sizeof(float) * 16);
    float inverse[16];
    mat_inverse(4, m, inverse);
    memcpy((((Sphere *)object)->inverse_transform), inverse, sizeof(float) * 16);

    mat_transpose(4, inverse, ((Sphere *)object)->transpose);
}
void set_camera_transform(struct camera *cam, float *m)
{
    memcpy(cam->transform, m, sizeof(float) * 16);
    float inverse[16];
    mat_inverse(4, m, inverse);
    memcpy(cam->inverse_transform, inverse, sizeof(float) * 16);
}

Tuple sphere_normal_at(Sphere s, Tuple p)
{
    Tuple object_point = mat_tuple_mult(s.inverse_transform, p);
    Tuple object_normal = tuple_sub(object_point, point(0, 0, 0));
    Tuple world_normal = mat_tuple_mult(s.transpose, object_normal);
    world_normal.w = 0;
    return tuple_normalize(world_normal);
}

Tuple plane_normal_at(Plane s, Tuple p)
{
    UNUSED(p);
    Tuple world_normal = mat_tuple_mult(s.transpose, vector(0, 1, 0));
    world_normal.w = 0;
    return tuple_normalize(world_normal);
}

/**
 * @brief Reflects a tuple about a surface normal.
 *
 * @param incoming The incoming tuple
 * @param normal The normal
 * @return Tuple the reflected normal.
 */
Tuple reflect(Tuple incoming, Tuple normal)
{
    return tuple_sub(incoming, tuple_scale(normal, 2 * tuple_dot(incoming, normal)));
}

PointLight point_light(Tuple position, Tuple intensity)
{
    return (PointLight){.intensity = intensity, .position = position};
}

struct material material(Tuple color, float ambient, float diffuse, float specular, float shininess)
{
    return (struct material){.color = color, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess, .reflective = 0.f};
}

bool materials_equal(struct material mat1, struct material mat2)
{
    return equal(mat2.ambient, mat1.ambient) && tuple_equal(mat1.color, mat2.color) && equal(mat2.diffuse, mat1.diffuse) && equal(mat2.shininess, mat1.shininess) && equal(mat2.specular, mat1.specular) && equal(mat2.reflective, mat1.reflective);
}

Tuple lighting(struct material material, PointLight light, Tuple position, Tuple eye_vector, Tuple normal_vector, bool in_shadow)
{
    Tuple effective_color = tuple_hadamard_product(material.color, light.intensity);
    Tuple light_vector = tuple_normalize(tuple_sub(light.position, position));
    float light_dot_normal = tuple_dot(light_vector, normal_vector);
    Tuple ambient = tuple_scale(effective_color, material.ambient);
    if (in_shadow)
        return ambient;
    Tuple diffuse, specular;
    if (light_dot_normal < 0)
    {
        diffuse = color(0, 0, 0);
        specular = color(0, 0, 0);
    }
    else
    {
        diffuse = tuple_scale(effective_color, material.diffuse * light_dot_normal);

        Tuple reflection_vector = reflect(tuple_negate(light_vector), normal_vector);
        float reflection_dot_eye = tuple_dot(reflection_vector, eye_vector);
        if (reflection_dot_eye <= 0)
        {
            specular = color(0, 0, 0);
        }
        else
        {
            float factor = powf(reflection_dot_eye, material.shininess);
            specular = tuple_scale(light.intensity, material.specular * factor);
        }
    }

    return tuple_add(diffuse, tuple_add(specular, ambient));
}

world_t new_world()
{
    return (world_t){.light = point_light(point(0, 0, 0), color(0, 0, 0)), .objects = malloc(sizeof(Primitive) * 10), .num_objects = 0};
}
/**
 * @brief Creates a default world.
 *
 * @return world_t The world.
 */
world_t default_world()
{
    world_t wor = {.light = point_light(point(-10, 10, -10), color(1, 1, 1)), .objects = malloc(sizeof(Primitive) * 10), .num_objects = 0};
    Primitive sphere1 = {.primitiveType = ObjectSphere, .primitive.s = sphere()};
    sphere1.primitive.s.material = material(color(0.8, 1, 0.6), 0.1, 0.7, 0.2, 200.0);

    float s2_mat[4 * 4];
    mat_scale(0.5, 0.5, 0.5, s2_mat);
    Primitive sphere2 = {.primitiveType = ObjectSphere, .primitive.s = sphere()};
    set_sphere_transform(&sphere2.primitive.s, s2_mat);

    wor.objects[0] = sphere1;
    wor.objects[1] = sphere2;
    wor.num_objects = 2;

    return wor;
}
/**
 * @brief Checks if a point in the world is in shadow
 *
 * @param world The world, e.g lights, objects etc, that will be checked for shadows.
 * @param point The point that will be checked.
 * @return true if the point is in a shadow. False if it is not.
 */
bool is_shadowed(world_t world, Tuple point)
{
    Tuple v = tuple_sub(world.light.position, point);
    float distance = tuple_magnitude(v);
    Tuple direction = tuple_normalize(v);

    Intersections inters;
    intersect_world(world, (struct ray){point, direction}, &inters);

    Intersection h = hit(inters);
    if (h.object != NULL && h.time < distance)
        return true;
    else
        return false;
}

void intersect_world(world_t world, struct ray r, Intersections *ret)
{
    ret->count = 0;
    for (int i = 0; i < world.num_objects; i++)
    {
        Intersections local_inters;
        intersect(&world.objects[i], r, &local_inters);

        for (int j = 0; j < local_inters.count; j++)
        {
            if (ret->count < MAX_INTERSECTIONS)
            {
                ret->intersections[ret->count++] = local_inters.intersections[j];
            }
        }
    }

    for (int i = 1; i < ret->count; i++)
    {
        Intersection key = ret->intersections[i];
        int j = i - 1;
        while (j >= 0 && ret->intersections[j].time > key.time)
        {
            ret->intersections[j + 1] = ret->intersections[j];
            j -= 1;
        }
        ret->intersections[j + 1] = key;
    }
}

Computation prepare_computations(Intersection inter, struct ray r1)
{
    // Instantiate a data structure for storing some precomputed values
    Computation comps;

    // Copy the intersection's properties, for convenience
    comps.time = inter.time;
    comps.object = inter.object;
    // Pre-compute some useful values
    comps.point = ray_position(r1, comps.time);
    comps.eyev = tuple_negate(r1.direction);

    Primitive *prim = comps.object;
    switch (prim->primitiveType)
    {
    case ObjectSphere:
        comps.normalv = sphere_normal_at(prim->primitive.s, comps.point);
        break;
    case ObjectPlane:
        comps.normalv = plane_normal_at(prim->primitive.pl, comps.point);
        break;

    default:
        break;
    }

    comps.inside = (tuple_dot(comps.normalv, comps.eyev) < 0.0f);

    if (comps.inside)
        comps.normalv = tuple_negate(comps.normalv);
    comps.point = tuple_add(comps.point, tuple_scale(comps.normalv, EPSILON * 2));
    return comps;
}

Tuple shade_hit(world_t w, Computation comps)
{
    bool shadowed = is_shadowed(w, comps.point);
    Primitive *prim = (Primitive *)comps.object;

    struct material mat;
    switch (prim->primitiveType)
    {
    case ObjectSphere:
        mat = prim->primitive.s.material;
        break;
    case ObjectPlane:
        mat = prim->primitive.pl.material;
        break;
    }

    return lighting(mat, w.light, comps.point, comps.eyev, comps.normalv, shadowed);
}

void set_transform(Primitive *object, float *m)
{
    float inverse[16];
    mat_inverse(4, m, inverse);
    switch (object->primitiveType)
    {
    case ObjectSphere:
        memcpy(object->primitive.s.transform, m, sizeof(float) * 16);
        memcpy(object->primitive.s.inverse_transform, inverse, sizeof(float) * 16);
        mat_transpose(4, inverse, object->primitive.s.transpose);
        break;
    case ObjectPlane:
        memcpy(object->primitive.pl.transform, m, sizeof(float) * 16);
        memcpy(object->primitive.pl.inverse_transform, inverse, sizeof(float) * 16);
        mat_transpose(4, inverse, object->primitive.pl.transpose);
        break;
    }
}
Tuple color_at(world_t w, struct ray r)
{
    Intersections inters;

    intersect_world(w, r, &inters);
    Intersection h = hit(inters);

    if (h.object == NULL)
        return color(0, 0, 0);

    Computation mafs = prepare_computations(h, r);
    return shade_hit(w, mafs);
}

void view_transform(Tuple from, Tuple to, Tuple up, float output_matrix[4 * 4])
{
    Tuple forward_vec = tuple_normalize(tuple_sub(to, from));
    Tuple left = tuple_cross(forward_vec, tuple_normalize(up));
    Tuple true_up = tuple_cross(left, forward_vec);

    float orientation[16];
    memset(orientation, 0, sizeof(float) * 16);

    orientation[0] = left.x;
    orientation[1] = left.y;
    orientation[2] = left.z;
    orientation[4] = true_up.x;
    orientation[5] = true_up.y;
    orientation[6] = true_up.z;
    orientation[8] = -forward_vec.x;
    orientation[9] = -forward_vec.y;
    orientation[10] = -forward_vec.z;
    orientation[15] = 1;

    float translate[16];
    mat_translate(-from.x, -from.y, -from.z, translate);

    mat_mult(4, orientation, translate, output_matrix);
}
struct camera camera(int hsize, int vsize, float field_of_view)
{
    struct camera c = {.hsize = hsize, .vsize = vsize, .field_of_view = field_of_view};

    mat_identity_4x4(c.transform);
    mat_identity_4x4(c.inverse_transform);

    float half_view = tan(c.field_of_view / 2.0);
    float aspect = (float)c.hsize / (float)c.vsize;
    if (aspect >= 1)
    {
        c.half_width = half_view;
        c.half_height = half_view / aspect;
    }
    else
    {
        c.half_width = half_view * aspect;
        c.half_height = half_view;
    }
    c.pixel_size = (c.half_width * 2) / c.hsize;
    return c;
}

struct ray ray_for_pixel(struct camera c, int x, int y)
{
    float xoffset, yoffset;

    xoffset = (x + 0.5) * c.pixel_size;
    yoffset = (y + 0.5) * c.pixel_size;

    float world_x = c.half_width - xoffset;
    float world_y = c.half_height - yoffset;

    Tuple pixel = mat_tuple_mult(c.inverse_transform, point(world_x, world_y, -1));
    Tuple origin = mat_tuple_mult(c.inverse_transform, point(0, 0, 0));
    Tuple direction = tuple_normalize(tuple_sub(pixel, origin));

    return (struct ray){origin, direction};
}

Canvas render(struct camera c, world_t w)
{
    Canvas image = new_canvas(c.hsize, c.vsize);
    for (int y = 0; y < c.vsize; y++)
    {
        for (int x = 0; x < c.hsize; x++)
        {
            struct ray ray = ray_for_pixel(c, x, y);
            Tuple color = color_at(w, ray);
            write_pixel(&image, x, y, color);
        }
    }

    return image;
}
struct thread_args
{
    int start_y, end_y;
    struct camera c;
    world_t w;
    Canvas *image;
};

void *thread_render(void *args_void)
{
    struct thread_args *args = (struct thread_args *)args_void;

    for (int y = args->start_y; y < args->end_y; y++)
    {
        for (int x = 0; x < args->c.hsize; x++)
        {
            struct ray ray = ray_for_pixel(args->c, x, y);
            Tuple color = color_at(args->w, ray);
            write_pixel(args->image, x, y, color);
        }
    }
    return NULL;
}

Canvas render_frame(struct camera c, world_t w, Canvas *image)
{
    pthread_t threads[NUM_THREADS];
    struct thread_args args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        args[i] = (struct thread_args){
            .start_y = i * (c.vsize / NUM_THREADS),
            .end_y = (i + 1) * (c.vsize / NUM_THREADS),
            .c = c,
            .w = w,
            .image = image};

        pthread_create(&threads[i], NULL, thread_render, &args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return *image;
}
