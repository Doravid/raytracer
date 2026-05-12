#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define EPSILON (0.00001)
typedef struct tuple
{
    float x, y, z, w;
} Tuple;

typedef struct canvas
{
    Tuple **canvas;
    int width;
    int height;
} Canvas;

typedef struct ray
{
    Tuple position;
    Tuple direction;
} Ray;

typedef struct sphere
{
    Tuple position;
    float radius;
    float transform[4][4];
} Sphere;

typedef struct intersection_tuple
{
    int count;
    float distances[2];
} Intersection_tuple;
typedef struct intersection
{
    float time;
    void *object;
} Intersection;

typedef struct intersections
{
    int count;
    Intersection *intersections;
} Intersections;

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
Canvas canvas(int width, int height);
Tuple color(float red, float green, float blue);
void write_pixel(Canvas *can, int x, int y, Tuple col);
void canvas_to_ppm(Canvas *can, char *file_name);
// void write_ppm_to_file(char *data, int width, int height, char *file_name);
void mat_identity(float mat[4][4]);
bool mat_equal(int size, float mat_a[size][size], float mat_b[size][size]);
void mat_mult(int size, float mat_a[size][size], float mat_b[size][size], float mat_out[size][size]);
void mat_transpose(int size, float mat_a[size][size], float mat_out[size][size]);
void submatrix(int size, float mat_a[size][size], float mat_out[size - 1][size - 1], int remove_row, int remove_col);
float mat_det(int size, float mat_a[size][size]);
float mat_minor(int size, float mat[size][size], int row, int col);
float mat_cofactor(int size, float mat[size][size], int row, int col);
void mat_inverse(int size, float mat[size][size], float out_mat[size][size]);
Tuple mat_tuple_mult(float mat[4][4], Tuple tup);
void mat_translate(float x, float y, float z, float out[4][4]);
void mat_rotate_x(float r, float out[4][4]);
void mat_rotate_y(float r, float out[4][4]);
void mat_rotate_z(float r, float out[4][4]);
void mat_scale(float x, float y, float z, float out[4][4]);
void mat_sheer(float a, float b, float c, float d, float e, float f, float out[4][4]);
Ray ray(Tuple origin, Tuple direction);
Tuple ray_position(Ray r, double time);
Ray ray_transform(Ray r, float m[4][4]);
Sphere sphere();
Intersections intersect(Sphere s, Ray r);
Intersection intersection(float time, void *object);
Intersections intersections(int count, ...);
Intersection hit(Intersections inters);
void set_transform(void *object, float m[4][4]);
Tuple sphere_normal_at(Sphere s, Tuple p);

