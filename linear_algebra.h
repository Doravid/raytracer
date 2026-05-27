#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdalign.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define EPSILON (0.0001)

typedef struct
{
    alignas(32) float m[16];
} Matrix4x4;

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
typedef struct material
{
    Tuple color;
    float ambient, diffuse, specular, shininess;
} Material;
typedef struct sphere
{
    Tuple position;
    float radius;
    float transform[4 * 4];
    float inverse_transform[4 * 4];
    Material material;
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

typedef struct point_light
{
    Tuple intensity;
    Tuple position;
} PointLight;
typedef struct world
{
    Sphere *spheres;
    int num_spheres;

    PointLight light;
} World;
typedef struct computation
{
    void *object;
    float time;
    Tuple point, eyev, normalv;
    bool inside;
} Computation;
typedef struct camera
{
    int hsize, vsize;
    float field_of_view, pixel_size, half_width, half_height;
    float transform[16];
} Camera;

Material
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
Canvas canvas(int width, int height);
Tuple color(float red, float green, float blue);
void write_pixel(Canvas *can, int x, int y, Tuple col);
void canvas_to_ppm(Canvas *can, char *file_name);
void mat_identity(float *out);
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
void mat_rotate_y(float r, float *out);
void mat_rotate_z(float r, float *out);
void mat_scale(float x, float y, float z, float *out);
void mat_sheer(float a, float b, float c, float d, float e, float f, float *out);
Ray ray(Tuple origin, Tuple direction);
Tuple ray_position(Ray r, double time);
Ray ray_transform(Ray r, float *m);
Sphere sphere();
Intersections intersect(Sphere *s, Ray r1);
Intersection intersection(float time, void *object);
Intersections intersections(int count, ...);
Intersection hit(Intersections inters);
void set_transform(void *object, float *m);
Tuple sphere_normal_at(Sphere s, Tuple p);
Tuple reflect(Tuple incoming, Tuple normal);
bool materials_equal(Material mat1, Material mat2);
Tuple lighting(Material material, PointLight light, Tuple position, Tuple eye_vector, Tuple normal_vector);
PointLight point_light(Tuple position, Tuple intensity);
void render_circle();
void render_sphere(float hight, char *file_name);
World world();
World default_world();
Intersections intersect_world(World world, Ray ray);
Computation prepare_computations(Intersection inter, Ray r1);
Tuple shade_hit(World w, Computation comps);
Tuple color_at(World w, Ray r);
void view_transform(Tuple from, Tuple to, Tuple up, float output_matrix[4 * 4]);
Camera camera(int hsize, int vsize, float field_of_view);
Ray ray_for_pixel(Camera c, int x, int y);
Canvas render(Camera c, World w);

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
    float my_matrix[4 * 4] = {1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5};
    float my_matrix2[4 * 4] = {1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.5};
    float my_matrix3[4 * 4] = {1, 2, 3, 4, 5.5, 6.5, 7.5, 8.5, 9, 10, 11, 12, 13.5, 14.5, 15.5, 16.54};

    assert(my_matrix[0] == 1);
    assert(my_matrix[3] == 4);
    assert(my_matrix[6] == 7.5);

    assert(mat_equal(4, my_matrix, my_matrix2));
    assert(!mat_equal(4, my_matrix, my_matrix3));
    // Test Matrix Mult
    float mat_1[4 * 4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2};
    float mat_2[16] = {-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8};

    float res_matrix[16];
    mat_mult(4, mat_1, mat_2, res_matrix);

    float my_mat[16] = {20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42};
    assert(mat_equal(4, my_mat, res_matrix));

    // Test Matrix x Tuple
    float mat_tuple[16] = {1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1};
    Tuple my_tuple = tuple(1, 2, 3, 1);
    Tuple res_tuple = mat_tuple_mult(mat_tuple, my_tuple);
    assert(tuple_equal(res_tuple, tuple(18, 24, 33, 1)));

    // Test Identity
    float identity_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    mat_mult(4, mat_1, identity_matrix, res_matrix);
    assert(mat_equal(4, mat_1, res_matrix));
    // Test Transpose
    float mat_a[16] = {0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8};
    float transpose_mat_a[16] = {0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8};
    mat_transpose(4, mat_a, res_matrix);
    assert(mat_equal(4, transpose_mat_a, res_matrix));
    // Test Submatrix
    float mat_3_3[9] = {1, 5, 0, -3, 2, 7, 0, 6, -3};
    float res_2_2[4];
    submatrix(3, mat_3_3, res_2_2, 0, 2);
    float ref_2_2[4] = {-3, 2, 0, 6};
    assert(mat_equal(2, ref_2_2, res_2_2));

    float mat_b[16] = {-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1};
    float res_3_3[9];
    submatrix(4, mat_b, res_3_3, 2, 1);
    float ref_3_3[9] = {-6, 1, 6, -8, 8, 6, -7, -1, 1};
    assert(mat_equal(3, ref_3_3, res_3_3));
    // Test Matrix DET
    float A[9] = {1, 2, 6, -5, 8, -4, 2, 6, 4};
    float B[16] = {-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9};
    float x_val = mat_det(3, A);
    float y_val = mat_det(4, B);
    assert(x_val == -196);
    assert(y_val == -4071);
    // Test Matrix Inverse
    float A1[16] = {8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4};
    float expected_inv1[16] = {-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308, 0.02564, 0.03077, 0.35897, 0.35897, 0.43590, 0.92308, -0.69231, -0.69231, -0.76923, -1.92308};
    float out_inv1[16];
    mat_inverse(4, A1, out_inv1);
    assert(mat_equal(4, expected_inv1, out_inv1));

    float A2[16] = {9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6, 4, -7, 6, 6, 2};
    float expected_inv2[16] = {-0.04074, -0.07778, 0.14444, -0.22222, -0.07778, 0.03333, 0.36667, -0.33333, -0.02901, -0.14630, -0.10926, 0.12963, 0.17778, 0.06667, -0.26667, 0.33333};
    float out_inv2[16];
    mat_inverse(4, A2, out_inv2);

    // Test Matrix Translate
    float trans_mat[16];
    float rev_trans_mat[16];
    mat_translate(5, -3, 2, trans_mat);
    mat_inverse(4, trans_mat, rev_trans_mat);

    Tuple some_point = point(-3, 4, 5);
    Tuple output = mat_tuple_mult(trans_mat, some_point);
    Tuple origin = mat_tuple_mult(rev_trans_mat, output);

    assert(tuple_equal(output, point(2, 1, 7)));
    assert(tuple_equal(some_point, origin));

    assert(mat_equal(4, expected_inv2, out_inv2));
    // Scaling
    float scale_mat[16];
    mat_scale(2, 3, 4, scale_mat);
    Tuple p_1 = point(-4, 6, 8);
    Tuple v_1 = vector(-4, 6, 8);

    Tuple scaled_point = mat_tuple_mult(scale_mat, p_1);
    Tuple scaled_vector = mat_tuple_mult(scale_mat, v_1);

    assert(tuple_equal(scaled_point, point(-8, 18, 32)));
    assert(tuple_equal(scaled_vector, vector(-8, 18, 32)));

    // Rotation
    Tuple test_point = point(0, 1, 0);
    float half_quarter[16];
    float full_quarter[16];
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
    float sheer[16];
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
    Intersections xs = intersect(&s, ray1);

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
    float m[16];
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
    xs = intersect(&s, r);
    assert(xs.count == 2);
    assert(xs.count == 2);
    assert(xs.intersections[0].time == 3);
    assert(xs.intersections[1].time == 7);

    // A program that casts rays at a sphere and draw the picture to a canvas.

    // render_circle();

    s = sphere();
    Tuple normal = sphere_normal_at(s, point(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    assert(tuple_equal(normal, vector(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3)));
    // Scenario: Computing the normal on a translated sphere
    s = sphere();
    float sphere_mat[16];
    mat_translate(0, 1, 0, sphere_mat);
    set_transform(&s, sphere_mat);
    normal = sphere_normal_at(s, point(0, 1.70711, -0.70711));
    assert(tuple_equal(normal, vector(0, 0.70711, -0.70711)));

    // Scenario: Reflecting a vector approaching at 45°
    v = vector(1, -1, 0);
    Tuple n = vector(0, 1, 0);
    Tuple res = reflect(v, n);

    // Scenario: Reflecting a vector off a slanted surface
    v = vector(0, -1, 0);
    n = vector(sqrt(2) / 2, sqrt(2) / 2, 0);
    res = reflect(v, n);
    assert(tuple_equal(res, vector(1, 0, 0)));

    // Scenario: A sphere has a default material
    s = sphere();
    Material material_1 = s.material;
    assert(materials_equal(material_1, material(color(1, 1, 1), 0.1, 0.9, 0.9, 200.0)));

    // Scenario: Lighting with the eye between the light and the surface

    Tuple eyev = vector(0, 0, -1);
    Tuple normalv = vector(0, 0, -1);
    PointLight light = point_light(point(0, 0, -10), color(1, 1, 1));

    result = lighting(material_1, light, point(0, 0, 0), eyev, normalv);

    assert(tuple_equal(color(1.9, 1.9, 1.9), result));

    // Scenario: Lighting with eye in the path of the reflection vector
    eyev = vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
    normalv = vector(0, 0, -1);
    light = point_light(point(0, 10, -10), color(1, 1, 1));
    result = lighting(material_1, light, point(0, 0, 0), eyev, normalv);
    assert(tuple_equal(color(1.6364, 1.6364, 1.6364), result));
    char filepath[64];
    // Put it together Chapter 6
    for (int frame = 0; frame < 1; frame++)
    {
        snprintf(filepath, sizeof(filepath) - 1, "cool_sphere-%03d.ppm", frame);
        render_sphere((float)(frame) / 20, filepath);
    }

    // Scenario: Intersect a world with a ray
    World world = default_world();
    r = ray(point(0, 0, -5), vector(0, 0, 1));

    xs = intersect_world(world, r);
    assert(xs.count == 4);
    assert(xs.intersections[0].time == 4);
    assert(xs.intersections[1].time == 4.5);
    assert(xs.intersections[2].time == 5.5);
    assert(xs.intersections[3].time == 6);

    // Scenario: Precomputing the state of an intersection
    r = ray(point(0, 0, -5), vector(0, 0, 1));
    Sphere shape = sphere();
    Intersection i = intersection(4, &shape);
    Computation comps = prepare_computations(i, r);
    assert(comps.time == i.time);
    assert(comps.object == i.object);
    assert(tuple_equal(comps.point, point(0, 0, -1)));
    assert(tuple_equal(comps.eyev, vector(0, 0, -1)));
    assert(tuple_equal(comps.normalv, vector(0, 0, -1)));

    // Scenario: The hit, when an intersection occurs on the outside
    r = ray(point(0, 0, -5), vector(0, 0, 1));
    shape = sphere();
    i = intersection(4, &shape);
    comps = prepare_computations(i, r);
    assert(comps.inside == false);

    // Scenario: The hit, when an intersection occurs on the inside
    r = ray(point(0, 0, 0), vector(0, 0, 1));
    shape = sphere();
    i = intersection(1, &shape);
    comps = prepare_computations(i, r);
    assert(tuple_equal(comps.point, point(0, 0, 1)));
    assert(tuple_equal(comps.eyev, vector(0, 0, -1)));
    assert(tuple_equal(comps.normalv, vector(0, 0, -1)));
    assert(comps.inside == true);

    // Scenario: Shading an intersection
    World w = default_world();
    r = ray(point(0, 0, -5), vector(0, 0, 1));
    shape = w.spheres[0];
    i = intersection(4, &shape);
    comps = prepare_computations(i, r);
    c = shade_hit(w, comps);
    assert(tuple_equal(c, color(0.38066, 0.47583, 0.2855)));

    // Scenario: Shading an intersection from the inside
    w = default_world();
    w.light = point_light(point(0, 0.25, 0), color(1, 1, 1));
    r = ray(point(0, 0, 0), vector(0, 0, 1));
    shape = w.spheres[1];
    i = intersection(0.5, &shape);
    comps = prepare_computations(i, r);
    c = shade_hit(w, comps);
    assert(tuple_equal(c, color(0.90498, 0.90498, 0.90498)));

    // Scenario: The color with an intersection behind the ray
    w = default_world();
    Sphere *outer = &w.spheres[0];
    outer->material.ambient = 1;
    Sphere *inner = &w.spheres[1];
    inner->material.ambient = 1;
    r = ray(point(0, 0, 0.75), vector(0, 0, -1));
    c = color_at(w, r);
    assert(tuple_equal(c, inner->material.color));

    //  Scenario: The transformation matrix for the default orientation
    Tuple from = point(0, 0, 0);
    Tuple to = point(0, 0, -1);
    Tuple up = vector(0, 1, 0);

    view_transform(from, to, up, mat_1);
    assert(mat_equal(4, mat_1, identity_matrix));

    // Scenario: A view transformation matrix looking in positive z direction
    from = point(0, 0, 0);
    to = point(0, 0, 1);
    up = vector(0, 1, 0);
    view_transform(from, to, up, mat_1);

    mat_scale(-1, 1, -1, res_matrix);

    assert(mat_equal(4, res_matrix, mat_1));

    // Scenario: An arbitrary view transformation
    from = point(1, 3, 2);
    to = point(4, -2, 8);
    up = vector(1, 1, 0);
    view_transform(from, to, up, mat_1);

    float mat_ref[16] = {-0.50709, 0.50709, 0.67612, -2.36643, 0.76772, 0.60609, 0.12122, -2.82843, -0.35857, 0.59761, -0.71714, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000};
    assert(mat_equal(4, mat_1, mat_ref));

    // Scenario: Constructing a camera
    int hsize = 160, vsize = 120;
    float field_of_view = M_PI / 2;
    Camera cam = camera(hsize, vsize, field_of_view);
    assert(cam.hsize == 160);
    assert(cam.vsize == 120);
    assert(equal(cam.field_of_view, M_PI / 2));
    assert(mat_equal(4, cam.transform, identity_matrix));

    // Scenario: The pixel size for a horizontal canvas
    cam = camera(200, 125, M_PI / 2.0);
    assert(equal(cam.pixel_size, 0.01));

    // Scenario: The pixel size for a vertical canvas
    cam = camera(125, 200, M_PI / 2.0);
    assert(equal(cam.pixel_size, 0.01));

    // Scenario: Constructing a ray through a corner of the canvas
    cam = camera(201, 101, M_PI / 2);
    r = ray_for_pixel(cam, 0, 0);
    assert(tuple_equal(r.position, point(0, 0, 0)));
    assert(tuple_equal(r.direction, vector(0.66519, 0.33259, -0.66851)));

    // Scenario: Constructing a ray when the camera is transformed
    cam = camera(201, 101, M_PI / 2);
    mat_rotate_y(M_PI / 4, mat_1);
    mat_translate(0, -2, 5, trans_mat);
    mat_mult(4, mat_1, trans_mat, cam.transform);
    r = ray_for_pixel(cam, 100, 50);
    assert(tuple_equal(r.position, point(0, 2, -5)));
    assert(tuple_equal(r.direction, vector(sqrt(2) / 2, 0, -sqrt(2) / 2)));

    // Scenario: Rendering a world with a camera
    w = default_world();
    cam = camera(11, 11, M_PI / 2);
    from = point(0, 0, -5);
    to = point(0, 0, 0);
    up = vector(0, 1, 0);
    view_transform(from, to, up, cam.transform);

    Canvas image = render(cam, w);
    assert(tuple_equal(image.canvas[5][5], color(0.38066, 0.47583, 0.2855)));

    w = default_world();
    w.spheres[1].material.color = color(0.8, 0.3, 0.8);
    cam = camera(410, 410, M_PI / 2);
    from = point(0, 0, -0.8);
    to = point(0, 0, 0);
    up = vector(0, 1, 0);
    view_transform(from, to, up, cam.transform);

    image = render(cam, w);
    canvas_to_ppm(&image, "2_thing.ppm");
}

void render_circle()
{
    const int SIZE = 480;
    Canvas my_canvas = canvas(SIZE, SIZE);
    Tuple red = color(.95, 0.2, 0.2);
    Tuple camera_origin = point(0, 0, 0);
    Tuple pixel_point, camera_vector;
    Ray color_ray;
    Sphere cool_red_sphere = sphere();
    Tuple black = color(0, 0, 0);
    float sphere_mat[16];

    mat_translate(0, 0, 2.0, sphere_mat);
    set_transform(&cool_red_sphere, sphere_mat);

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            pixel_point = point((float)(x - (SIZE / 2)) / SIZE, (float)(y - (SIZE / 2)) / SIZE, 0.75);
            camera_vector = tuple_sub(pixel_point, camera_origin);
            color_ray = ray(camera_origin, camera_vector);

            Intersections sphere_intersects = intersect(&cool_red_sphere, color_ray);
            if (sphere_intersects.count > 0)
                my_canvas.canvas[x][y] = red;
            else
                my_canvas.canvas[x][y] = black;
        }
    }
    canvas_to_ppm(&my_canvas, "cool_circle.ppm");
}
void render_sphere(float hight, char *file_name)
{
    const int SIZE = 240;
    Canvas my_canvas = canvas(SIZE, SIZE);
    Tuple camera_origin = point(0, 0, 0);
    Tuple pixel_point, camera_vector;
    Ray color_ray;
    Sphere cool_red_sphere = sphere();
    Tuple black = color(0, 0, 0);
    camera_origin = point(0, hight, 0);
    cool_red_sphere = sphere();
    cool_red_sphere.material.color = color(0.85, 0.39, 0.18);
    PointLight my_point_light = point_light(point(-10, 10, -10), color(1.0, 0.9, 0.95));
    black = color(0, 0, 0);
    float sphere_mat[16];
    mat_translate(0, 0, 2.0, sphere_mat);
    set_transform(&cool_red_sphere, sphere_mat);

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            pixel_point = point((float)(x - (SIZE / 2)) / SIZE, (float)((SIZE / 2) - y) / SIZE, 0.75);
            camera_vector = tuple_normalize(tuple_sub(pixel_point, camera_origin));
            color_ray = ray(camera_origin, camera_vector);

            Intersections sphere_intersects = intersect(&cool_red_sphere, color_ray);
            if (sphere_intersects.count > 0)
            {
                float hit_time = sphere_intersects.intersections[0].time;
                Sphere *hit_object = (Sphere *)sphere_intersects.intersections[0].object;

                Tuple hit_pos = ray_position(color_ray, hit_time);
                Tuple s_normal = sphere_normal_at(*hit_object, hit_pos);
                Tuple eye = tuple_negate(color_ray.direction);

                my_canvas.canvas[x][y] = lighting(hit_object->material, my_point_light, hit_pos, eye, s_normal);
            }
            else
                my_canvas.canvas[x][y] = black;
        }
    }
    canvas_to_ppm(&my_canvas, file_name);
}
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
void mat_identity(float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1;
    out[5] = 1;
    out[10] = 1;
    out[15] = 1;
}
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

