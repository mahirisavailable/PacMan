#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define row 31
#define col 28
#define space 100
#define speed 250

void ghost_direction(char[][30], char[][30], Vector2, Vector2*, char**, Vector2, double, double);

int main(void)
{
    int width = col * 25 + 2 * space;
    int height = row * 25 + 1.5 * space;

    InitWindow(width, height, "PacMan");
    InitAudioDevice();
    SetTargetFPS(60);

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
    Texture2D pinky = LoadTexture("assets/ghosts/pinky.png");
    Texture2D inky = LoadTexture("assets/ghosts/inky.png");
    Texture2D clyde = LoadTexture("assets/ghosts/clyde.png");

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

    char decision[31][30] = {
        "############################", // 00
        "#d....d.....d##d.....d....d#", // 01
        "#.####.#####.##.#####.####.#", // 02
        "#o#  #.#   #.##.#   #.#  #o#", // 03
        "#.####.#####.##.#####.####.#", // 04
        "#d....d..d..d..d..d..d....d#", // 05
        "#.####.##.########.##.####.#", // 06
        "#.####.##.########.##.####.#", // 07
        "#d....d##d..d##d..d##d....d#", // 08
        "######.##### ## #####.######", // 09
        "     #.##### ## #####.#     ", // 10
        "     #.##d   dd   d##.#     ", // 11
        "     #.## ###--### ##.#     ", // 12
        "######.## #      # ##.######", // 13
        "      d  d#      #d  d      ", // 14
        "######.## #      # ##.######", // 15
        "     #.## ######## ##.#     ", // 16
        "     #.##d        d##.#     ", // 17
        "     #.## ######## ##.#     ", // 18
        "######.## ######## ##.######", // 19
        "#d....d..d..d##d..d..d....d#", // 20
        "#.####.#####.##.#####.####.#", // 21
        "#.####.#####.##.#####.####.#", // 22
        "#o.d##d..d..d  d..d..d##d.o#", // 23
        "###.##.##.########.##.##.###", // 24
        "###.##.##.########.##.##.###", // 25
        "#d.d..d##d..d##d..d##d..d.d#", // 26
        "#.##########.##.##########.#", // 27
        "#.##########.##.##########.#", // 28
        "#d..........d..d..........d#", // 29
        "############################", // 30
    };

    // Variables
    const Vector2 origin = {0, 0};
    Vector2 pac_pos = {13 * 25 + space, 23 * 25 + space};
    Vector2 pac_speed = {0, 0};

    // blinky
    Vector2 blinky_pos = {13 * 25 + space, 14 * 25 + space - 15};
    Vector2 blinky_target;
    Vector2 blinky_speed = {0, -speed * 0.8};
    char *blinkymove = "up";

    // pinky
    Vector2 pinky_pos = {14 * 25 + space, 14 * 25 + space - 15};
    Vector2 pinky_target;
    Vector2 pinky_speed = {0, -speed * 0.8};
    char *pinkymove = "up";

    // clyde
    Vector2 clyde_pos = {14 * 25 + space, 15 * 25 + space};
    Vector2 clyde_target;
    Vector2 clyde_speed = {0, -speed * 0.8};
    char *clydemove = "up";

    char *pacmove = "null";

    int dots = 240;
    int bigdots = 4;
    float appletime;
    float countdown;
    float blinky_skatter = 12;
    float pinky_skatter = 12;

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
                //     DrawRectanglePro(cell, origin, 0, (Color){255, 255, 255, 150});
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
        if ((currenttime - countdown) > 4)
        {
            if (IsKeyPressed(KEY_LEFT))
                pacmove = "left";
            else if (IsKeyPressed(KEY_RIGHT))
                pacmove = "right";
            else if (IsKeyPressed(KEY_UP))
                pacmove = "up";
            else if (IsKeyPressed(KEY_DOWN))
                pacmove = "down";
        }

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
            if (pacmove == "left" && maze[(int)round(y)][(int)round(x) - 1] != '#')
            {
                pac_speed.x = -speed;
                pac_speed.y = 0;
            }
            else if (pacmove == "right" && maze[(int)round(y)][(int)round(x) + 1] != '#')
            {
                pac_speed.x = speed;
                pac_speed.y = 0;
            }
            else if (pacmove == "up" && maze[(int)round(y) - 1][(int)round(x)] != '#')
            {
                pac_speed.x = 0;
                pac_speed.y = -speed;
            }
            else if (pacmove == "down" && maze[(int)round(y) + 1][(int)round(x)] != '#')
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

        // blinky direction
        double blinky_x, blinky_y;
        blinky_x = (blinky_pos.x - space) / 25;
        blinky_y = (blinky_pos.y - space) / 25;

        if (currenttime - blinky_skatter < 3)
        {
            blinky_target = (Vector2){width, 0};
        }
        else if (currenttime - blinky_skatter < 10)
        {
            blinky_target = pac_pos;
        }
        else
        {
            blinky_skatter = currenttime;
        }
        ghost_direction(maze, decision, blinky_pos, &blinky_speed, &blinkymove, blinky_target, moe*0.8, speed*0.8);

        // Position Update
        if ((currenttime - countdown) > 4)
        {
            blinky_pos = Vector2Add(blinky_pos, Vector2Scale(blinky_speed, dt));
            if (blinky_pos.x + 12.5 < space)
                blinky_pos.x += col * 25;
            else if (blinky_pos.x + 12.5 > space + col * 25)
                blinky_pos.x -= col * 25;
        }

        // pinky direction
        double pinky_x, pinky_y;
        pinky_x = (pinky_pos.x - space) / 25;
        pinky_y = (pinky_pos.y - space) / 25;

        if (currenttime - pinky_skatter < 3)
        {
            pinky_target = (Vector2){0, 0};
        }
        else if (currenttime - pinky_skatter < 10)
        {
            pinky_target = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt*30));
        }
        else
        {
            pinky_skatter = currenttime;
        }
        ghost_direction(maze, decision, pinky_pos, &pinky_speed, &pinkymove, pinky_target, moe*0.8, speed*0.8);

        // Position Update
        if ((currenttime - countdown) > 10)
        {
            pinky_pos = Vector2Add(pinky_pos, Vector2Scale(pinky_speed, dt));
            if (pinky_pos.x + 12.5 < space)
                pinky_pos.x += col * 25;
            else if (pinky_pos.x + 12.5 > space + col * 25)
                pinky_pos.x -= col * 25;
        }

        // clyde direction
        double clyde_x, clyde_y;
        clyde_x = (clyde_pos.x - space) / 25;
        clyde_y = (clyde_pos.y - space) / 25;

        if ((clyde_pos.x-blinky_pos.x) < 50 && (clyde_pos.x-blinky_pos.x) > -50 && (clyde_pos.y-blinky_pos.y) < 50 && (clyde_pos.y-blinky_pos.y) > -50)
        {
            clyde_target = (Vector2){0, height};
        }
        else
        {
            clyde_target = pac_pos;
        }
        ghost_direction(maze, decision, clyde_pos, &clyde_speed, &clydemove, clyde_target, moe*0.8, speed*0.8);

        // Position Update
        if ((currenttime - countdown) > 16)
        {
            clyde_pos = Vector2Add(clyde_pos, Vector2Scale(clyde_speed, dt));
            if (clyde_pos.x + 12.5 < space)
                clyde_pos.x += col * 25;
            else if (clyde_pos.x + 12.5 > space + col * 25)
                clyde_pos.x -= col * 25;
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
            pinky_skatter = currenttime + 6;
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
            if (pacmove == "up")
                DrawTexturePro(pac_up[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (pacmove == "down")
                DrawTexturePro(pac_down[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (pacmove == "left")
                DrawTexturePro(pac_left[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else if (pacmove == "right")
                DrawTexturePro(pac_right[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
            else
                DrawTexturePro(idle, (Rectangle){0, 0, idle.width, idle.height}, (Rectangle){pac_pos.x - 5 + 12.5, pac_pos.y - 5, 35, 35}, origin, 0, WHITE);
        }

        // blinky
        Rectangle blinkyblinky = {blinky_pos.x - 5, blinky_pos.y - 5, 35, 35};
        DrawTexturePro(blinky, (Rectangle){0, 0, blinky.width, blinky.height}, blinkyblinky, origin, 0, WHITE);

        // pinky
        Rectangle pinkypinky = {pinky_pos.x - 5, pinky_pos.y - 5, 35, 35};
        DrawTexturePro(pinky, (Rectangle){0, 0, pinky.width, pinky.height}, pinkypinky, origin, 0, WHITE);
        
        // clyde
        Rectangle clydeclyde = {clyde_pos.x - 5, clyde_pos.y - 5, 35, 35};
        DrawTexturePro(clyde, (Rectangle){0, 0, clyde.width, clyde.height}, clydeclyde, origin, 0, WHITE);

        if (currenttime - countdown > 5 &&CheckCollisionRecs(blinkyblinky, pinkypinky)) {
            blinky_speed = Vector2Scale(blinky_speed, -1);
            pinky_speed = Vector2Scale(pinky_speed, -1);
            if (blinkymove=="up") blinkymove = "down";
            else if (blinkymove=="down") blinkymove = "up";
            else if (blinkymove=="left") blinkymove = "right";
            else blinkymove = "left";
            if (pinkymove=="up") pinkymove = "down";
            else if (pinkymove=="down") pinkymove = "up";
            else if (pinkymove=="left") pinkymove = "right";
            else pinkymove = "left";
        }

        if (currenttime - countdown > 11 &&CheckCollisionRecs(blinkyblinky, clydeclyde)) {
            blinky_speed = Vector2Scale(blinky_speed, -1);
            clyde_speed = Vector2Scale(clyde_speed, -1);
            if (blinkymove=="up") blinkymove = "down";
            else if (blinkymove=="down") blinkymove = "up";
            else if (blinkymove=="left") blinkymove = "right";
            else blinkymove = "left";
            if (clydemove=="up") clydemove = "down";
            else if (clydemove=="down") clydemove = "up";
            else if (clydemove=="left") clydemove = "right";
            else clydemove = "left";
        }

        if (currenttime - countdown > 11 &&CheckCollisionRecs(clydeclyde, pinkypinky)) {
            clyde_speed = Vector2Scale(clyde_speed, -1);
            pinky_speed = Vector2Scale(pinky_speed, -1);
            if (clydemove=="up") clydemove = "down";
            else if (clydemove=="down") clydemove = "up";
            else if (clydemove=="left") clydemove = "right";
            else clydemove = "left";
            if (pinkymove=="up") pinkymove = "down";
            else if (pinkymove=="down") pinkymove = "up";
            else if (pinkymove=="left") pinkymove = "right";
            else pinkymove = "left";
        }

        if (CheckCollisionRecs(pacpac, blinkyblinky) || CheckCollisionRecs(pacpac, pinkypinky) || CheckCollisionRecs(pacpac, clydeclyde))
        {
            life--;
            countdown = currenttime;
            blinky_pos = (Vector2){13 * 25 + space, 14 * 25 + space - 15};
            pinky_pos = (Vector2){14 * 25 + space, 14 * 25 + space - 15};
            clyde_pos = (Vector2){14 * 25 + space, 15 * 25 + space};
            pac_pos = (Vector2){13 * 25 + space, 23 * 25 + space};
            pacmove = "null";
            blinkymove = "up";
            pinkymove = "up";
            clydemove = "up";
            blinky_speed = (Vector2){0, -speed * 0.8};
            pinky_speed = (Vector2){0, -speed * 0.8};
            clyde_speed = (Vector2){0, -speed * 0.8};
            appletime = currenttime - 8;
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
    UnloadTexture(inky);
    UnloadTexture(pinky);
    UnloadTexture(clyde);

    // Unload Sound
    UnloadSound(chomp);
    UnloadSound(eatfruit);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void ghost_direction(char maze[][30], char decision[][30], Vector2 pos, Vector2 *g_speed, char **move, Vector2 target, double moe, double gspd)
{
    double gx = (pos.x - space) / 25;
    double gy = (pos.y - space) / 25;

    if (gx + moe >= round(gx) && gx - moe <= round(gx) && gy + moe >= round(gy) && gy - moe <= round(gy) && decision[(int)round(gy)][(int)round(gx)] == 'd')
    {
        if (*move == "up")
        {
            if (maze[(int)round(gy) - 1][(int)round(gx)] != '#' && target.y < pos.y)
            {
                *move = "up";
                g_speed->x = 0; // (*g_speed).x
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) - 1] != '#' && target.x < pos.x)
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) + 1] != '#' && target.x > pos.x)
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else if (maze[(int)round(gy) - 1][(int)round(gx)] != '#')
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) - 1] != '#')
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) + 1] != '#')
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
        }
        else if (*move == "down")
        {
            if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && target.y > pos.y)
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) + 1] != '#' && target.x > pos.x)
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) - 1] != '#' && target.x < pos.x)
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#')
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) - 1] != '#')
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) + 1] != '#')
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
        }
        else if (*move == "right")
        {
            if (maze[(int)round(gy)][(int)round(gx) + 1] != '#' && target.x > pos.x)
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else if (maze[(int)round(gy) - 1][(int)round(gx)] != '#' && target.y < pos.y)
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && target.y > pos.y)
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) + 1] != '#')
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
            else if (maze[(int)round(gy) - 1][(int)round(gx)] != '#')
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#')
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
        }
        else
        {
            if (maze[(int)round(gy)][(int)round(gx) - 1] != '#' && target.x < pos.x)
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy) - 1][(int)round(gx)] != '#' && target.y < pos.y)
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && target.y > pos.y)
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else if (maze[(int)round(gy)][(int)round(gx) - 1] != '#')
            {
                *move = "left";
                g_speed->y = 0;
                g_speed->x = -gspd;
            }
            else if (maze[(int)round(gy) - 1][(int)round(gx)] != '#')
            {
                *move = "up";
                g_speed->x = 0;
                g_speed->y = -gspd;
            }
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#')
            {
                *move = "down";
                g_speed->x = 0;
                g_speed->y = gspd;
            }
            else
            {
                *move = "right";
                g_speed->y = 0;
                g_speed->x = gspd;
            }
        }
    }
}