void test_linear_algebra()
{
    // Test Equality
    assert(!equal(0.2, 0.30000001));
    assert(equal(0.2, 0.20000001));
    // Vector and Point Init Tests
    Tuple tup1 = {.x = 4, .y = -4, .z = 3, .w = 1};
    Tuple point_a = point(4, -4, 3);
    assert(tuple_equal(tup1, point_a));

    Tuple tup2 = {.x = 4, .y = -4, .z = 3, .w = 0};
    Tuple vector_a = vector(4, -4, 3);

    assert(tuple_equal(tup2, vector_a));

    // Vector and Point addition tests.
    Tuple a1 = {.x = 3, .y = -2, .z = 5, .w = 1};
    Tuple a2 = {.x = -2, .y = 3, .z = 1, .w = 0};

    Tuple a1_a2 = tuple_add(a1, a2);
    Tuple test_result1 = {.x = 1, .y = 1, .z = 6, .w = 1};

    assert(tuple_equal(a1_a2, test_result1));

    // Vector and Point subtraction tests.

    Tuple p1 = point(3, 2, 1);
    Tuple p2 = point(5, 6, 7);
    Tuple p1_p2 = tuple_sub(p1, p2);

    assert(tuple_equal(p1_p2, vector(-2, -4, -6)));
    // Tuple Negation Test
    Tuple a = tuple(1, -2, 3, -4);
    Tuple a_neg = tuple_negate(a);
    assert(tuple_equal(a_neg, tuple(-1, 2, -3, 4)));

    // Tuple Scale Test
    Tuple b = tuple_scale(a, 3.5);
    Tuple result = tuple(3.5, -7, 10.5, -14);
    assert(tuple_equal(result, b));

    // Tuple Divide Test
    Tuple c = tuple_divide(a, 2);
    result = tuple(0.5, -1, 1.5, -2);
    assert(tuple_equal(result, c));

    // Tuple Magnitude Checks
    Tuple v1 = vector(1, 0, 0);
    Tuple v2 = vector(0, 1, 0);
    Tuple v3 = vector(0, 0, 1);
    Tuple v4 = vector(1, 2, 3);
    Tuple v5 = vector(-1, -2, -3);
    assert(tuple_magnitude(v1) == 1);
    assert(tuple_magnitude(v2) == 1);
    assert(tuple_magnitude(v3) == 1);
    assert(tuple_magnitude(v4) == sqrtf(14));
    assert(tuple_magnitude(v5) == sqrtf(14));

    // Tuple normalize Tests
    Tuple v = vector(4, 0, 0);
    Tuple normalized = vector(1, 0, 0);
    assert(tuple_equal((tuple_normalize(v)), normalized));
    // Tuple dot product test
    a = vector(1, 2, 3);
    b = vector(2, 3, 4);
    assert(tuple_dot(a, b) == 20);

    // Tuple cross product test
    assert(tuple_equal(tuple_cross(a, b), vector(-1, 2, -1)));
    assert(tuple_equal(tuple_cross(b, a), vector(1, -2, 1)));

    // Test Canvas
    Canvas can = canvas(1920, 2);
    Tuple my_color = can.canvas[0][0];
    assert(tuple_equal(my_color, color(0, 0, 0)));

    // Test Write to canvas

    write_pixel(&can, 1900, 1, color(1, 0.2, 0.3));
    // Tuple col = can.canvas[1900][1];
    assert(tuple_equal(can.canvas[1900][1], color(1, 0.2, 0.3)));

    // Test write to ppm
    const int size = 80;
    Canvas new_canvas = canvas(size, size);
    Tuple red = color(0.9, 0.3, 0.3);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            new_canvas.canvas[i][j] = color(.6, i / 30.0, j / 30.0);
        }
    }
    canvas_to_ppm(&new_canvas, "output.ppm");
    // Test Physics Sim thing
    // Tuple black = color(0, 0, 0);
    Canvas physics_canvas = canvas(size, size);
    Tuple position = point(0, 0, 0);
    Tuple velocity = vector(5, 12, 0);

    Tuple acceleration = vector(0.15, -1.5, 0);
    const int MAX_STEPS = 30;
    for (int i = 0; i < MAX_STEPS; ++i)
    {

        int x = (int)position.x;
        int y = (int)position.y;

        if (x >= 0 && x < size && y >= 0 && y < size)
        {
            physics_canvas.canvas[x][size - y - 1] = red;
        }
        position = tuple_add(position, velocity);
        velocity = tuple_add(velocity, acceleration);
    }
    canvas_to_ppm(&physics_canvas, "physics.ppm");
    // Test Matrix
    float my_matrix[4][4] = {
        {1, 2, 3, 4},
        {5.5, 6.5, 7.5, 8.5},
        {9, 10, 11, 12},
        {13.5, 14.5, 15.5, 16.5},
    };
    float my_matrix2[4][4] = {
        {1, 2, 3, 4},
        {5.5, 6.5, 7.5, 8.5},
        {9, 10, 11, 12},
        {13.5, 14.5, 15.5, 16.5},
    };
    float my_matrix3[4][4] = {
        {1, 2, 3, 4},
        {5.5, 6.5, 7.5, 8.5},
        {9, 10, 11, 12},
        {13.5, 14.5, 15.5, 16.54},
    };

    assert(my_matrix[0][0] == 1);
    assert(my_matrix[0][3] == 4);
    assert(my_matrix[1][2] == 7.5);

    assert(mat_equal(4, &my_matrix[0], &my_matrix2[0]));
    assert(!mat_equal(4, &my_matrix[0], &my_matrix3[0]));
    // Test Matrix Mult
    float mat_1[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 8, 7, 6},
        {5, 4, 3, 2},
    };

    float mat_2[4][4] = {
        {-2, 1, 2, 3},
        {3, 2, 1, -1},
        {4, 3, 6, 5},
        {1, 2, 7, 8},
    };

    float res_matrix[4][4];
    mat_mult(4, mat_1, mat_2, res_matrix);

    float my_mat[4][4] = {
        {20, 22, 50, 48},
        {44, 54, 114, 108},
        {40, 58, 110, 102},
        {16, 26, 46, 42},
    };
    assert(mat_equal(4, my_mat, res_matrix));
    // Test Matrix x Tuple
    float mat_tuple[4][4] = {
        {1, 2, 3, 4},
        {2, 4, 4, 2},
        {8, 6, 4, 1},
        {0, 0, 0, 1},
    };
    Tuple my_tuple = tuple(1, 2, 3, 1);

    Tuple res_tuple = mat_tuple_mult(mat_tuple, my_tuple);

    assert(tuple_equal(res_tuple, tuple(18, 24, 33, 1)));

    // Test Identity
    float identity_matrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };
    mat_mult(4, mat_1, identity_matrix, res_matrix);
    assert(mat_equal(4, mat_1, res_matrix));
    // Test Transpose
    float mat_a[4][4] = {
        {0, 9, 3, 0},
        {9, 8, 0, 8},
        {1, 8, 5, 3},
        {0, 0, 5, 8},
    };
    float transpose_mat_a[4][4] = {
        {0, 9, 1, 0},
        {9, 8, 8, 0},
        {3, 0, 5, 5},
        {0, 8, 3, 8},
    };
    mat_transpose(4, mat_a, res_matrix);
    assert(mat_equal(4, transpose_mat_a, res_matrix));
    // Test Submatrix
    float mat_3_3[3][3] = {
        {1, 5, 0},
        {-3, 2, 7},
        {0, 6, -3},
    };
    float res_2_2[2][2];
    submatrix(3, mat_3_3, res_2_2, 0, 2);
    float ref_2_2[2][2] = {
        {-3, 2},
        {0, 6},
    };

    assert(mat_equal(2, ref_2_2, res_2_2));

    float mat_b[4][4] = {
        {-6, 1, 1, 6},
        {-8, 5, 8, 6},
        {-1, 0, 8, 2},
        {-7, 1, -1, 1},
    };
    float res_3_3[3][3];

    submatrix(4, mat_b, res_3_3, 2, 1);

    float ref_3_3[3][3] = {
        {-6, 1, 6},
        {-8, 8, 6},
        {-7, -1, 1},
    };

    assert(mat_equal(3, ref_3_3, res_3_3));
    float A[3][3] = {{1, 2, 6}, {-5, 8, -4}, {2, 6, 4}};
    float B[4][4] = {
        {-2, -8, 3, 5},
        {-3, 1, 7, 3},
        {1, 2, -9, 6},
        {-6, 7, 7, -9},
    };
    float x = mat_det(3, A);
    float y = mat_det(4, B);
    assert(x == -196);
    assert(y == -4071);
    // Test Matrix Inverse
    float A1[4][4] = {
        {8, -5, 9, 2},
        {7, 5, 6, 1},
        {-6, 0, 9, 6},
        {-3, 0, -9, -4}};
    float expected_inv1[4][4] = {
        {-0.15385, -0.15385, -0.28205, -0.53846},
        {-0.07692, 0.12308, 0.02564, 0.03077},
        {0.35897, 0.35897, 0.43590, 0.92308},
        {-0.69231, -0.69231, -0.76923, -1.92308}};
    float out_inv1[4][4];

    mat_inverse(4, A1, out_inv1);
    assert(mat_equal(4, expected_inv1, out_inv1));

    float A2[4][4] = {
        {9, 3, 0, 9},
        {-5, -2, -6, -3},
        {-4, 9, 6, 4},
        {-7, 6, 6, 2}};
    float expected_inv2[4][4] = {
        {-0.04074, -0.07778, 0.14444, -0.22222},
        {-0.07778, 0.03333, 0.36667, -0.33333},
        {-0.02901, -0.14630, -0.10926, 0.12963},
        {0.17778, 0.06667, -0.26667, 0.33333}};
    float out_inv2[4][4];
    mat_inverse(4, A2, out_inv2);

    // Test Matrix Translate
    float trans_mat[4][4];
    float rev_trans_mat[4][4];
    mat_translate(5, -3, 2, trans_mat);
    mat_inverse(4, trans_mat, rev_trans_mat);

    Tuple some_point = point(-3, 4, 5);
    Tuple output = mat_tuple_mult(trans_mat, some_point);
    Tuple origin = mat_tuple_mult(rev_trans_mat, output);

    assert(tuple_equal(output, point(2, 1, 7)));
    assert(tuple_equal(some_point, origin));

    assert(mat_equal(4, expected_inv2, out_inv2));
    // Scaling
    float scale_mat[4][4];
    mat_scale(2, 3, 4, scale_mat);
    Tuple p_1 = point(-4, 6, 8);
    Tuple v_1 = vector(-4, 6, 8);

    Tuple scaled_point = mat_tuple_mult(scale_mat, p_1);
    Tuple scaled_vector = mat_tuple_mult(scale_mat, v_1);

    assert(tuple_equal(scaled_point, point(-8, 18, 32)));
    assert(tuple_equal(scaled_vector, vector(-8, 18, 32)));

    // Rotation
    Tuple test_point = point(0, 1, 0);
    float half_quarter[4][4];
    float full_quarter[4][4];
    // X Rotation
    mat_rotate_x(M_PI / 4, half_quarter);
    mat_rotate_x(M_PI / 2, full_quarter);

    Tuple half_point = mat_tuple_mult(half_quarter, test_point);
    Tuple quarter_point = mat_tuple_mult(full_quarter, test_point);

    assert(tuple_equal(half_point, point(0, sqrtf(2) / 2.0, sqrtf(2) / 2)));
    assert(tuple_equal(quarter_point, point(0, 0, 1)));

    // Y Rotation
    test_point = point(0, 0, 1);
    mat_rotate_y(M_PI / 4, half_quarter);
    mat_rotate_y(M_PI / 2, full_quarter);

    half_point = mat_tuple_mult(half_quarter, test_point);
    quarter_point = mat_tuple_mult(full_quarter, test_point);

    assert(tuple_equal(half_point, point(sqrtf(2) / 2.0, 0, sqrtf(2) / 2)));
    assert(tuple_equal(quarter_point, point(1, 0, 0)));

    // Z Rotation
    test_point = point(0, 1, 0);
    mat_rotate_z(M_PI / 4, half_quarter);
    mat_rotate_z(M_PI / 2, full_quarter);

    half_point = mat_tuple_mult(half_quarter, test_point);
    quarter_point = mat_tuple_mult(full_quarter, test_point);

    assert(tuple_equal(half_point, point(-sqrtf(2) / 2.0, sqrtf(2) / 2, 0)));
    assert(tuple_equal(quarter_point, point(-1, 0, 0)));
    // Shearing
    float sheer[4][4];
    mat_sheer(0, 0, 0, 0, 0, 1, sheer);
    test_point = point(2, 3, 4);
    output = mat_tuple_mult(sheer, test_point);
    assert(tuple_equal(output, point(2, 3, 7)));

    mat_sheer(0, 1, 0, 0, 0, 0, sheer);
    test_point = point(2, 3, 4);
    output = mat_tuple_mult(sheer, test_point);
    assert(tuple_equal(output, point(6, 3, 4)));

    mat_sheer(0, 0, 1, 0, 0, 0, sheer);
    test_point = point(2, 3, 4);
    output = mat_tuple_mult(sheer, test_point);
    assert(tuple_equal(output, point(2, 5, 4)));

    mat_sheer(0, 0, 0, 1, 0, 0, sheer);
    test_point = point(2, 3, 4);
    output = mat_tuple_mult(sheer, test_point);
    assert(tuple_equal(output, point(2, 7, 4)));
    // Ray Tests
    Ray ray_1 = ray(point(2, 3, 4), vector(1, 0, 0));
    assert(tuple_equal(ray_position(ray_1, 0), point(2, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, 1), point(3, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, -1), point(1, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, 2.5), point(4.5, 3, 4)));

    // Sphere Intersection Tests
    Ray ray1 = ray(point(0, 0, 0), vector(0, 0, 1));
    Sphere s = sphere();
    Intersections xs = intersect(s, ray1);

    assert(xs.count == 2);
    assert(xs.intersections[0].time == -1.0);
    assert(xs.intersections[1].time == 1.0);

    Intersection i1 = intersection(1, &s);
    Intersection i2 = intersection(2, &s);

    Intersections my_intersections = intersections(2, i1, i2);

    assert(my_intersections.intersections[0].time == 1);
    assert(my_intersections.intersections[1].time == 2);
    assert(my_intersections.intersections[0].object == &s);
    assert(my_intersections.intersections[1].object == &s);

    // Scenario: The hit is always the lowest nonnegative intersection
    Intersection int1 = intersection(5, &s);
    Intersection int2 = intersection(7, &s);
    Intersection int3 = intersection(-3, &s);
    Intersection int4 = intersection(2, &s);
    Intersections inters = intersections(4, int1, int2, int3, int4);

    Intersection inter = hit(inters);

    assert(inter.time == int4.time);
    assert(inter.object == int4.object);

    // Scenario: Translating a ray

    Ray r1 = ray(point(1, 2, 3), vector(0, 1, 0));
    float m[4][4];
    mat_translate(3, 4, 5, m);
    Ray r2 = ray_transform(r1, m);
    assert(tuple_equal(r2.position, point(4, 6, 8)));
    assert(tuple_equal(r2.direction, vector(0, 1, 0)));

    // Scenario: Scaling a ray
    r1 = ray(point(1, 2, 3), vector(0, 1, 0));
    mat_scale(2, 3, 4, m);
    r2 = ray_transform(r1, m);
    assert(tuple_equal(r2.position, point(2, 6, 12)));
    assert(tuple_equal(r2.direction, vector(0, 3, 0)));
    // Scenario: A sphere's default transformation
    s = sphere();
    mat_identity(m);
    assert(mat_equal(4, s.transform, m));

    // Scenario: Changing a sphere's transformation
    mat_translate(2, 3, 4, m);
    set_transform(&s, m);
    assert(mat_equal(4, s.transform, m));

    // Scenario: Intersecting a scaled sphere with a ray
    Ray r = ray(point(0, 0, -5), vector(0, 0, 1));
    s = sphere();
    mat_scale(2, 2, 2, m);
    set_transform(&s, m);
    xs = intersect(s, r);
    assert(xs.count == 2);
    assert(xs.count == 2);
    assert(xs.intersections[0].time == 3);
    assert(xs.intersections[1].time == 7);

    // A program that casts rays at a sphere and draw the picture to a canvas.
    const int SIZE = 100;
    Canvas my_canvas = canvas(SIZE, SIZE);

    // We want to define a point, say (0, 0) that will be the origin of all of our rays.
    // Then we will have a screen be say 1 unit in front of our point.
    // The circle will then be one unit in front of the screen
    // We will check if the vector from our point (0,0) to the given pixel on the screen intersects with the sphere.
    Tuple camera_origin = point(0, 0, 0);
    Tuple pixel_point, camera_vector;
    Ray color_ray;
    Sphere cool_red_sphere = sphere();
    Tuple black = color(0, 0, 0);
    float sphere_mat[4][4];

    mat_translate(0, 0, 2.0, sphere_mat);
    set_transform(&cool_red_sphere, sphere_mat);

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            // float offset = (float)rand() / (float)RAND_MAX;
            pixel_point = point((float)(x - (SIZE / 2)) / SIZE, (float)(y - (SIZE / 2)) / SIZE, 0.75);
            camera_vector = tuple_sub(pixel_point, camera_origin);
            color_ray = ray(camera_origin, camera_vector);

            Intersections sphere_intersects = intersect(cool_red_sphere, color_ray);
            if (sphere_intersects.count > 0)
                my_canvas.canvas[x][y] = red;
            else
                my_canvas.canvas[x][y] = black;
        }
    }
    canvas_to_ppm(&my_canvas, "cool_circle.ppm");

    // Scenario: The normal on a sphere at a nonaxial point
    s = sphere();
    Tuple normal = sphere_normal_at(s, point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    assert(tuple_equal(normal, vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3)));
    // Scenario: Computing the normal on a translated sphere
    s = sphere();
    mat_translate(0, 1, 0, sphere_mat);
    set_transform(&s, sphere_mat);
    normal = sphere_normal_at(s, point(0, 1.70711, -0.70711));
    assert(tuple_equal(normal, vector(0, 0.70711, -0.70711)));
}
//
// Create The Objects

