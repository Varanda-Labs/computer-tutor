
#include "raylib.h"

#define SPEED 200.0
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    float speed;
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        speed = GetFrameTime() * SPEED;
        // speed = SPEED;

        // simulate delays
        // usleep(100 * 1000);

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += speed;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= speed;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= speed;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += speed;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(ballPosition, 50, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