void mat_transpose(int size, float *mat_a, float *mat_out)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mat_out[i * size + j] = mat_a[j * size + i];
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
float mat_det_4x4(float *m)
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

float mat_det_3x3(float *m)
{
    return m[0] * (m[4] * m[8] - m[5] * m[7]) -
           m[1] * (m[3] * m[8] - m[5] * m[6]) +
           m[2] * (m[3] * m[7] - m[4] * m[6]);
}

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

bool mat_is_invertible(int size, float *mat)
{
    return !equal(mat_det(size, mat), 0);
}
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

Tuple mat_tuple_mult(float *mat, Tuple tup)
{
    Tuple temp;
    temp.x = mat[0] * tup.x + mat[1] * tup.y + mat[2] * tup.z + mat[3] * tup.w;
    temp.y = mat[4] * tup.x + mat[5] * tup.y + mat[6] * tup.z + mat[7] * tup.w;
    temp.z = mat[8] * tup.x + mat[9] * tup.y + mat[10] * tup.z + mat[11] * tup.w;
    temp.w = mat[12] * tup.x + mat[13] * tup.y + mat[14] * tup.z + mat[15] * tup.w;
    return temp;
}

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

void mat_scale(float x, float y, float z, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = x;
    out[5] = y;
    out[10] = z;
    out[15] = 1.0f;
}