Tuple tuple(float x, float y, float z, float w)
{
    Tuple tup = {.x = x, .y = y, .z = z, .w = w};
    return tup;
}
Tuple point(float x, float y, float z)
{
    Tuple new_point = {.x = x, .y = y, .z = z, .w = 1};
    return new_point;
}
Tuple vector(float x, float y, float z)
{
    Tuple new_point = {.x = x, .y = y, .z = z, .w = 0};
    return new_point;
}
Tuple color(float red, float green, float blue)
{
    Tuple new_point = {.x = red, .y = green, .z = blue, .w = 0};
    return new_point;
}

Canvas canvas(int width, int height)
{
    Tuple **pixels = malloc(sizeof(Tuple *) * width);
    pixels[0] = malloc(sizeof(Tuple) * width * height);
    memset(pixels[0], 0, sizeof(Tuple) * width * height);

    for (int i = 1; i < width; i++)
    {
        pixels[i] = pixels[0] + i * height;
    }

    Canvas c = {.canvas = pixels, .height = height, .width = width};
    return c;
}

// Equality Checks
bool equal(float x, float y) { return (fabsf(x - y) < EPSILON); }
bool tuple_equal(Tuple a, Tuple b)
{
    return (equal(a.x, b.x) && equal(a.y, b.y) && equal(a.z, b.z) &&
            equal(a.w, b.w));
}
// Helper Functions

