#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPSILON (0.00001)
struct tuple
{
    float x, y, z, w;
} typedef Tuple;

struct canvas
{
    Tuple **canvas;
    int width;
    int height;
} typedef Canvas;

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
char *canvas_to_ppm(Canvas *can, char *file_name);
void write_ppm_to_file(char *data, int width, int height, char *file_name);
bool mat_equal(int size, float mat_a[size][size], float mat_b[size][size]);
void mat_mult(int size, float mat_a[size][size], float mat_b[size][size], float mat_out[size][size]);

int main(int argc, char const *argv[])
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
    Tuple col = can.canvas[1900][1];
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
    char *image = canvas_to_ppm(&new_canvas, "output.ppm");
    // Test Physics Sim thing
    Tuple black = color(0, 0, 0);
    char filename[25];
    Canvas physics_canvas = canvas(size, size);
    Tuple position = point(0, 0, 0);
    Tuple velocity = vector(5, 12, 0);

    Tuple acceleration = vector(-0.15, -1, 0);
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
    float my_matrix[4][4] = {{1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}};
    float my_matrix2[4][4] = {{1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}};
    float my_matrix3[4][4] = {{1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.54}};

    assert(my_matrix[0][0] == 1);
    assert(my_matrix[0][3] == 4);
    assert(my_matrix[1][2] == 7.5);

    assert(mat_equal(4, &my_matrix[0], &my_matrix2[0]));
    assert(!mat_equal(4, &my_matrix[0], &my_matrix3[0]));

    // Everything passes
    puts("All checks pass.");
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

char *canvas_to_ppm(Canvas *can, char *file_name)
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

            char r = (unsigned char)red;
            char g = (unsigned char)green;
            char b = (unsigned char)blue;

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
    return ppm_data;
}

bool mat_equal(int size, float mat_a[size][size], float mat_b[size][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (!equal(mat_a[i][j], mat_b[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}
// THIS HAS SIDE EFFECTS,
void mat_mult(int size, float mat_a[size][size], float mat_b[size][size], float mat_out[size][size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                mat_out[i][j] = mat_a[i][k] * mat_b[k][j];
            }
        }
    }
}