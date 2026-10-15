#include <math.h>
#include <pthread.h>

#include "raylib.h"
#include "window.h"
#include "linear_algebra.h"
#include "tests.h"

int main(void)
{
    test_linear_algebra();

    struct world world;
    Canvas image = canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct camera cam = camera(SCREEN_WIDTH, SCREEN_HEIGHT, M_PI / 3);
    Tuple cam_pos = point(2, 3, -10);
    float mat_transform[16];

    init_world(&world);
    view_transform(cam_pos, point(0, 0, 0), vector(0, 1, 0), mat_transform);

    set_camera_transform(&cam, mat_transform);
    Texture2D texture = init_texture(&image);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
            DisableCursor();
        }
        handle_input(&cam, &cam_pos);

        render_frame(cam, world, &image);

        UpdateTexture(texture, image.pixels);

        float screenW = (float)GetScreenWidth();
        float screenH = (float)GetScreenHeight();
        float scale = fmaxf(screenW / SCREEN_WIDTH, screenH / SCREEN_HEIGHT);

        Rectangle sourceRec = {0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        Rectangle destRec = {
            (screenW - ((float)SCREEN_WIDTH * scale)) * 0.5f,
            (screenH - ((float)SCREEN_HEIGHT * scale)) * 0.5f,
            (float)SCREEN_WIDTH * scale,
            (float)SCREEN_HEIGHT * scale};
        BeginDrawing();

        DrawTexturePro(texture, sourceRec, destRec, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        DrawFPS(5, 5);
        EndDrawing();
    }

    UnloadTexture(texture);
    free(image.pixels);
    CloseWindow();
    return 0;
}