Tuple tuple_add(Tuple a, Tuple b)
{
    Tuple x = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
    return x;
}
Tuple tuple_sub(Tuple a, Tuple b)
{
    Tuple x = {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
    return x;
}
Tuple tuple_negate(Tuple a)
{
    Tuple x = {.x = -a.x, .y = -a.y, .z = -a.z, .w = -a.w};
    return x;
}
Tuple tuple_scale(Tuple a, float scaler)
{
    Tuple x = {.x = a.x * scaler,
               .y = a.y * scaler,
               .z = a.z * scaler,
               .w = a.w * scaler};
    return x;
}

Tuple tuple_divide(Tuple a, float divisor)
{
    Tuple x = {.x = a.x / divisor,
               .y = a.y / divisor,
               .z = a.z / divisor,
               .w = a.w / divisor};
    return x;
}

float tuple_magnitude(Tuple a)
{
    float x = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
    return sqrtf(x);
}

Tuple tuple_normalize(Tuple a)
{
    float length = tuple_magnitude(a);
    Tuple b = tuple_divide(a, length);
    return b;
}
Tuple tuple_cross(Tuple a, Tuple b)
{
    return vector(a.y * b.z - (a.z * b.y), a.z * b.x - (a.x * b.z),
                  (a.x * b.y) - (a.y * b.x));
}
float tuple_dot(Tuple a, Tuple b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

Tuple tuple_hadamard_product(Tuple a, Tuple b)
{
    return color(a.x * b.x, a.y * b.y, a.z * b.z);
}

void write_pixel(Canvas *can, int x, int y, Tuple col)
{
    (*can).canvas[x][y] = col;
}

void canvas_to_ppm(Canvas *can, char *file_name)
{
    char *ppm_data = malloc(sizeof(char) * 3 * can->width * can->height + 1);

    char *pointer = ppm_data;
    for (int i = 0; i < can->height; i++)
    {
        for (int j = 0; j < can->width; j++)
        {
            int red = (int)(can->canvas[j][i].x * 255);
            int green = (int)(can->canvas[j][i].y * 255);
            int blue = (int)(can->canvas[j][i].z * 255);
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
    int header_len = sprintf(temp, "P6\n%d %d\n255\n", can->width, can->height);

    fwrite(temp, 1, header_len, file);
    fwrite(ppm_data, 1, (can->width * can->height * 3) + 1, file);
    fclose(file);

    free(ppm_data);
}
void mat_identity(float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);
    out[0][0] = 1;
    out[1][1] = 1;
    out[2][2] = 1;
    out[3][3] = 1;
}
bool mat_equal(int size, float mat_a[size][size], float mat_b[size][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!equal(mat_a[i][j], mat_b[i][j]))
            {
                // printf("i: %d, j: %d, mat1 %f, mat2 %f\n", i, j, mat_a[i][j], mat_b[i][j]);
                return false;
            }
        }
    }
    return true;
}

void mat_mult(int size, float mat_a[size][size], float mat_b[size][size], float mat_out[size][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mat_out[i][j] = 0.0f;
            for (int k = 0; k < size; k++)
            {
                mat_out[i][j] += mat_a[i][k] * mat_b[k][j];
            }
        }
    }
}

