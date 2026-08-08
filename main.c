#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define row 31
#define col 28
#define space 100
#define speed 250

char maze[31][30] = {
    "############################", // 0
    "#............##............#", // 1
    "#.####.#####.##.#####.####.#", // 2
    "#o#  #.#   #.##.#   #.#  #o#", // 3
    "#.####.#####.##.#####.####.#", // 4
    "#..........................#", // 5
    "#.####.##.########.##.####.#", // 6
    "#.####.##.########.##.####.#", // 7
    "#......##....##....##......#", // 8
    "######.##### ## #####.######", // 9
    "     #.##### ## #####.#     ", // 10
    "     #.##          ##.#     ", // 11
    "     #.## ###--### ##.#     ", // 12
    "######.## #      # ##.######", // 13
    "      .   #      #   .      ", // 14
    "######.## #      # ##.######", // 15
    "     #.## ######## ##.#     ", // 16
    "     #.##          ##.#     ", // 17
    "     #.## ######## ##.#     ", // 18
    "######.## ######## ##.######", // 19
    "#............##............#", // 20
    "#.####.#####.##.#####.####.#", // 21
    "#.####.#####.##.#####.####.#", // 22
    "#o..##.......  .......##..o#", // 23
    "###.##.##.########.##.##.###", // 24
    "###.##.##.########.##.##.###", // 25
    "#......##....##....##......#", // 26
    "#.##########.##.##########.#", // 27
    "#.##########.##.##########.#", // 28
    "#..........................#", // 29
    "############################", // 30
};