void mat_rotate_x(float r, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = 1.0f;
    out[5] = cosf(r);
    out[10] = cosf(r);
    out[6] = -sinf(r);
    out[9] = sinf(r);
    out[15] = 1.0f;
}

void mat_rotate_y(float r, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = cosf(r);
    out[5] = 1.0;
    out[10] = cosf(r);
    out[15] = 1.0f;
    out[8] = -sinf(r);
    out[2] = sinf(r);
}

void mat_rotate_z(float r, float *out)
{
    memset(out, 0, sizeof(float) * 16);
    out[0] = cosf(r);
    out[5] = cosf(r);
    out[10] = 1.0f;
    out[15] = 1.0f;
    out[1] = -sinf(r);
    out[4] = sinf(r);
}

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
    Sphere s = {.position = point(0, 0, 0), .radius = 1, .transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .inverse_transform = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, .material = material(color(1, 1, 1), 0.1, 0.9, 0.9, 200.0)};
    return s;
}

Intersections intersect(Sphere *s, Ray r1)
{

    Ray ray = ray_transform(r1, s->inverse_transform);
    Intersections ret;
    ret.count = 2;
    Tuple sphere_to_ray = tuple_sub(ray.position, s->position);

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
    Intersection inter1 = intersection(t1, s);
    Intersection inter2 = intersection(t2, s);

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

Ray ray_transform(Ray r, float *m)
{
    r.position = mat_tuple_mult(m, r.position);
    r.direction = mat_tuple_mult(m, r.direction);
    return r;
}
void set_transform(void *object, float *m)
{
    memcpy((((Sphere *)object)->transform), m, sizeof(float) * 16);
    float inverse[16];
    mat_inverse(4, m, inverse);
    memcpy((((Sphere *)object)->inverse_transform), inverse, sizeof(float) * 16);
}

Tuple sphere_normal_at(Sphere s, Tuple p)
{
    float mat2[16];
    Tuple object_point = mat_tuple_mult(s.inverse_transform, p);
    Tuple object_normal = tuple_sub(object_point, point(0, 0, 0));
    mat_transpose(4, s.inverse_transform, mat2);
    Tuple world_normal = mat_tuple_mult(mat2, object_normal);
    world_normal.w = 0;
    return tuple_normalize(world_normal);
}

Tuple reflect(Tuple incoming, Tuple normal)
{
    return tuple_sub(incoming, tuple_scale(normal, 2 * tuple_dot(incoming, normal)));
}

PointLight point_light(Tuple position, Tuple intensity)
{
    PointLight light = {.intensity = intensity, .position = position};
    return light;
}

Material material(Tuple color, float ambient, float diffuse, float specular, float shininess)
{
    Material m = {.color = color, .ambient = ambient, .diffuse = diffuse, .specular = specular, .shininess = shininess};
    return m;
}

bool materials_equal(Material mat1, Material mat2)
{
    return equal(mat2.ambient, mat1.ambient) && tuple_equal(mat1.color, mat2.color) && equal(mat2.diffuse, mat1.diffuse) && equal(mat2.shininess, mat1.shininess) && equal(mat2.specular, mat1.specular);
}

Tuple lighting(Material material, PointLight light, Tuple position, Tuple eye_vector, Tuple normal_vector)
{
    Tuple effective_color = tuple_hadamard_product(material.color, light.intensity);
    Tuple light_vector = tuple_normalize(tuple_sub(light.position, position));
    float light_dot_normal = tuple_dot(light_vector, normal_vector);
    Tuple ambient = tuple_scale(effective_color, material.ambient);
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
            float factor = pow(reflection_dot_eye, material.shininess);
            specular = tuple_scale(light.intensity, material.specular * factor);
        }
    }

    return tuple_add(diffuse, tuple_add(specular, ambient));
}

