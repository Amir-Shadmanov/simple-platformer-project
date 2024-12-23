#include "raylib.h"
#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 480
#define COLLISION_OFFSET 0.1f

void update_game() {
    game_frame++;

    switch (game_state) {
        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                move_player_horizontally(MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                move_player_horizontally(-MOVEMENT_SPEED);
            }

            is_player_on_ground = is_colliding({player_pos.x, player_pos.y + COLLISION_OFFSET}, WALL);
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
                player_y_velocity = -JUMP_STRENGTH;
            }
            break;

        case VICTORY_STATE:
            if (IsKeyDown(KEY_SPACE)) {
                reset_game();
            }
            break;
        case PAUSE_STATE:
            if (IsKeyPressed(KEY_R)) {
                game_state = GAME_STATE; // Resume game on pressing 'R'
            } else if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE; // Resume game on pressing 'ESCAPE'
            }
        break;

        case MENU_STATE:
            if (IsKeyDown(KEY_ENTER)) {
                game_state = GAME_STATE;
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyDown(KEY_SPACE)) {
                reset_game();
            }
            break;
    }

    if (IsKeyDown(KEY_ESCAPE)) {
        game_state = PAUSE_STATE;
    }

    update_player();
}

void draw_game() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (game_state) {
        case MENU_STATE:
            draw_menu();
            break;

        case GAME_STATE:
            draw_level();
            draw_game_overlay();
            break;

        case PAUSE_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;

        case GAME_OVER_STATE:
            draw_game_over_menu();
            break;

        case VICTORY_STATE:
            draw_victory_menu();
            draw_victory_menu_background();
            break;
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();
    load_level();

    while (!WindowShouldClose()) {
        update_game();
        draw_game();
    }

    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

