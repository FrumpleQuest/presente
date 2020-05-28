
#include <raylib.h>
#include <math.h>

#include "level.h"
#include "draw.h"
#include "state.h"

#define INITIAL_SPAWN_RATE 40

int main(int argc, char const *argv[]){

    // Initialization
    const int screen_width = 800;
    const int screen_height = 600;  

    InitWindow(screen_width,screen_height,"Presente - the game");
    SetTargetFPS(60);

    // Initialize level and fill randomly
    level *lvl = level_new(20,20);
    level_fill_random(lvl,6);

    // Initialize state (now add enemies is at the bottom of the while loop, for respawn purposes)
    state *sta = state_new();

    // Main loo
    int FrameCounter = 0;
    int Enemy_spawn_rate = INITIAL_SPAWN_RATE;
    while(!WindowShouldClose()){
        
        // Update input depending if keys are pressed or not
        sta->button_state[0] = IsKeyDown(KEY_D);
        sta->button_state[1] = IsKeyDown(KEY_W);
        sta->button_state[2] = IsKeyDown(KEY_A);
        sta->button_state[3] = IsKeyDown(KEY_S);
        sta->button_state[4] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        // Compute the angle the player is looking at from the mouse's position relative to the screen's center
        float mouse_y = GetMouseY()-GetScreenHeight()/2;
        float mouse_x = GetMouseX()-GetScreenWidth()/2;
        sta->aim_angle = atan2(-mouse_y,mouse_x);

        // Update the state
        state_update(lvl,sta);

        // Drawing
        BeginDrawing();

            ClearBackground(RAYWHITE);
            draw_state(lvl, sta);

            DrawText("Presente profe!",190,200,20,LIGHTGRAY);

        EndDrawing();

        //:: Respawnean los monstruos cada 500 frames, ademas, cada vez que respawnean
        //:: aumenta la cantidad de enemigos que aparecen.
        FrameCounter += 1;
        if (FrameCounter % 500 == 1){
            state_populate_random(lvl,sta,Enemy_spawn_rate);
            Enemy_spawn_rate += INITIAL_SPAWN_RATE;
        }

    }

    // Closer window
    CloseWindow();

    //:: Liberamos memoria de específico a general (enemies < state).
    bullets_free(sta);
    enemies_free(sta);
    state_free(sta);
    level_free(lvl);

    return 0;
}