void mat_transpose(int size, float mat_a[size][size], float mat_out[size][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mat_out[i][j] = mat_a[j][i];
        }
    }
}

float mat_minor(int size, float mat[size][size], int row, int col)
{
    float sub[size - 1][size - 1];
    submatrix(size, mat, sub, row, col);
    return mat_det(size - 1, sub);
}

float mat_cofactor(int size, float mat[size][size], int row, int col)
{
    float minor = mat_minor(size, mat, row, col);
    return ((row + col) % 2 != 0) ? -minor : minor;
}

void submatrix(int size, float mat_a[size][size], float mat_out[size - 1][size - 1], int remove_row, int remove_col)
{
    int new_col = 0;
    for (int col = 0; col < size; col++)
    {
        int new_row = 0;

        if (col == remove_col)
            continue;
        for (int row = 0; row < size; row++)
        {
            if (row == remove_row)
                continue;
            mat_out[new_row][new_col] = mat_a[row][col];
            new_row++;
        }
        new_col++;
    }
    return;
}

float mat_det(int size, float mat_a[size][size])
{
    if (size == 2)
    {
        return mat_a[0][0] * mat_a[1][1] - mat_a[0][1] * mat_a[1][0];
    }

    float det = 0;
    float sub[size - 1][size - 1];
    int sign = 1;

    for (int i = 0; i < size; i++)
    {
        submatrix(size, mat_a, sub, 0, i);
        det += sign * mat_a[0][i] * mat_det(size - 1, sub);
        sign = -sign;
    }
    return det;
}

