/*******************************************************************************************
*
*   Animation sample, by Varanda Labs
*   http://www.varanda.ca
*   Code changes are: Copyright (c) 2025 Varanda Labs Inc.
*
*   Assets:
*       https://www.gameart2d.com/adventurer-girl---free-sprites.html
*       https://www.gameart2d.com/free-desert-platformer-tileset.html
*
*   Derived code from:
*   ==================
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

/*

Info from GIMP file:
===================


box-1 pos: 293, 833   size: 1792x128
box-2 pos: 892, 627   size: 385x96   (plataforma)
box-3 pos: 1324, 497  size: 515x96    (plataforma)
box-4 pos: 2342, 829  size: 764x130
box-5 pos: 3108, 577  size: 643x383
box-6 pos: 4003, 431  size: 512x96     (plataforma)
box-7 pos: 4774, 831  size: 1627x130
box-8 pos: 2801, 671  size 257x96

bush-1 pos: 4130, 359  size: 133x73

Cactos-1 pos: 1442, 377  size: 106x119
Cactos-2 pos: 2626, 734  size: 92x98
Cactos-3 pos: 3256, 464  size: 109x117
Cactos-4 pos: 5026, 718  size: 111x112
Cactos-5 pos: 5593, 721  size: 112x110


*/

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define LIMIT_SINGLE_JUMP

#define G 400
#define PLAYER_JUMP_SPD 420.0  //350.0f
#define PLAYER_HOR_SPD 200.0f

#define BACKGROUND_OFFSET 80

#define DRAW_SPEED_FACTOR 200.0

// image files
#define STATIC_BACKGROUND_FILENAME "assets/background-1280x960.png"
#define MOVING_BACKGROUND_FILENAME "assets/bk-move-6400x960.png"
#define GROUND_FILENAME "assets/ground-01.png"


typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
    int old_state;
    int state;
} Player;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

//-----------------------------------------
//  Variables (file scope)
//-----------------------------------------

//------------------------ Girl Animation ---------------------------
#define MENINA_DIR "./assets/girl/"
#define GIRL_POS_OFFSET_X 8
#define GIRL_POS_OFFSET_Y -60

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

anim_t * animation_info_ptr;
#define NUM_ANIMATIONS  (sizeof(anim_array) / sizeof(anim_array[0]))

static float anim_timer = 0.0;
static int curr_anim = 0, anim_idx = 0;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
Texture2D * UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenterMV          (Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenter            (Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMap   (Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding  (Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush  (Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
//    const int screenWidth = 800;
//    const int screenHeight = 450;

    // 1280x960
    const int screenWidth = (1280 / 4) * 3;
    const int screenHeight = (960 / 4) * 3;

    int i;
    char temp_text[256];

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");


    //------------ load Girl textures -------------
    for (anim_idx = 0; anim_idx < NUM_ANIMATIONS; anim_idx++)
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

    //---- load textures ----
    Image temp_image = LoadImage(STATIC_BACKGROUND_FILENAME);         // Loaded in CPU memory (RAM)
    Texture2D bg_texture = LoadTextureFromImage(temp_image);          // Image converted to texture, GPU memory (VRAM)
    UnloadImage(temp_image);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    temp_image = LoadImage(MOVING_BACKGROUND_FILENAME);
    Texture2D moving_bg_texture = LoadTextureFromImage(temp_image);
    UnloadImage(temp_image);

    temp_image = LoadImage(GROUND_FILENAME);
    Texture2D ground_texture = LoadTextureFromImage(temp_image);
    UnloadImage(temp_image);
#define PLAYER_INITIAL_X 400
#define PLAYER_INITIAL_Y 830

    Player player = { 0 };
    player.position = (Vector2){ PLAYER_INITIAL_X, PLAYER_INITIAL_Y};
    player.speed = 0;
    player.canJump = false;

    EnvItem envItems[] = {
    {{ 293, 833, 1792, 128 }, 1, GRAY },
    {{ 892, 627, 385, 96 }, 1, GRAY },   //(plataforma)
    {{ 1324, 497, 515, 96 }, 1, GRAY },  //  (plataforma)
    {{ 2342, 829,  764, 130 }, 1, GRAY },
    {{ 3108, 577,  643, 383}, 1, GRAY },
    {{ 4003, 431,  512, 96  }, 1, GRAY }, //   (plataforma)
    {{ 4774, 831,  1627, 130}, 1, GRAY },
    {{ 2801, 671, 257, 96}, 1, GRAY },
    };
    /*
        box-1 pos: 293, 833   size: 1792x128
        box-2 pos: 892, 627   size: 385x96   (plataforma)
        box-3 pos: 1324, 497  size: 515x96    (plataforma)
        box-4 pos: 2342, 829  size: 764x130
        box-5 pos: 3108, 577  size: 643x383
        box-6 pos: 4003, 431  size: 512x96     (plataforma)
        box-7 pos: 4774, 831  size: 1627x130
        box-8 pos: 2801, 671 size 257x96 */


    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenterMV,
        UpdateCameraCenter,
        UpdateCameraCenterInsideMap,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
    };

    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);

    char *cameraDescriptions[] = {
        "Fixo na vertical (Jogo)",
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; update player center vertically after landing",
        "Player push camera on getting too close to screen edge"
    };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    int moving_bg_texture_x = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        Texture2D * CurrentGirlTexture = UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

        camera.zoom += ((float)GetMouseWheelMove()*0.05f * deltaTime * DRAW_SPEED_FACTOR);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            player.position = (Vector2){ PLAYER_INITIAL_X, PLAYER_INITIAL_Y };
        }

        if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1)%cameraUpdatersLength;

        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);

                //------------
                DrawTexture(bg_texture, player.position.x - PLAYER_INITIAL_X - BACKGROUND_OFFSET, 0, WHITE);
                DrawTexture(moving_bg_texture, moving_bg_texture_x - (BACKGROUND_OFFSET * 4), 0, WHITE);
                DrawTexture(ground_texture, 0, 0, WHITE);

                //for (int i = 1; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);

                // show fake character as a square