World world()
{
    World wor = {.light = point_light(point(0, 0, 0), color(0, 0, 0)), .spheres = malloc(sizeof(Sphere) * 10), .num_spheres = 0};
    return wor;
}

World default_world()
{
    World wor = {.light = point_light(point(-10, 10, -10), color(1, 1, 1)), .spheres = malloc(sizeof(Sphere) * 10), .num_spheres = 0};
    Sphere s1 = sphere();
    s1.material = material(color(0.8, 1, 0.6), 0.1, 0.7, 0.2, 200.0);
    float s2_mat[4 * 4];
    mat_scale(0.5, 0.5, 0.5, s2_mat);
    Sphere s2 = sphere();
    set_transform(&s2, s2_mat);

    wor.spheres[0] = s1;
    wor.spheres[1] = s2;
    wor.num_spheres = 2;

    return wor;
}

Intersections intersect_world(World world, Ray r)
{

    Intersections ret;
    ret.intersections = malloc(sizeof(Intersection) * world.num_spheres * 2);
    ret.count = 0;
    int offset_counter = 0;
    for (int i = 0; i < world.num_spheres; i++)
    {
        Ray ray = ray_transform(r, world.spheres[i].inverse_transform);
        Tuple sphere_to_ray = tuple_sub(ray.position, world.spheres[i].position);
        float a = tuple_dot(ray.direction, ray.direction);
        float b = 2 * tuple_dot(ray.direction, sphere_to_ray);
        float c = tuple_dot(sphere_to_ray, sphere_to_ray) - 1;

        float discriminant = b * b - (4 * a * c);

        if (discriminant < 0)
        {
            offset_counter++;
            continue;
        }
        ret.count += 2;

        float t1 = (-1 * b - sqrt(discriminant)) / (2 * a);
        float t2 = (-1 * b + sqrt(discriminant)) / (2 * a);
        Intersection inter1 = intersection(t1, &world.spheres[i]);
        Intersection inter2 = intersection(t2, &world.spheres[i]);

        ret.intersections[(i - offset_counter) * 2] = inter1;
        ret.intersections[(i - offset_counter) * 2 + 1] = inter2;
    }
    qsort(ret.intersections, ret.count, sizeof(Intersection), comp);
    return ret;
}

