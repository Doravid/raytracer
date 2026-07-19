#include <raylib.h>
#include "linear_algebra.h"

#define MOUSE_SENSITIVITY (0.002f)
#define MOVE_SPEED (0.2f)
void handle_input(struct camera *cam, Tuple *cam_pos)
{
    static float pitch;
    static float yaw;
    static bool init = false;

    if (!init)
    {
        Tuple fwd = mat_tuple_mult(cam->inverse_transform, vector(0, 0, -1));
        yaw = atan2f(fwd.x, -fwd.z);
        pitch = asinf(fwd.y);
        init = true;
    }

    Vector2 mouseDelta = GetMouseDelta();
    pitch -= mouseDelta.y * MOUSE_SENSITIVITY;
    yaw -= mouseDelta.x * MOUSE_SENSITIVITY;
    if (pitch > 1.55f)
        pitch = 1.55f;
    if (pitch < -1.55f)
        pitch = -1.55f;

    Tuple forward = vector(
        sinf(yaw) * cosf(pitch),
        sinf(pitch),
        -cosf(yaw) * cosf(pitch));

    Tuple world_up = vector(0, 1, 0);
    Tuple right = tuple_normalize(tuple_cross(forward, world_up));
    Tuple up = tuple_normalize(tuple_cross(right, forward));

    if (IsKeyDown(KEY_W))
        *cam_pos = tuple_add(*cam_pos, tuple_scale(forward, MOVE_SPEED));
    if (IsKeyDown(KEY_S))
        *cam_pos = tuple_sub(*cam_pos, tuple_scale(forward, MOVE_SPEED));
    if (IsKeyDown(KEY_A))
        *cam_pos = tuple_add(*cam_pos, tuple_scale(right, MOVE_SPEED));
    if (IsKeyDown(KEY_D))
        *cam_pos = tuple_sub(*cam_pos, tuple_scale(right, MOVE_SPEED));
    if (IsKeyDown(KEY_SPACE))
        *cam_pos = tuple_add(*cam_pos, tuple_scale(up, MOVE_SPEED));
    if (IsKeyDown(KEY_LEFT_SHIFT))
        *cam_pos = tuple_sub(*cam_pos, tuple_scale(up, MOVE_SPEED));

    Tuple target = tuple_add(*cam_pos, forward);
    float new_transform[16];
    view_transform(*cam_pos, target, up, new_transform);
    set_camera_transform(cam, new_transform);
}
Texture2D init_texture(Canvas *image)
{

    Image raylib_img = {
        .data = image->pixels,
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .format = PIXELFORMAT_UNCOMPRESSED_R32G32B32,
        .mipmaps = 1};
    return LoadTextureFromImage(raylib_img);
}

void init_world(World *werld)
{
    *werld = default_world();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raytracer");
    SetTargetFPS(999);
    werld->num_spheres = 6;

    float temp1[16], temp2[16];
    float scale[16], rotate_x[16], rotate_y[16], translate[16], final_transform[16];

    werld->spheres[0] = sphere();
    mat_scale(10, 0.1, 10, scale);
    set_transform(&werld->spheres[0], scale);
    werld->spheres[0].material.color = color(1, 0.9, 0.9);
    werld->spheres[0].material.specular = 0;

    werld->spheres[1] = sphere();
    mat_rotate_x(M_PI / 2, rotate_x);
    mat_rotate_y(-M_PI / 4, rotate_y);
    mat_translate(0, 0, 5, translate);
    mat_mult(4, rotate_x, scale, temp1);
    mat_mult(4, rotate_y, temp1, temp2);
    mat_mult(4, translate, temp2, final_transform);
    set_transform(&werld->spheres[1], final_transform);
    werld->spheres[1].material = werld->spheres[0].material;

    werld->spheres[2] = sphere();
    mat_rotate_y(M_PI / 4, rotate_y);
    mat_mult(4, rotate_y, temp1, temp2);
    mat_mult(4, translate, temp2, final_transform);
    set_transform(&werld->spheres[2], final_transform);
    werld->spheres[2].material = werld->spheres[0].material;

    werld->spheres[3] = sphere();
    mat_translate(-0.5, 1.0, 0.5, translate);
    set_transform(&werld->spheres[3], translate);
    werld->spheres[3].material.color = color(0.1, 1.0, 0.5);
    werld->spheres[3].material.diffuse = 0.7f;
    werld->spheres[3].material.specular = 0.3f;

    werld->spheres[4] = sphere();
    mat_scale(0.5, 0.5, 0.5, scale);
    mat_translate(1.5, 0.5, -0.5, translate);
    mat_mult(4, translate, scale, final_transform);
    set_transform(&werld->spheres[4], final_transform);
    werld->spheres[4].material.color = color(0.5, 1.0, 0.1);
    werld->spheres[4].material.diffuse = 0.7f;
    werld->spheres[4].material.specular = 0.3f;
    werld->spheres[5] = sphere();
    mat_scale(0.33, 0.33, 0.33, scale);
    mat_translate(-1.5, 0.33, -0.75, translate);
    mat_mult(4, translate, scale, final_transform);
    set_transform(&werld->spheres[5], final_transform);
    werld->spheres[5].material.color = color(1.0, 0.8, 0.1);
    werld->spheres[5].material.diffuse = 0.7f;
    werld->spheres[5].material.specular = 0.3f;

    werld->light = point_light(point(-10, 10, -10), color(1, 1, 1));
}