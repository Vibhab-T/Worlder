#include "raylib.h"
#include "iostream"

Vector3 v0 = {-0.85, 0, 0.525};
Vector3 v1 = {0.85, 0, 0.525};
Vector3 v2 = {0.85, 0, -0.525};
Vector3 v3 = {-0.85, 0, -0.525};

void DrawRectPlane(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, Color color)
{
    DrawLine3D(v0, v1, color);
    DrawLine3D(v1, v2, color);
    DrawLine3D(v2, v3, color);
    DrawLine3D(v3, v0, color);
}

Vector3 Rotate90InX(Vector3 v)
{
    return (Vector3){v.x, -(v.z), 0};
}
Vector3 Rotate90InY(Vector3 v)
{
    return (Vector3){v.z, v.y, -(v.x)};
}
Vector3 Rotate90InZ(Vector3 v)
{
    return (Vector3){-(v.y), v.x, v.z};
}

void DrawGoldenRectangles()
{
    std::cout << "First Points (RED):\n"
              << "  v0: (" << v0.x << ", " << v0.y << ", " << v0.z << ")\n"
              << "  v1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")\n"
              << "  v2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")\n"
              << "  v3: (" << v3.x << ", " << v3.y << ", " << v3.z << ")\n";
    DrawRectPlane(v0, v1, v2, v3, RED);

    Vector3 x0 = Rotate90InX(v0);
    Vector3 x1 = Rotate90InX(v1);
    Vector3 x2 = Rotate90InX(v2);
    Vector3 x3 = Rotate90InX(v3);
    x0 = Rotate90InZ(x0);
    x1 = Rotate90InZ(x1);
    x2 = Rotate90InZ(x2);
    x3 = Rotate90InZ(x3);

    std::cout << "Second Points (BLUE):\n"
              << "  v0: (" << x0.x << ", " << x0.y << ", " << x0.z << ")\n"
              << "  v1: (" << x1.x << ", " << x1.y << ", " << x1.z << ")\n"
              << "  v2: (" << x2.x << ", " << x2.y << ", " << x2.z << ")\n"
              << "  v3: (" << x3.x << ", " << x3.y << ", " << x3.z << ")\n";
    DrawRectPlane(x0, x1, x2, x3, BLUE);

    x0 = Rotate90InZ(x0);
    x1 = Rotate90InZ(x1);
    x2 = Rotate90InZ(x2);
    x3 = Rotate90InZ(x3);
    x0 = Rotate90InY(x0);
    x1 = Rotate90InY(x1);
    x2 = Rotate90InY(x2);
    x3 = Rotate90InY(x3);

    std::cout << "Third Points (GREEN):\n"
              << "  v0: (" << x0.x << ", " << x0.y << ", " << x0.z << ")\n"
              << "  v1: (" << x1.x << ", " << x1.y << ", " << x1.z << ")\n"
              << "  v2: (" << x2.x << ", " << x2.y << ", " << x2.z << ")\n"
              << "  v3: (" << x3.x << ", " << x3.y << ", " << x3.z << ")\n";
    DrawRectPlane(x0, x1, x2, x3, GREEN);
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

    while (!WindowShouldClose())
    {

        // UPDATES

        UpdateCamera(&camera, CAMERA_FREE);
        if (IsKeyPressed(KEY_Z))
            camera.target = (Vector3){0, 0, 0};

        BeginDrawing();
        {
            ClearBackground(WHITE);

            BeginMode3D(camera);
            {
                DrawGrid(100, 1.f);

                DrawGoldenRectangles();
            }
            EndMode3D();
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}