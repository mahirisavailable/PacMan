#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define row 31
#define col 28
#define space 100
#define speed 250

int main(void)
{
    InitWindow(col * 25 + 2 * space, row * 25 + 1.5 * space, "PacMan");
    InitAudioDevice();
    SetTargetFPS(60);

    int width = col * 25 + 2 * space;
    int height = row * 25 + 1.5 * space;

    // Texture Loading
    Texture2D pac_left[3], pac_right[3], pac_up[3], pac_down[3], bg, idle;
    bg = LoadTexture("assets/bg.png");
    idle = LoadTexture("assets/idle.png");
    Texture2D apple = LoadTexture("assets/other/apple.png");
    Texture2D heart = LoadTexture("assets/other/heart.png");
    Texture2D logo = LoadTexture("assets/pacman-logo.png");
    Texture2D play = LoadTexture("assets/play-button.png");
    Texture2D hudai = LoadTexture("assets/other/hudai.png");
    Texture2D blinky = LoadTexture("assets/ghosts/blinky.png");

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

    int point = 0;
    int life = 3;
    bool menu = true;

restart:
    char maze[31][30] = {
        "############################", // 00
        "#............##............#", // 01
        "#.####.#####.##.#####.####.#", // 02
        "#o#  #.#   #.##.#   #.#  #o#", // 03
        "#.####.#####.##.#####.####.#", // 04
        "#..........................#", // 05
        "#.####.##.########.##.####.#", // 06
        "#.####.##.########.##.####.#", // 07
        "#......##....##....##......#", // 08
        "######.##### ## #####.######", // 09
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

    // Variables
    const Vector2 origin = {0, 0};
    Vector2 pac_pos = {13 * 25 + space, 23 * 25 + space};
    Vector2 pac_speed = {0, 0};

    // blinky
    Vector2 blinky_pos = {13 * 25 + space, 14 * 25 + space};
    Vector2 blinky_speed = {0, 0};
    char *blinky_hmove = "right";
    char *blinky_vmove = "up";

    char *nextmove = "null";

    int dots = 240;
    int bigdots = 4;
    float appletime;
    float countdown;
    float blinky_skatter;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        float dt = GetFrameTime();
        int curr = GetTime() * 15;
        float currenttime = GetTime();
        float alpha = (sinf(curr / 2) + 1.0f) / 2.0f;

        if (menu)
        {
            DrawTexturePro(logo, (Rectangle){0, 0, logo.width, logo.height}, (Rectangle){space, space, col * 25, 200}, origin, 0, WHITE);
            DrawTexturePro(hudai, (Rectangle){0, 0, hudai.width, hudai.height}, (Rectangle){space, height - 350 - space, 400, 400}, origin, 0, WHITE);
            Rectangle button = {width / 2 - 125, height / 2 - 100, 250, 100};
            DrawTexturePro(play, (Rectangle){0, 0, play.width, play.height}, button, origin, 0, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                menu = false;
                countdown = currenttime;
            }
            EndDrawing();
            continue;
        }

        DrawTexturePro(bg, (Rectangle){0, 0, bg.width, bg.height}, (Rectangle){space, space - 20, col * 25, row * 25 + 15}, origin, 0, WHITE);
        DrawText(TextFormat("Points: %d", point), width - space - 300, space - 50, 50, RAYWHITE);
        for (int i = 0; i < life; i++)
            DrawTexturePro(heart, (Rectangle){0, 0, heart.width, heart.height}, (Rectangle){space + 50 * i, space - 50, 50, 50}, origin, 0, WHITE);

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
        if (currenttime - appletime >= 8 && maze[17][13] == 'a')
            maze[17][13] = 'A';

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
        double x, y, moe = GetFrameTime() * speed / 50;
        x = (pac_pos.x - space) / 25;
        y = (pac_pos.y - space) / 25;
        if (x + moe >= round(x) && x - moe <= round(x) && y + moe >= round(y) && y - moe <= round(y))
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

        // Countdown
        if ((currenttime - countdown) < 4)
        {
            int cd = 3 - (int)(currenttime - countdown);
            if (cd)
                DrawText(TextFormat("%d", cd), width / 2 - 10, 17 * 25 + space - 10, 50, YELLOW);
            else
                DrawText("READY", width / 2 - 65, 17 * 25 + space - 5, 40, YELLOW);
            pac_speed = (Vector2){0, 0};
            blinky_skatter = currenttime;
        }

        // Position Update
        pac_pos = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt));
        if (pac_pos.x + 12.5 < space)
            pac_pos.x += col * 25;
        else if (pac_pos.x + 12.5 > space + col * 25)
            pac_pos.x -= col * 25;

        // blinky position and speed update
        if ((currenttime - blinky_skatter) > 3 && blinky_pos.x + moe <= pac_pos.x)
        {
            blinky_hmove = "right";
        }
        else if ((currenttime - blinky_skatter) > 3 && blinky_pos.x - moe >= pac_pos.x)
        {
            blinky_hmove = "left";
        }
        if ((currenttime - blinky_skatter) > 3 && blinky_pos.y + moe <= pac_pos.y)
        {
            blinky_vmove = "down";
        }
        else if ((currenttime - blinky_skatter) > 3 && blinky_pos.y - moe >= pac_pos.y)
        {
            blinky_vmove = "up";
        }
        if ((currenttime - blinky_skatter) > 10)
        {
            blinky_skatter = currenttime;
        } else if ((currenttime - blinky_skatter) < 3)
        {
            blinky_hmove = "right";
            blinky_vmove = "up";
        }

        double blinky_x, blinky_y;
        blinky_x = (blinky_pos.x - space) / 25;
        blinky_y = (blinky_pos.y - space) / 25;

        if (blinky_x + moe >= round(blinky_x) && blinky_x - moe <= round(blinky_x) && blinky_y + moe >= round(blinky_y) && blinky_y - moe <= round(blinky_y))
        {
            // Collision with front block
            if (maze[(int)round(blinky_y + blinky_speed.y / speed)][(int)round(blinky_x + blinky_speed.x / speed)] == '#')
            {
                blinky_speed.x = 0;
                blinky_speed.y = 0;
            }
            if (blinky_vmove == "up")
            {
                if (maze[(int)round(blinky_y) - 1][(int)round(blinky_x)] != '#' && ((currenttime - blinky_skatter) < 3 || blinky_pos.y - 2*moe > pac_pos.y))
                {
                    blinky_speed.x = 0;
                    blinky_speed.y = -speed;
                }
                else if (blinky_hmove == "right")
                {
                    if (maze[(int)round(blinky_y)][(int)round(blinky_x) + 1] != '#')
                    {
                        blinky_speed.y = 0;
                        blinky_speed.x = speed;
                    }
                }
                else if (blinky_hmove == "left")
                {
                    if (maze[(int)round(blinky_y)][(int)round(blinky_x) - 1] != '#')
                    {
                        blinky_speed.y = 0;
                        blinky_speed.x = -speed;
                    }
                }
            }
            else if (blinky_vmove == "down")
            {
                if (maze[(int)round(blinky_y) + 1][(int)round(blinky_x)] != '#' && maze[(int)round(blinky_y) + 1][(int)round(blinky_x)] != '-' && ((currenttime - blinky_skatter) < 3 || blinky_pos.y + 2*moe < pac_pos.y))
                {
                    blinky_speed.x = 0;
                    blinky_speed.y = speed;
                }
                else if (blinky_hmove == "right")
                {
                    if (maze[(int)round(blinky_y)][(int)round(blinky_x) + 1] != '#')
                    {
                        blinky_speed.y = 0;
                        blinky_speed.x = speed;
                    }
                }
                else if (blinky_hmove == "left")
                {
                    if (maze[(int)round(blinky_y)][(int)round(blinky_x) - 1] != '#')
                    {
                        blinky_speed.y = 0;
                        blinky_speed.x = -speed;
                    }
                }
            }
        }
        // printf("vmove : %f , hmove : %f\n", currenttime-blinky_skatter, blinky_skatter);

        if ((currenttime - countdown) > 4)
        {
            blinky_pos = Vector2Add(blinky_pos, Vector2Scale(blinky_speed, dt));
            if (blinky_pos.x + 12.5 < space)
                blinky_pos.x += col * 25;
            else if (blinky_pos.x + 12.5 > space + col * 25)
                blinky_pos.x -= col * 25;
        }

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

        // blinky
        Rectangle blinkyblinky = {blinky_pos.x - 5, blinky_pos.y - 5, 35, 35};
        DrawTexturePro(blinky, (Rectangle){0, 0, blinky.width, blinky.height}, blinkyblinky, origin, 0, WHITE);

        if (CheckCollisionRecs(pacpac, blinkyblinky))
        {
            life--;
            countdown = currenttime;
            blinky_pos = (Vector2){13 * 25 + space, 14 * 25 + space};
            pac_pos = (Vector2){13 * 25 + space, 23 * 25 + space};
            nextmove = "null";
            blinky_hmove = "right";
            blinky_vmove = "up";
        }

        EndDrawing();
        if (life == 0)
        {
            menu = true;
            life = 3;
            point = 0;
            goto restart;
        }
        if (dots == 0 && bigdots == 0)
        {
            countdown = currenttime;
            goto restart;
        }
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
    UnloadTexture(heart);
    UnloadTexture(logo);
    UnloadTexture(play);
    UnloadTexture(hudai);
    UnloadTexture(blinky);

    // Unload Sound
    UnloadSound(chomp);
    UnloadSound(eatfruit);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