bool mat_is_invertible(int size, float mat[size][size])
{
    return !equal(mat_det(size, mat), 0);
}

void mat_inverse(int size, float mat[size][size], float out_mat[size][size])
{
    float det = mat_det(size, mat);
    if (det == 0)
        return;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            float c = mat_cofactor(size, mat, row, col);
            out_mat[col][row] = c / det;
        }
    }
}

Tuple mat_tuple_mult(float mat[4][4], Tuple tup)
{
    Tuple temp;
    temp.x = mat[0][0] * tup.x + mat[0][1] * tup.y + mat[0][2] * tup.z + mat[0][3] * tup.w;
    temp.y = mat[1][0] * tup.x + mat[1][1] * tup.y + mat[1][2] * tup.z + mat[1][3] * tup.w;
    temp.z = mat[2][0] * tup.x + mat[2][1] * tup.y + mat[2][2] * tup.z + mat[2][3] * tup.w;
    temp.w = mat[3][0] * tup.x + mat[3][1] * tup.y + mat[3][2] * tup.z + mat[3][3] * tup.w;

    return temp;
}

void mat_translate(float x, float y, float z, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);
    out[0][0] = 1;
    out[1][1] = 1;
    out[2][2] = 1;
    out[3][3] = 1;

    out[0][3] = x;
    out[1][3] = y;
    out[2][3] = z;
}

