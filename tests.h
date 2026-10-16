#pragma once

#include "linear_algebra.h"

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
    Canvas can = new_canvas(1920, 2);
    Tuple my_color = read_pixel(&can, 0, 0);
    assert(tuple_equal(my_color, color(0, 0, 0)));

    // Test Write to canvas

    write_pixel(&can, 1900, 1, color(1, 0.2, 0.3));
    assert(tuple_equal(read_pixel(&can, 1900, 1), color(1, 0.2, 0.3)));

    // Test write to ppm
    const int size = 80;
    Canvas canvas = new_canvas(size, size);
    Tuple red = color(0.9, 0.3, 0.3);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            write_pixel(&canvas, j, i, color(.6, i / 30.0, j / 30.0));
        }
    }
    canvas_to_ppm(&canvas, "output.ppm");
    // Test Physics Sim thing
    // Tuple black = color(0, 0, 0);
    Canvas physics_canvas = new_canvas(size, size);
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
            write_pixel(&physics_canvas, x, size - y - 1, red);
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
    // struct ray Tests
    struct ray ray_1 = (struct ray){point(2, 3, 4), vector(1, 0, 0)};
    assert(tuple_equal(ray_position(ray_1, 0), point(2, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, 1), point(3, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, -1), point(1, 3, 4)));
    assert(tuple_equal(ray_position(ray_1, 2.5), point(4.5, 3, 4)));

    // Sphere Intersection Tests
    struct ray ray1 = (struct ray){point(0, 0, 0), vector(0, 0, 1)};
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

    struct ray r1 = (struct ray){point(1, 2, 3), vector(0, 1, 0)};
    float m[16];
    mat_translate(3, 4, 5, m);
    struct ray r2 = ray_transform(r1, m);
    assert(tuple_equal(r2.position, point(4, 6, 8)));
    assert(tuple_equal(r2.direction, vector(0, 1, 0)));

    // Scenario: Scaling a ray
    r1 = (struct ray){point(1, 2, 3), vector(0, 1, 0)};
    mat_scale(2, 3, 4, m);
    r2 = ray_transform(r1, m);
    assert(tuple_equal(r2.position, point(2, 6, 12)));
    assert(tuple_equal(r2.direction, vector(0, 3, 0)));
    // Scenario: A sphere's default transformation
    s = sphere();
    mat_identity_4x4(m);
    assert(mat_equal(4, s.transform, m));

    // Scenario: Changing a sphere's transformation
    mat_translate(2, 3, 4, m);
    set_transform(&s, m);
    assert(mat_equal(4, s.transform, m));

    // Scenario: Intersecting a scaled sphere with a ray
    struct ray r = (struct ray){point(0, 0, -5), vector(0, 0, 1)};
    s = sphere();
    mat_scale(2, 2, 2, m);
    set_transform(&s, m);
    xs = intersect(&s, r);
    assert(xs.count == 2);
    assert(xs.count == 2);
    assert(xs.intersections[0].time == 3);
    assert(xs.intersections[1].time == 7);

    // A program that casts rays at a sphere and draw the picture to a canvas.

    render_circle();

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
    struct material material_1 = s.material;
    assert(materials_equal(material_1, material(color(1, 1, 1), 0.1, 0.9, 0.9, 200.0)));

    // Scenario: Lighting with the eye between the light and the surface

    Tuple eyev = vector(0, 0, -1);
    Tuple normalv = vector(0, 0, -1);
    PointLight light = point_light(point(0, 0, -10), color(1, 1, 1));

    result = lighting(material_1, light, point(0, 0, 0), eyev, normalv, false);
    assert(tuple_equal(color(1.9, 1.9, 1.9), result));

    // Scenario: Lighting with eye in the path of the reflection vector
    eyev = vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
    normalv = vector(0, 0, -1);
    light = point_light(point(0, 10, -10), color(1, 1, 1));
    result = lighting(material_1, light, point(0, 0, 0), eyev, normalv, false);
    assert(tuple_equal(color(1.6364, 1.6364, 1.6364), result));

    // Scenario: Intersect a world with a ray
    world_t world = default_world();
    r = (struct ray){point(0, 0, -5), vector(0, 0, 1)};

    intersect_world(world, r, &xs);
    assert(xs.count == 4);
    assert(xs.intersections[0].time == 4);
    assert(xs.intersections[1].time == 4.5);
    assert(xs.intersections[2].time == 5.5);
    assert(xs.intersections[3].time == 6);

    // Scenario: Precomputing the state of an intersection
    r = (struct ray){point(0, 0, -5), vector(0, 0, 1)};
    Sphere shape = sphere();
    Intersection i = intersection(4, &shape);
    Computation comps = prepare_computations(i, r);
    assert(comps.time == i.time);
    assert(comps.object == i.object);
    // assert(tuple_equal(comps.point, point(0, 0, -1)));
    assert(tuple_equal(comps.eyev, vector(0, 0, -1)));
    assert(tuple_equal(comps.normalv, vector(0, 0, -1)));

    // Scenario: The hit, when an intersection occurs on the outside
    r = (struct ray){point(0, 0, -5), vector(0, 0, 1)};
    shape = sphere();
    i = intersection(4, &shape);
    comps = prepare_computations(i, r);
    // assert(comps.inside == false);

    // Scenario: The hit, when an intersection occurs on the inside
    r = (struct ray){point(0, 0, 0), vector(0, 0, 1)};
    shape = sphere();
    i = intersection(1, &shape);
    comps = prepare_computations(i, r);
    // assert(tuple_equal(comps.point, point(0, 0, 1)));
    assert(tuple_equal(comps.eyev, vector(0, 0, -1)));
    assert(tuple_equal(comps.normalv, vector(0, 0, -1)));
    assert(comps.inside == true);

    // Scenario: Shading an intersection
    world_t w = default_world();
    r = (struct ray){point(0, 0, -5), vector(0, 0, 1)};
    shape = w.spheres[0];
    i = intersection(4, &shape);
    comps = prepare_computations(i, r);
    c = shade_hit(w, comps);
    // assert(tuple_equal(c, color(0.38066, 0.47583, 0.2855)));

    // Scenario: Shading an intersection from the inside
    w = default_world();
    w.light = point_light(point(0, 0.25, 0), color(1, 1, 1));
    r = (struct ray){point(0, 0, 0), vector(0, 0, 1)};
    shape = w.spheres[1];
    i = intersection(0.5, &shape);
    comps = prepare_computations(i, r);
    c = shade_hit(w, comps);
    // assert(tuple_equal(c, color(0.90498, 0.90498, 0.90498)));

    // Scenario: The color with an intersection behind the ray
    w = default_world();
    Sphere *outer = &w.spheres[0];
    outer->material.ambient = 1;
    Sphere *inner = &w.spheres[1];
    inner->material.ambient = 1;
    r = (struct ray){point(0, 0, 0.75), vector(0, 0, -1)};
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
    struct camera cam = camera(hsize, vsize, field_of_view);
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

    mat_mult(4, mat_1, trans_mat, mat_2);
    set_camera_transform(&cam, mat_2);
    r = ray_for_pixel(cam, 100, 50);
    assert(tuple_equal(r.position, point(0, 2, -5)));
    assert(tuple_equal(r.direction, vector(sqrt(2) / 2, 0, -sqrt(2) / 2)));

    // Scenario: Rendering a world with a camera
    w = default_world();
    cam = camera(11, 11, M_PI / 2);
    from = point(0, 0, -5);
    to = point(0, 0, 0);
    up = vector(0, 1, 0);
    view_transform(from, to, up, mat_1);
    set_camera_transform(&cam, mat_1);

    Canvas image = render(cam, w);
    // assert(tuple_equal(read_pixel(&image, 5, 5), color(0.38066, 0.47583, 0.2855)));

    w = default_world();
    w.spheres[1].material.color = color(0.8, 0.3, 0.8);
    cam = camera(410, 410, M_PI / 2);
    from = point(0, 0, -0.8);
    to = point(0, 0, 0);
    up = vector(0, 1, 0);
    view_transform(from, to, up, mat_2);
    set_camera_transform(&cam, mat_2);

    image = render(cam, w);
    canvas_to_ppm(&image, "2_thing.ppm");
    Canvas scene = render_scene();
    canvas_to_ppm(&scene, "scene.ppm");

    // Shadows

    eyev = vector(0, 0, -1);
    normalv = vector(0, 0, -1);
    light = point_light(point(0, 0, -10), color(1, 1, 1));
    bool in_shadow = true;
    result = lighting(material_1, light, point(0, 0, 0), eyev, normalv, in_shadow);
    printf("result r, g, b: %f, %f, %f\n", result.x, result.y, result.z);
    assert(tuple_equal(result, color(0.1, 0.1, 0.1)));

    // there is no shadow when nothing is collinear with the point.
    w = default_world();
    Tuple p = point(0, 10, 0);
    assert(is_shadowed(w, p) == false);

    // light is between point and object

    p = point(10, -10, 10);
    assert(is_shadowed(w, p) == true);
    // There is no shadow when an object is behind the light
    p = point(-20, 20, -20);
    assert(is_shadowed(w, p) == false);

    // Scenario: shade_hit() is given an intersection in shadow
    w = new_world();
    w.light = point_light(point(0, 0, -10), color(1, 1, 1));
    Sphere s1 = sphere();
    w.spheres[0] = s1;
    Sphere s2 = sphere();
    mat_translate(0, 0, 0, s2.transform);
    w.spheres[1] = s2;
    w.num_spheres = 2;
    r = (struct ray){.position = point(0, 0, 5), .direction = vector(0, 0, 1)};
    i = intersection(4, &s2);
    comps = prepare_computations(i, r);
    c = shade_hit(w, comps);
    assert(tuple_equal(color(0.1, 0.1, 0.1), c));
}