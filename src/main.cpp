#include "raylib.h"
#include <random>
#include <array>

Vector3 v0 = {0.8506508f, 0.5257311f, 0.f};
Vector3 v1 = {0.000000101405476f, 0.8506507f, -0.525731f};
Vector3 v2 = {0.000000101405476f, 0.8506506f, 0.525731f};
Vector3 v3 = {0.5257309f, -0.00000006267203f, -0.85065067f};
Vector3 v4 = {0.52573115f, -0.00000006267203f, 0.85065067f};
Vector3 v5 = {0.8506508f, -0.5257311f, 0.f};
Vector3 v6 = {-0.52573115f, 0.00000006267203f, -0.85065067f};
Vector3 v7 = {-0.8506508f, 0.5257311f, 0.f};
Vector3 v8 = {-0.5257309f, 0.00000006267203f, 0.85065067f};
Vector3 v9 = {-0.000000101405476f, -0.8506506f, -0.525731f};
Vector3 v10 = {-0.000000101405476f, -0.8506507f, 0.525731f};
Vector3 v11 = {-0.8506508f, -0.5257311f, 0.f};

std::array<std::array<Vector3, 3>, 20> triangleFaces = {
    {{v0, v1, v2},
     {v0, v3, v1},
     {v0, v2, v4},
     {v3, v0, v5},
     {v0, v4, v5},
     {v1, v3, v6},
     {v1, v7, v2},
     {v7, v1, v6},
     {v4, v2, v8},
     {v7, v8, v2},
     {v9, v3, v5},
     {v6, v3, v9},
     {v5, v4, v10},
     {v4, v8, v10},
     {v9, v5, v10},
     {v7, v6, v11},
     {v7, v11, v8},
     {v11, v6, v9},
     {v8, v11, v10},
     {v10, v11, v9}}

};

Color GetRandomColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_int_distribution<int> hueRange(0, 360);

    return ColorFromHSV(hueRange(gen), 1, 1);
}

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Ico");

    // Camera
    Camera3D camera = {0};
    camera.position = (Vector3){10.f, 10.f, 10.f};
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();

    SetTargetFPS(60);

    std::array<Color, 20> faceColors;
    for (size_t i = 0; i < faceColors.size(); i++)
    {
        faceColors[i] = GetRandomColor();
    }

    while (!WindowShouldClose())
    {

        // UPDATES
        Color triangleColor = GetRandomColor();

        UpdateCamera(&camera, CAMERA_FREE);
        if (IsKeyPressed(KEY_Z))
            camera.target = (Vector3){0, 0, 0};

        BeginDrawing();
        {
            ClearBackground(WHITE);

            BeginMode3D(camera);
            {
                DrawGrid(100, 1.f);
                for (size_t i = 0; i < triangleFaces.size(); i++)
                {
                    auto triangle = triangleFaces[i];
                    Color triangleColor = faceColors[i];

                    DrawLine3D(triangle[0], triangle[1], triangleColor);
                    DrawLine3D(triangle[1], triangle[2], triangleColor);
                    DrawLine3D(triangle[2], triangle[0], triangleColor);
                }
            }
            EndMode3D();
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}