void mat_scale(float x, float y, float z, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);
    out[0][0] = x;
    out[1][1] = y;
    out[2][2] = z;
    out[3][3] = 1.0f;
}

void mat_rotate_x(float r, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);
    out[0][0] = 1.0f;
    out[1][1] = cosf(r);
    out[2][2] = cosf(r);
    out[1][2] = -sinf(r);
    out[2][1] = sinf(r);
    out[3][3] = 1.0f;
}

void mat_rotate_y(float r, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);
    out[0][0] = cosf(r);
    out[1][1] = 1.0;
    out[2][2] = cosf(r);
    out[3][3] = 1.0f;

    out[2][0] = -sinf(r);
    out[0][2] = sinf(r);
}

void mat_rotate_z(float r, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);

    out[0][0] = cosf(r);
    out[1][1] = cosf(r);
    out[2][2] = 1.0f;
    out[3][3] = 1.0f;

    out[0][1] = -sinf(r);
    out[1][0] = sinf(r);
}

void mat_sheer(float a, float b, float c, float d, float e, float f, float out[4][4])
{
    memset(out, 0, sizeof(float) * 16);

    out[0][0] = 1;
    out[1][1] = 1;
    out[2][2] = 1;
    out[3][3] = 1;

    out[0][1] = a;
    out[0][2] = b;

    out[1][0] = c;
    out[1][2] = d;

    out[2][0] = e;
    out[2][1] = f;
}