Computation prepare_computations(Intersection inter, Ray r1)
{
    // Instantiate a data structure for storing some precomputed values
    Computation comps;

    // Copy the intersection's properties, for convenience
    comps.time = inter.time;
    comps.object = inter.object;
    // Pre-compute some useful values
    comps.point = ray_position(r1, comps.time);
    comps.eyev = tuple_negate(r1.direction);
    comps.normalv = sphere_normal_at(*(Sphere *)comps.object, comps.point);
    comps.inside = (tuple_dot(comps.normalv, comps.eyev) < 0);
    if (comps.inside)
        comps.normalv = tuple_negate(comps.normalv);
    return comps;
}

Tuple shade_hit(World w, Computation comps)
{
    return lighting(((Sphere *)comps.object)->material, w.light, comps.point, comps.eyev, comps.normalv);
}

Tuple color_at(World w, Ray r)
{
    Intersections inters = intersect_world(w, r);
    Intersection h = hit(inters);
    if (inters.count == 0)
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
Camera camera(int hsize, int vsize, float field_of_view)
{
    Camera c = {.hsize = hsize, .vsize = vsize, .field_of_view = field_of_view};
    mat_identity(c.transform);
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

Ray ray_for_pixel(Camera c, int x, int y)
{
    float xoffset, yoffset;

    xoffset = (x + 0.5) * c.pixel_size;
    yoffset = (y + 0.5) * c.pixel_size;

    float world_x = c.half_width - xoffset;
    float world_y = c.half_height - yoffset;
    float inverse[16];
    mat_inverse_4x4(c.transform, inverse);
    Tuple pixel = mat_tuple_mult(inverse, point(world_x, world_y, -1));
    Tuple origin = mat_tuple_mult(inverse, point(0, 0, 0));
    Tuple direction = tuple_normalize(tuple_sub(pixel, origin));

    return ray(origin, direction);
}

Canvas render(Camera c, World w)
{
    Canvas image = canvas(c.hsize, c.vsize);
    for (int y = 0; y < c.vsize; y++)
    {
        for (int x = 0; x < c.hsize; x++)
        {
            Ray ray = ray_for_pixel(c, x, y);
            Tuple color = color_at(w, ray);
            write_pixel(&image, x, y, color);
        }
    }

    return image;
}