#include "raylib.h"
#include "raymath.h"

#define row 31
#define col 28
#define space 100
#define speed 100

char maze[31][30] = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o#  #.#   #.##.#   #.#  #o#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "#    #.##### ## #####.#    #",
    "#    #.##          ##.#    #",
    "#    #.## ###--### ##.#    #",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "#    #.## ######## ##.#    #",
    "#    #.##          ##.#    #",
    "#    #.## ######## ##.#    #",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#.####.#####.##.#####.####.#",
    "#o..##................##..o#",
    "###.##.##.########.##.##.###",
    "###.##.##.########.##.##.###",
    "#......##....##....##......#",
    "#.##########.##.##########.#",
    "#.##########.##.##########.#",
    "#..........................#",
    "############################",
};

int main(void)
{
    InitWindow(col*25 + 2*space, row*25 + 2*space, "PacMan");
    SetTargetFPS(60);

    // Texture Loading
    Texture2D pac_left[3], pac_right[3], pac_up[3], pac_down[3];
    for (int i=0; i<3; i++) {
        pac_left[i] = LoadTexture(TextFormat("assets/pacman-left/%d.png", i+1));
        pac_right[i] = LoadTexture(TextFormat("assets/pacman-right/%d.png", i+1));
        pac_up[i] = LoadTexture(TextFormat("assets/pacman-up/%d.png", i+1));
        pac_down[i] = LoadTexture(TextFormat("assets/pacman-down/%d.png", i+1));
    }

    // Variables
    const Vector2 origin = {0, 0};
    Vector2 pac_pos = {13*25+ 12.5 +space, 23*25+space};
    Vector2 pac_speed = {0, 0};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        float dt = GetFrameTime();
        int curr = GetTime()*20;

        // Maze
        for (int i=0; i<row; i++) {
            for (int j=0; j<col; j++) {                
                if (maze[i][j]=='#') {
                    Rectangle cell = {j*25+5 + space, i*25+5 + space, 15, 15};
                    DrawRectanglePro(cell, origin, 0, BLUE);
                }
                if (maze[i][j]=='.') DrawCircle(j*25+12 + space, i*25+12 + space, 3, RAYWHITE);
                if (maze[i][j]=='o') DrawCircle(j*25+12 + space, i*25+12 + space, 10, RED);
            }
        }

        // Direction input
        if (IsKeyPressed(KEY_LEFT)) {
            pac_speed.x = -speed;
            pac_speed.y = 0;
        } else if (IsKeyPressed(KEY_RIGHT)) {
            pac_speed.x = speed;
            pac_speed.y = 0;    
        } else if (IsKeyPressed(KEY_UP)) {
            pac_speed.x = 0;
            pac_speed.y = -speed;    
        } else if (IsKeyPressed(KEY_DOWN)) {
            pac_speed.x = 0;
            pac_speed.y = speed;    
        }

        // Position Update
        pac_pos = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt));

        // Draw PacMan
        Rectangle pacpac = {pac_pos.x, pac_pos.y, 25, 25};
        if (pac_speed.x<=0 && pac_speed.y==0) DrawTexturePro(pac_left[curr%3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x>0 && pac_speed.y==0) DrawTexturePro(pac_right[curr%3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x==0 && pac_speed.y<0) DrawTexturePro(pac_up[curr%3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x==0 && pac_speed.y>0) DrawTexturePro(pac_down[curr%3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        
        EndDrawing();
    }
    
    // Unload
    for (int i=0; i<3; i++) {
        UnloadTexture(pac_left[i]);
        UnloadTexture(pac_right[i]);
        UnloadTexture(pac_up[i]);
        UnloadTexture(pac_down[i]);
    }

    CloseWindow();

    return 0;
}