Ray ray(Tuple origin, Tuple direction)
{
    Ray ret = {.position = origin, .direction = direction};
    return ret;
}

Tuple ray_position(Ray r, double time)
{
    Tuple distance_moved = tuple_scale(r.direction, time);
    return tuple_add(r.position, distance_moved);
}
Sphere sphere()
{
    Sphere s = {.position = point(0, 0, 0), .radius = 1, .transform = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    return s;
}

Intersections intersect(Sphere s, Ray r1)
{
    float m[4][4];
    mat_inverse(4, s.transform, m);
    Ray ray = ray_transform(r1, m);
    Intersections ret;
    ret.count = 2;
    Tuple sphere_to_ray = tuple_sub(ray.position, s.position);

    float a = tuple_dot(ray.direction, ray.direction);
    float b = 2 * tuple_dot(ray.direction, sphere_to_ray);
    float c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;

    float discriminant = b * b - (4 * a * c);

    if (discriminant < 0)
    {
        ret.count = 0;
        return ret;
    }

    float t1 = (-1 * b - sqrt(discriminant)) / (2 * a);
    float t2 = (-1 * b + sqrt(discriminant)) / (2 * a);
    Intersection inter1 = intersection(t1, &s);
    Intersection inter2 = intersection(t2, &s);

    if (t1 > t2)
        return intersections(2, inter2, inter1);
    return intersections(2, inter1, inter2);
}

Intersection intersection(float time, void *object)
{
    Intersection ret = {.time = time, .object = object};
    return ret;
}
int comp(const void *a, const void *b)
{
    return ((Intersection *)a)->time > ((Intersection *)b)->time;
}

// This function ensures that the intersections are sorted by their time variable.
Intersections intersections(int count, ...)
{
    Intersections ret;
    ret.count = count;
    if (count <= 0)
        return ret;
    ret.intersections = malloc(count * sizeof(Intersection));

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

Ray ray_transform(Ray r, float m[4][4])
{
    r.position = mat_tuple_mult(m, r.position);
    r.direction = mat_tuple_mult(m, r.direction);
    return r;
}
void set_transform(void *object, float m[4][4])
{
    memcpy((((Sphere *)object)->transform), m, sizeof(((Sphere *)object)->transform));
}

Tuple sphere_normal_at(Sphere s, Tuple p)
{
    float mat[4][4];
    float mat2[4][4];
    mat_inverse(4, s.transform, mat);
    Tuple object_point = mat_tuple_mult(mat, p);
    Tuple object_normal = tuple_sub(object_point, point(0, 0, 0));
    mat_transpose(4, mat, mat2);
    Tuple world_normal = mat_tuple_mult(mat2, object_normal);
    world_normal.w = 0;
    return tuple_normalize(world_normal);
}