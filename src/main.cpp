#include "raylib.h"
#include <random>
#include <array>
#include <cmath>

float dot(Vector3 a, Vector3 b)
{
    return (float((a.x * b.x) + (a.y * b.y) + (a.z * b.z)));
}

Vector3 mul(Vector3 a, float multiplier)
{
    return Vector3{a.x * multiplier, a.y * multiplier, a.z * multiplier};
}

Vector3 add(Vector3 a, Vector3 b)
{
    return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 slerp(Vector3 p0, Vector3 p1, float t = 0.5)
{
    float th = acos(dot(p0, p1));
    Vector3 slerp = add(mul(p0, (sin((1 - t) * th) / sin(th))), mul(p1, (sin(t * th) / sin(th))));
    return slerp;
}

Mesh Fragment(Mesh mesh, uint8_t fragDept)
{
    // for one triangle
    struct Triangle
    {
        Vector3 a, b, c;
    };

    // store all triangles of the mesh here
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < mesh.triangleCount; i++)
    {
        int i0 = mesh.indices[i * 3 + 0];
        int i1 = mesh.indices[i * 3 + 1];
        int i2 = mesh.indices[i * 3 + 2];

        Triangle tri;
        tri.a = {mesh.vertices[i0 * 3], mesh.vertices[i0 * 3 + 1], mesh.vertices[i0 * 3 + 2]};
        tri.b = {mesh.vertices[i1 * 3], mesh.vertices[i1 * 3 + 1], mesh.vertices[i1 * 3 + 2]};
        tri.c = {mesh.vertices[i2 * 3], mesh.vertices[i2 * 3 + 1], mesh.vertices[i2 * 3 + 2]};
        triangles.push_back(tri);
    }

    for (size_t depth = 0; depth < fragDept; depth++)
    {
        std::vector<Triangle> subdivided;

        for (Triangle &tri : triangles)
        {
            // find midpoints
            Vector3 mAB = slerp(tri.a, tri.b);
            Vector3 mBC = slerp(tri.b, tri.c);
            Vector3 mCA = slerp(tri.c, tri.a);

            // split into 4 smaller triangles
            subdivided.push_back({tri.a, mAB, mCA});
            subdivided.push_back({mAB, tri.b, mBC});
            subdivided.push_back({mCA, mBC, tri.c});
            subdivided.push_back({mAB, mBC, mCA});
        }

        triangles = subdivided;
    }

    // make a new mesh from the subdivided triangles
    Mesh result{0};
    result.triangleCount = (int)triangles.size();
    result.vertexCount = result.triangleCount * 3;
    result.vertices = (float *)MemAlloc(result.vertexCount * 3 * sizeof(float));

    for (size_t i = 0; i < result.triangleCount; i++)
    {
        Vector3 verts[3] = {triangles[i].a, triangles[i].b, triangles[i].c};
        for (size_t j = 0; j < 3; j++)
        {
            result.vertices[(i * 3 + j) * 3 + 0] = verts[j].x;
            result.vertices[(i * 3 + j) * 3 + 1] = verts[j].y;
            result.vertices[(i * 3 + j) * 3 + 2] = verts[j].z;
        }
    }

    UploadMesh(&result, false);
    return result;
}

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

std::vector<Vector3> vertices = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11};

std::vector<uint32_t> triangleFacesIndices =
    {
        0, 1, 2,
        0, 3, 1,
        0, 2, 4,
        3, 0, 5,
        0, 4, 5,
        1, 3, 6,
        1, 7, 2,
        7, 1, 6,
        4, 2, 8,
        7, 8, 2,
        9, 3, 5,
        6, 3, 9,
        5, 4, 10,
        4, 8, 10,
        9, 5, 10,
        7, 6, 11,
        7, 11, 8,
        11, 6, 9,
        8, 11, 10,
        10, 11, 9};

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

    // Init the mesh
    Mesh mesh = {0};
    mesh.vertexCount = vertices.size();
    mesh.triangleCount = triangleFacesIndices.size() / 3;
    // allocate vertex positions
    // vertexCount * 3 becuase one vertex is x,y,z
    // the loop:
    // raylib expects flat array just like the triangleFacesIndices for vertex
    //[x0,y0,z0, x1,y1,z1, x2,y2,z2] - the format
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    for (size_t i = 0; i < mesh.vertexCount; i++)
    {
        mesh.vertices[i * 3 + 0] = vertices[i].x;
        mesh.vertices[i * 3 + 1] = vertices[i].y;
        mesh.vertices[i * 3 + 2] = vertices[i].z;
    }
    // allocate indices
    mesh.indices = (unsigned short *)MemAlloc(triangleFacesIndices.size() * sizeof(unsigned short));
    for (size_t i = 0; i < triangleFacesIndices.size(); i++)
    {
        mesh.indices[i] = triangleFacesIndices[i];
    }

    // fragment the mesh
    mesh = Fragment(mesh, 3);
    UploadMesh(&mesh, false);
    Model icosahedronModel = LoadModelFromMesh(mesh);

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
                DrawModelWires(icosahedronModel, {0, 0, 0}, 1.0f, GREEN);
            }
            EndMode3D();
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}