int main(void)
{
    InitWindow(col * 25 + 2 * space, row * 25 + 1.5 * space, "PacMan");
    InitAudioDevice();
    SetTargetFPS(60);

    // Texture Loading
    Texture2D pac_left[3], pac_right[3], pac_up[3], pac_down[3], bg, idle;
    bg = LoadTexture("assets/bg.png");
    idle = LoadTexture("assets/idle.png");
    Texture2D apple = LoadTexture("assets/other/apple.png");
    for (int i = 0; i < 3; i++)
    {
        pac_left[i] = LoadTexture(TextFormat("assets/pacman-left/%d.png", i + 1));
        pac_right[i] = LoadTexture(TextFormat("assets/pacman-right/%d.png", i + 1));
        pac_up[i] = LoadTexture(TextFormat("assets/pacman-up/%d.png", i + 1));
        pac_down[i] = LoadTexture(TextFormat("assets/pacman-down/%d.png", i + 1));
    }

    // sound loading
    Sound chomp = LoadSound("assets/audio/pacman_chomp.wav");
    Sound eatfruit = LoadSound("assets/audio/pacman_eatfruit.wav");

    // Variables
    const Vector2 origin = {0, 0};
    Vector2 pac_pos = {13 * 25 + space, 23 * 25 + space};
    Vector2 pac_speed = {0, 0};
    // collision with maze
    char *nextmove = "null";

    int point = 0;
    int dots = 240;
    int bigdots = 4;
    float appletime;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        float dt = GetFrameTime();
        int curr = GetTime() * 15;
        float currenttime = GetTime();
        float alpha = (sinf(curr / 2) + 1.0f) / 2.0f;

        DrawTexturePro(bg, (Rectangle){0, 0, bg.width, bg.height}, (Rectangle){space, space - 20, col * 25, row * 25 + 15}, origin, 0, WHITE);
        DrawText(TextFormat("Points: %d", point), 500, 50, 50, RAYWHITE);

        // Maze
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                // if (maze[i][j] == '#')
                // {
                //     Rectangle cell = {j * 25 + space, i * 25 + space, 25, 25};
                //     DrawRectanglePro(cell, origin, 0, (Color){255, 255, 255, 50});
                // }
                if (maze[i][j] == '.')
                    DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 3, RAYWHITE);
                if (maze[i][j] == 'o')
                    DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 10, (Color){255, 0, 0, alpha * 255});
                if (maze[i][j] == '-')
                    DrawRectangle(j * 25 + space, i * 25 + 18 + space, 25, 2, RAYWHITE);
            }
        }

        // Apple logic
        if (dots <= 150 && maze[17][13] == ' ')
        {
            maze[17][13] = 'a';
            appletime = currenttime;
        }
        if (currenttime - appletime < 8 && maze[17][13] == 'a')
            DrawTexturePro(apple, (Rectangle){0, 0, apple.width, apple.height}, (Rectangle){13 * 25 + space - 5 + 12.5, 17 * 25 + space - 5, 35, 35}, origin, 0, WHITE);

        // Direction input
        if (IsKeyPressed(KEY_LEFT))
            nextmove = "left";
        else if (IsKeyPressed(KEY_RIGHT))
            nextmove = "right";
        else if (IsKeyPressed(KEY_UP))
            nextmove = "up";
        else if (IsKeyPressed(KEY_DOWN))
            nextmove = "down";

        // Direction execution & wall conflict
        double x, y, mor = GetFrameTime() * speed / 50;
        x = (pac_pos.x - space) / 25;
        y = (pac_pos.y - space) / 25;
        if (x + mor >= round(x) && x - mor <= round(x) && y + mor >= round(y) && y - mor <= round(y))
        {
            // Collision with front block
            if (maze[(int)round(y + pac_speed.y / speed)][(int)round(x + pac_speed.x / speed)] == '#')
            {
                pac_speed.x = 0;
                pac_speed.y = 0;
            }

            // Is next move available?
            if (nextmove == "left" && maze[(int)round(y)][(int)round(x) - 1] != '#')
            {
                pac_speed.x = -speed;
                pac_speed.y = 0;
            }
            else if (nextmove == "right" && maze[(int)round(y)][(int)round(x) + 1] != '#')
            {
                pac_speed.x = speed;
                pac_speed.y = 0;
            }
            else if (nextmove == "up" && maze[(int)round(y) - 1][(int)round(x)] != '#')
            {
                pac_speed.x = 0;
                pac_speed.y = -speed;
            }
            else if (nextmove == "down" && maze[(int)round(y) + 1][(int)round(x)] != '#')
            {
                pac_speed.x = 0;
                pac_speed.y = speed;
            }

            // Point system
            if (maze[(int)round(y)][(int)round(x)] == '.')
            {
                point += 10;
                dots--;
                maze[(int)round(y)][(int)round(x)] = ' ';
                PlaySound(chomp);
            }
            if (maze[(int)round(y)][(int)round(x)] == 'o')
            {
                point += 50;
                bigdots--;
                maze[(int)round(y)][(int)round(x)] = ' ';
                PlaySound(eatfruit);
            }
            if (maze[(int)round(y)][(int)round(x)] == 'a')
            {
                point += 200;
                maze[(int)round(y)][(int)round(x)] = 'A';
                PlaySound(eatfruit);
            }
        }

        // Position Update
        pac_pos = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt));
        if (pac_pos.x + 12.5 < space)
            pac_pos.x += col * 25;
        else if (pac_pos.x + 12.5 > space + col * 25)
            pac_pos.x -= col * 25;

        // Draw PacMan
        Rectangle pacpac = {pac_pos.x - 5, pac_pos.y - 5, 35, 35};
        if (pac_speed.x < 0 && pac_speed.y == 0)
            DrawTexturePro(pac_left[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x > 0 && pac_speed.y == 0)
            DrawTexturePro(pac_right[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x == 0 && pac_speed.y < 0)
            DrawTexturePro(pac_up[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else if (pac_speed.x == 0 && pac_speed.y > 0)
            DrawTexturePro(pac_down[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
        else
        {
            if (nextmove == "up")
                DrawTexturePro(pac_up[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (nextmove == "down")
                DrawTexturePro(pac_down[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (nextmove == "left")
                DrawTexturePro(pac_left[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (nextmove == "right")
                DrawTexturePro(pac_right[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else
                DrawTexturePro(idle, (Rectangle){0, 0, idle.width, idle.height}, (Rectangle){pac_pos.x - 5 + 12.5, pac_pos.y - 5, 35, 35}, origin, 0, WHITE);
        }

        EndDrawing();
    }

    // Unload Texture
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(pac_left[i]);
        UnloadTexture(pac_right[i]);
        UnloadTexture(pac_up[i]);
        UnloadTexture(pac_down[i]);
    }
    UnloadTexture(bg);
    UnloadTexture(idle);
    UnloadTexture(apple);

    // Unload Sound
    UnloadSound(chomp);
    UnloadSound(eatfruit);

    CloseWindow();

    return 0;
}
