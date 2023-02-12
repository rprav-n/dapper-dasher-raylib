#include <cstdio>

#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
    int velocity;
};

bool isOnGround(AnimData data, int window_height) {
    return data.pos.y >= window_height - data.rec.height;
}

AnimData updateAnimData(AnimData data, float dt, int max_frame) {
    data.running_time += dt;
    if (data.running_time >= data.update_time) {
        data.rec.x = data.frame * data.rec.width;
        data.running_time = 0;
        data.frame++;
        if (data.frame > max_frame) data.frame = 0;
    }
    return data;
}

int main() {
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    bool collision = false;

    // window dimesions
    const int window_width = 512;
    const int window_height = 380;

    InitWindow(window_width, window_height, "Dapper Dasher");

    // acceleration due to gravity (pixel/sec)/sec
    const int gravity = 2000;

    Texture2D scarfy = LoadTexture("./textures/scarfy.png");

    AnimData scarfyData =
        AnimData{Rectangle{0, 0, (float)scarfy.width / 6, (float)scarfy.height},
                 Vector2{100, (float)window_height - scarfy.height},
                 0,
                 1.0 / 12.0,
                 0,
                 0};

    // Rectangle scarfy_rect;
    // scarfy_rect.width = scarfy.width / 6;
    // scarfy_rect.height = scarfy.height;
    // scarfy_rect.x = 0;
    // scarfy_rect.y = 0;
    // Vector2 scarfy_pos;
    // scarfy_pos.x = 100;
    // scarfy_pos.y = window_height - scarfy_rect.height;
    // int frame = 0;
    // const float update_time = 1.0 / 12.0;
    // float running_time = 0;
    // int velocity = 0;

    bool is_in_air = false;

    // jump velocity pixel/sec
    const int jump_vel = -1000;

    // Nebula
    Texture2D nebula = LoadTexture("./textures/12_nebula_spritesheet.png");
    Rectangle nebula_rect =
        Rectangle{0, 0, (float)nebula.width / 8, (float)nebula.height / 8};
    Vector2 nebula_pos =
        Vector2{window_width, window_height - nebula_rect.height};

    AnimData nebData =
        AnimData{nebula_rect, nebula_pos, 0, 1.0 / 12.0, 0, -250};
    // int nebula_vel = -250;
    // int nebula_frame = 0;
    // const float nebula_update_time = 1.0 / 12.0;
    // float nebula_running_time = 0;

    Rectangle nebula_rect_2 =
        Rectangle{0, 0, (float)nebula.width / 8, (float)nebula.height / 8};
    Vector2 nebula_pos_2 =
        Vector2{window_width + 300, window_height - nebula_rect.height};

    AnimData nebData2 =
        AnimData{nebula_rect_2, nebula_pos_2, 0, 1.0 / 12.0, 0, -250};

    const int size_of_nebulae = 6;
    AnimData nebulae[size_of_nebulae];

    for (int i = 0; i < size_of_nebulae; i++) {
        nebulae[i].rec =
            Rectangle{0, 0, (float)nebula.width / 8, (float)nebula.height / 8};
        nebulae[i].pos =
            Vector2{window_width, window_height - nebula_rect.height};
        nebulae[i].pos.x = window_width + float(i * 300);
        nebulae[i].frame = 0;
        nebulae[i].running_time = 1.0 / 12.0;
        nebulae[i].update_time = 0;
        nebulae[i].velocity = -250;
    }

    // int nebula_vel_2 = -250;
    // int nebula_frame_2 = 0;
    // const float nebula_update_time_2 = 1.0 / 16.0;
    // float nebula_running_time_2 = 0;

    // Background
    Texture2D background = LoadTexture("./textures/far-buildings.png");
    float bg_x = 0.0;

    // Midground
    Texture2D midground = LoadTexture("./textures/back-buildings.png");
    float mg_x = 0.0;

    Texture2D foreground = LoadTexture("./textures/foreground.png");
    float fg_x = 0.0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        float dt = GetFrameTime();

        // game Logic

        // draw the background
        bg_x -= 20 * dt;
        mg_x -= 40 * dt;
        fg_x -= 80 * dt;

        if (bg_x < -background.width * 2) {
            bg_x = 0;
        }
        if (mg_x < -midground.width * 2) {
            mg_x = 0;
        }
        if (fg_x < -foreground.width * 2) {
            fg_x = 0;
        }

        Vector2 bg_pos_1{bg_x, 0.0};
        Vector2 bg_pos_2{bg_x + background.width * 2, 0.0};
        DrawTextureEx(background, bg_pos_1, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg_pos_2, 0.0, 2.0, WHITE);

        Vector2 mg_pos_1{mg_x, 0.0};
        Vector2 mg_pos_2{mg_x + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg_pos_1, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg_pos_2, 0.0, 2.0, WHITE);

        Vector2 fg_pos_1{fg_x, 0.0};
        Vector2 fg_pos_2{fg_x + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg_pos_1, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg_pos_2, 0.0, 2.0, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, window_height)) {
            // rect is on the ground
            scarfyData.velocity = 0;
            is_in_air = false;
        } else {
            // apply gravity
            scarfyData.velocity += gravity * dt;
        }

        if (IsKeyPressed(KEY_SPACE) && !is_in_air) {
            scarfyData.velocity += jump_vel;
            is_in_air = true;
        }

        // update position
        scarfyData.pos.y += scarfyData.velocity * dt;

        if (!is_in_air) {
            scarfyData = updateAnimData(scarfyData, dt, 5);
            // scarfyData.running_time += dt;
            // if (scarfyData.running_time >= scarfyData.update_time) {
            //     scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            //     scarfyData.running_time = 0;
            //     scarfyData.frame++;
            //     if (scarfyData.frame > 5) scarfyData.frame = 0;
            // }
        }

        for (int i = 0; i < size_of_nebulae; i++) {
            nebulae[i].pos.x += nebulae[i].velocity * dt;

            nebulae[i] = updateAnimData(nebulae[i], dt, 7);

            // nebulae[i].running_time += dt;
            // if (nebulae[i].running_time >= nebulae[i].update_time) {
            //     nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
            //     nebulae[i].running_time = 0;
            //     nebulae[i].frame++;
            //     if (nebulae[i].frame > 7) nebulae[i].frame = 0;
            // }
        }

        float finish_line = nebulae[size_of_nebulae - 1].pos.x;

        for (AnimData nebula : nebulae) {
            float pad = 30;

            Rectangle neb_rec = Rectangle{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad,
            };
            DrawRectangleLines(neb_rec.x, neb_rec.y, neb_rec.width,
                               neb_rec.height, GREEN);
            Rectangle scarfy_rect = Rectangle{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height,
            };
            if (CheckCollisionRecs(neb_rec, scarfy_rect)) {
                collision = true;
            }
        }

        // nebData.pos.x += nebData.velocity * dt;

        // nebData.running_time += dt;
        // if (nebData.running_time >= nebData.update_time) {
        //     nebData.rec.x = nebData.frame * nebData.rec.width;
        //     nebData.running_time = 0;
        //     nebData.frame++;
        //     if (nebData.frame > 7) nebData.frame = 0;
        // }

        // nebData2.pos.x += nebData2.velocity * dt;

        // nebData2.running_time += dt;
        // if (nebData2.running_time >= nebData2.update_time) {
        //     nebData2.rec.x = nebData2.frame * nebData2.rec.width;
        //     nebData2.running_time = 0;
        //     nebData2.frame++;
        //     if (nebData2.frame > 7) nebData2.frame = 0;
        // }

        // nebula_pos_2.x += nebula_vel_2 * dt;

        // nebula_running_time_2 += dt;
        // if (nebula_running_time_2 >= nebula_update_time_2) {
        //     nebula_rect_2.x = nebula_frame_2 * nebula_rect_2.width;
        //     nebula_running_time_2 = 0;
        //     nebula_frame_2++;
        //     if (nebula_frame_2 > 7) nebula_frame_2 = 0;
        // }

        if (collision) {
            DrawText("Game over", 10, 10, 20, RED);
        } else if (!collision && scarfyData.pos.x > finish_line) {
            DrawText("YOU WON", window_width / 2, 10, 40, RED);
        } else {
            // draw here
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            DrawRectangleLines(scarfyData.pos.x, scarfyData.pos.y,
                               scarfyData.rec.width, scarfyData.rec.height,
                               RED);
            for (int i = 0; i < size_of_nebulae; i++) {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
        }

        // DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE);
        // DrawTextureRec(nebula, nebData2.rec, nebData2.pos, RED);
        // DrawTextureRec(nebula, nebula_rect_2, nebula_pos_2, RED);

        // end drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}