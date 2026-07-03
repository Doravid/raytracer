#include <math.h>

#include "raylib.h"
#include "window.h"
#include "linear_algebra.h"

int main(void)
{
    int renderWidth = 480;
    int renderHeight = 360;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Raytracer");
    SetTargetFPS(999);

    // 1. Setup your raytracer world and camera
    struct world world = default_world();
    init_world(&world);
    struct camera cam = camera(renderWidth, renderHeight, M_PI / 3);
    float mat_transform[16];
    Tuple cam_pos = point(0, 1.5, -5);
    view_transform(cam_pos, point(0, 1, 0), vector(0, 1, 0), mat_transform);
    set_camera_transform(&cam, mat_transform);

    Canvas image = canvas(renderWidth, renderHeight);

    Image raylib_img = {
        .data = image.pixels,
        .width = renderWidth,
        .height = renderHeight,
        .format = PIXELFORMAT_UNCOMPRESSED_R32G32B32,
        .mipmaps = 1};
    Texture2D texture = LoadTextureFromImage(raylib_img);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
            DisableCursor();
        }
        handle_input(&cam, &cam_pos);
        render_frame(cam, world, &image);

        // Send the updated pixels to the GPU
        UpdateTexture(texture, image.pixels);

        float screenW = (float)GetScreenWidth();
        float screenH = (float)GetScreenHeight();
        float scale = fmaxf(screenW / renderWidth, screenH / renderHeight);

        Rectangle sourceRec = {0.0f, 0.0f, (float)renderWidth, (float)renderHeight};
        Rectangle destRec = {
            (screenW - ((float)renderWidth * scale)) * 0.5f,
            (screenH - ((float)renderHeight * scale)) * 0.5f,
            (float)renderWidth * scale,
            (float)renderHeight * scale};
        Vector2 origin = {0.0f, 0.0f};

        // Draw the frame
        BeginDrawing();
        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawFPS(5, 5);

        EndDrawing();
    }

    // Clean up
    UnloadTexture(texture);
    free(image.pixels);
    CloseWindow();

    return 0;
}