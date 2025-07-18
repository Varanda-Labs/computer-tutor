/*******************************************************************************************
*
*   raylib [core] example - 2D Camera platformer
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.0
*
*   Example contributed by arvyy (@arvyy) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 arvyy (@arvyy)
*
********************************************************************************************/

#include "raylib.h"
#include <stdio.h>

#define MENINA_DIR "../2d-game/assets/girl/"

/* asset files:
Dead-10.png  Dead-9.png   Idle-8.png   Jump-7.png   Melee-7.png  Shoot-1.png
Dead-1.png   Idle-10.png  Idle-9.png   Jump-8.png   Run-1.png    Shoot-2.png
Dead-2.png   Idle-1.png   Jump-10.png  Jump-9.png   Run-2.png    Shoot-3.png
Dead-3.png   Idle-2.png   Jump-1.png   Melee-1.png  Run-3.png    Slide-1.png
Dead-4.png   Idle-3.png   Jump-2.png   Melee-2.png  Run-4.png    Slide-2.png
Dead-5.png   Idle-4.png   Jump-3.png   Melee-3.png  Run-5.png    Slide-3.png
Dead-6.png   Idle-5.png   Jump-4.png   Melee-4.png  Run-6.png    Slide-4.png
Dead-7.png   Idle-6.png   Jump-5.png   Melee-5.png  Run-7.png    Slide-5.png
Dead-8.png   Idle-7.png   Jump-6.png   Melee-6.png  Run-8.png */

#define ANIM_ID_IDLE        0
#define ANIM_ID_RUN         1
#define ANIM_ID_JUMP        2
#define ANIM_ID_SHOOT       3
#define ANIM_ID_SLIDE       4
#define ANIM_ID_DEAD        5
#define ANIM_ID_MELEE       6


typedef struct anim_st {
    const int anim_id;
    const char * name;
    const char * filename_format;
    const int num_frames;
    float frame_period;
    Texture2D  textures[10];
    Texture2D  flipped_textures[10];
} anim_t;

static  anim_t anim_array[] = {
    //--------------- idle ----------------
    {
        anim_id: ANIM_ID_IDLE,
        name: "Idle",
        filename_format: MENINA_DIR "Idle-%d.png",
        num_frames: 10,
        frame_period: 0.1f
    },
    //--------------- run ----------------
    {
        anim_id: ANIM_ID_RUN,
        name: "Run",
        filename_format: MENINA_DIR "Run-%d.png",
        num_frames: 8,
        frame_period: 0.1f
    },
    //--------------- jump ----------------
    {
        anim_id: ANIM_ID_JUMP,
        name: "Jump",
        filename_format: MENINA_DIR "Jump-%d.png",
        num_frames: 10,
        frame_period: 0.1f
    },
    //--------------- shoot ----------------
    {
        anim_id: ANIM_ID_SHOOT,
        name: "Shoot",
        filename_format: MENINA_DIR "Shoot-%d.png",
        num_frames: 3,
        frame_period: 0.1f
    },
    //--------------- slide ----------------
    {
        anim_id: ANIM_ID_SLIDE,
        name: "Slide",
        filename_format: MENINA_DIR "Slide-%d.png",
        num_frames: 5,
        frame_period: 0.1f
    },
    //--------------- dead ----------------
    {
        anim_id: ANIM_ID_DEAD,
        name: "Dead",
        filename_format: MENINA_DIR "Dead-%d.png",
        num_frames: 10,
        frame_period: 0.1f
    },
    //--------------- MeLee ----------------
    {
        anim_id: ANIM_ID_MELEE,
        name: "MeLee",
        filename_format: MENINA_DIR "Melee-%d.png",
        num_frames: 7,
        frame_period: 0.1f
    },
};

#define NUM_ANIMATIONS  (sizeof(anim_array) / sizeof(anim_array[0]))


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    float speed;
    int i;
    char temp_text[256];
    int anim_idx = 0;

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");


    for (int anim_idx = 0; anim_idx < NUM_ANIMATIONS; anim_idx++)
    {
        for (i=0; i< anim_array[anim_idx].num_frames; i++)
        {
            snprintf(temp_text, sizeof(temp_text), anim_array[anim_idx].filename_format, i+1);
            Image img = LoadImage(temp_text);
            anim_array[anim_idx].textures[i] = LoadTextureFromImage(img);
            ImageFlipHorizontal(&img);
            anim_array[anim_idx].flipped_textures[i] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    Texture2D * menina_texture_sample_ptr = &anim_array[0].textures[0];

    Vector2 girlPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float anim_timer = 0;

    int curr_anim = ANIM_ID_IDLE;
    int show_reverse = 0;

#define ANIM_SPEED 0.1
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float ft = GetFrameTime();
        speed = ft * SPEED;
        anim_timer += ft;
        if (anim_timer >= anim_array[curr_anim].frame_period) {
            anim_timer = 0;
            anim_idx++;
            if (anim_idx >= anim_array[curr_anim].num_frames)
                anim_idx = 0;
        }

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) girlPosition.x += speed;
        if (IsKeyDown(KEY_LEFT)) girlPosition.x -= speed;
        if (IsKeyDown(KEY_UP)) girlPosition.y -= speed;
        if (IsKeyDown(KEY_DOWN)) girlPosition.y += speed;

        if (IsKeyPressed(KEY_A)) {
            if (show_reverse == 1) {
                curr_anim++;
                if (curr_anim >= NUM_ANIMATIONS) curr_anim = 0;
                show_reverse = 0;
            }
            else {
                show_reverse++;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            if (show_reverse == 0)
                snprintf(temp_text, sizeof(temp_text), "Animacao: %s", anim_array[curr_anim].name);
            else
                snprintf(temp_text, sizeof(temp_text), "Animacao: %s flipped", anim_array[curr_anim].name);

            DrawText(temp_text, 10, 10, 20, DARKGRAY);


            Rectangle menina_source = {0,0, menina_texture_sample_ptr->width, menina_texture_sample_ptr->height};
            Rectangle menina_dest = {girlPosition.x, girlPosition.y, menina_texture_sample_ptr->width/4, menina_texture_sample_ptr->height/4};
            Vector2 menina_ori = {menina_texture_sample_ptr->width/8, menina_texture_sample_ptr->height/8};

            if (show_reverse == 0)
                DrawTexturePro(anim_array[curr_anim].textures[anim_idx], menina_source,  menina_dest, menina_ori, 0, WHITE);
            else
                DrawTexturePro(anim_array[curr_anim].flipped_textures[anim_idx], menina_source,  menina_dest, menina_ori, 0, WHITE);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