//                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40.0f, 40.0f };
//                DrawRectangleRec(playerRect, RED);

                //------ draw the girl ----
                Rectangle menina_source = {0,0, menina_texture_sample_ptr->width, menina_texture_sample_ptr->height};
                Rectangle menina_dest = {player.position.x + GIRL_POS_OFFSET_X, player.position.y + GIRL_POS_OFFSET_Y, menina_texture_sample_ptr->width/4, menina_texture_sample_ptr->height/4};
                Vector2 menina_ori = {menina_texture_sample_ptr->width/8, menina_texture_sample_ptr->height/8};
                DrawTexturePro(*CurrentGirlTexture, menina_source,  menina_dest, menina_ori, 0, WHITE);


                DrawCircleV(player.position, 5.0f, GOLD);

                moving_bg_texture_x = player.position.x / 2;


            EndMode2D();

            DrawText("Controls:", 20, 20, 10, BLACK);
            DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
            DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
            DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
            DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
            DrawText("Current camera mode:", 20, 120, 10, BLACK);
            DrawText(cameraDescriptions[cameraOption], 40, 140, 10, DARKGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayerState(Player *player, int state)
{
    if (player->state != state) {
        anim_idx = 0;
        curr_anim = state;
    }
    player->old_state = player->state;
    player->state = state;
}

Texture2D * UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    Texture2D * ret = NULL;
    static int face_right = 1;
    static float log_timer = 0;
    int log = 0;
    static int log_cnt = 0;

    log_timer += delta;
    if (log_timer >= .25) {
        log = 1;
        log_timer = 0;
        log_cnt++;
    }

    anim_timer += delta;

    // if the time has expired select the next frame
    // Se o timer espirou entao seleciona o proximo frame para ser exibido via incremento de anim_idx
    if (anim_timer >= anim_array[curr_anim].frame_period) {
        anim_timer = 0;
        if (player->canJump == 0) {
            // for jump we have a special sequence: 1,2,3 and loop 4 and 6 (indexes: 0,1,2 and loop 3 and 5):
            switch(anim_idx)
            {
                case 0: anim_idx = 1; break;
                case 1: anim_idx = 2; break;
                case 2: anim_idx = 3; break;
                case 3: anim_idx = 5; break;
                case 5: anim_idx = 3; break;
                default: anim_idx = 0; break;
            }
        }
        else {
            anim_idx++;
        }
        if (anim_idx >= anim_array[curr_anim].num_frames) // se ultimo frame seleciona o primeiro
            anim_idx = 0;
    }
    ret = &anim_array[curr_anim].textures[anim_idx];


    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= PLAYER_HOR_SPD*delta;
        face_right = 0;
        if (player->canJump) {
            UpdatePlayerState(player, ANIM_ID_RUN);
        }
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += PLAYER_HOR_SPD*delta;
        face_right = 1;
        if (player->canJump) {
            UpdatePlayerState(player, ANIM_ID_RUN);
        }
    }
    else {
        if (player->canJump) {
            UpdatePlayerState(player, ANIM_ID_IDLE);
        }

    }

    if ((IsKeyDown(KEY_SPACE) && player->canJump) ||
        (IsKeyDown(KEY_UP) && player->canJump))

    {
        player->speed = -PLAYER_JUMP_SPD;
#ifdef LIMIT_SINGLE_JUMP
        player->canJump = false;
        UpdatePlayerState(player, ANIM_ID_JUMP);
#endif
    }

    bool hitObstacle = false;
    bool in_air = true;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            (ei->rect.y) >= (p->y) &&
            (ei->rect.y) <= (p->y) + player->speed*delta)
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            in_air = false;
            curr_anim = player->state;
            break;
        }

    }

    if (in_air) {
        if (log) printf("in air %d\n", log_cnt);

        // if the girl is running we change the animation to jump as she is falling
        if (player->state == ANIM_ID_RUN) {
            curr_anim = ANIM_ID_JUMP;

        }
    }

    if (!hitObstacle)
    {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
#ifdef LIMIT_SINGLE_JUMP
        player->canJump = false;
#endif
    }
    else player->canJump = true;

    if (face_right)
        ret = &anim_array[curr_anim].textures[anim_idx];
    else
        ret = &anim_array[curr_anim].flipped_textures[anim_idx];

    return ret;
}

void UpdateCameraCenterMV(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2.0f, height - 120};///2.0f };
    Vector2 pos = {player->position.x, PLAYER_INITIAL_Y};
    camera->target = pos; //player->position;
}

void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = player->position;
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target.x = player->position.x;

    if (eveningOut)
    {
        if (evenOutTarget > camera->target.y)
        {
            camera->target.y += evenOutSpeed*delta;

            if (camera->target.y > evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
        else
        {
            camera->target.y -= evenOutSpeed*delta;

            if (camera->target.y < evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
    }
    else
    {
        if (player->canJump && (player->speed == 0) && (player->position.y != camera->target.y))
        {
            eveningOut = 1;
            evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

    if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
