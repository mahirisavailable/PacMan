#include "raylib.h"

#define col 

int main(void)
{
    InitWindow(win_width, win_height, "PacMan");
    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
