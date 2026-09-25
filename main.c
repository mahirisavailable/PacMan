#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

#define row 31
#define col 28
#define space 100
#define speed 250

void ghost_direction(char[][30], char[][30], Vector2, Vector2 *, char **, Vector2, double, double);
void ghost_bounce(Vector2 *, Vector2 *, char **, char **);

int main(void)
{
    int width = col * 25 + 2 * space;
    int height = row * 25 + 1.5 * space;

    InitWindow(width, height, "PacMan");
    InitAudioDevice();
    SetTargetFPS(60);

    // Texture Loading
    Texture2D pac_left[3], pac_right[3], pac_up[3], pac_down[3], tom_ghost[4];
    Texture2D bg = LoadTexture("assets/bg.png");
    Texture2D hole = LoadTexture("assets/black-hole.png");
    Texture2D controls = LoadTexture("assets/howtoplay.png");
    Texture2D idle = LoadTexture("assets/idle.png");
    Texture2D apple = LoadTexture("assets/other/apple.png");
    Texture2D heart = LoadTexture("assets/other/heart.png");
    Texture2D logo = LoadTexture("assets/pacman-logo.png");
    Texture2D play = LoadTexture("assets/play-button.png");
    Texture2D hudai = LoadTexture("assets/other/hudai.png");
    Texture2D blinky = LoadTexture("assets/ghosts/blinky.png");
    Texture2D pinky = LoadTexture("assets/ghosts/pinky.png");
    Texture2D inky = LoadTexture("assets/ghosts/inky.png");
    Texture2D clyde = LoadTexture("assets/ghosts/clyde.png");
    Texture2D blueghost = LoadTexture("assets/ghosts/blue_ghost.png");
    Texture2D jerry = LoadTexture("assets/theme1/jerry.png");
    Texture2D bob = LoadTexture("assets/theme1/bob.png");

    for (int i = 0; i < 3; i++)
    {
        pac_left[i] = LoadTexture(TextFormat("assets/pacman-left/%d.png", i + 1));
        pac_right[i] = LoadTexture(TextFormat("assets/pacman-right/%d.png", i + 1));
        pac_up[i] = LoadTexture(TextFormat("assets/pacman-up/%d.png", i + 1));
        pac_down[i] = LoadTexture(TextFormat("assets/pacman-down/%d.png", i + 1));
    }
    for (int i = 0; i < 4; i++)
        tom_ghost[i] = LoadTexture(TextFormat("assets/theme1/tomghost%d.png", i + 1));

    // sound loading
    Sound chomp = LoadSound("assets/audio/pacman_chomp.wav");
    Sound eatfruit = LoadSound("assets/audio/pacman_eatfruit.wav");
    Music starting = LoadMusicStream("assets/audio/starting.mp3");
    Music ghost_siren = LoadMusicStream("assets/audio/ghost_siren.mp3");
    ghost_siren.looping = true;
    Music eating_ghost = LoadMusicStream("assets/audio/eating_ghost.mp3");
    eating_ghost.looping = true;

    int point = 0;
    int life = 3;
    float coeff = 0.8;
    bool mode = false;
    bool skin = false;
    bool menu = true;
    bool isalive = true;
    bool howtoplay = false;

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
        "     #.##d  dddd  d##.#     ", // 11
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
        "#d.d##d..d..d  d..d..d##d.d#", // 23
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
    Vector2 neg_pac_pos = {0, 0};
    char *pacmove = "null";

    // blinky
    Vector2 blinky_pos = {13 * 25 + space, 14 * 25 + space - 15};
    Vector2 blinky_target;
    Vector2 blinky_speed = {0, -speed * coeff};
    char *blinkymove = "up";
    float blinky_skatter = 12;

    // pinky
    Vector2 pinky_pos = {14 * 25 + space, 14 * 25 + space - 15};
    Vector2 pinky_target;
    Vector2 pinky_speed = {0, -speed * coeff};
    char *pinkymove = "up";
    float pinky_skatter = 22;

    // inky
    Vector2 inky_pos = {13 * 25 + space, 15 * 25 + space - 5};
    Vector2 inky_target;
    Vector2 inky_speed = {0, -speed * coeff};
    char *inkymove = "up";
    float inky_skatter = 32;

    // clyde
    Vector2 clyde_pos = {14 * 25 + space, 15 * 25 + space - 5};
    Vector2 clyde_target;
    Vector2 clyde_speed = {0, -speed * coeff};
    char *clydemove = "up";

    int dots = 240;
    int bigdots = 4;
    float appletime;
    float countdown;

    // invincible mode
    bool invincible_mode = false;
    float invincible_time = 0;

    // High Score
    FILE *faah = fopen("record.txt", "r+");
    char strscore[20];
    int hscore = 0;
    if (faah)
    {
        fgets(strscore, 20, faah);
        for (int i = 0; strscore[i] != '\n'; i++)
            hscore = hscore * 10 + (int)(strscore[i] - '0');
    }

    // sound management
    bool play_starting_sound = true;
    bool checking_starting_sound = false;
    bool play_ghost_siren = false;
    bool play_eating_ghost = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        float dt = GetFrameTime();
        int curr = GetTime() * 15;
        float currenttime = GetTime();
        float alpha = (sinf(curr / 2) + 1.0f) / 2.0f;

        if (howtoplay)
        {
            DrawTexturePro(controls, (Rectangle){0, 0, controls.width, controls.height}, (Rectangle){0, -50, width, height}, origin, 0, WHITE);

            // Return button
            Rectangle returnrec = {width / 2 - 100, height - 1.5 * space, 200, 50};
            DrawRectangleRoundedLines(returnrec, 3, 10, YELLOW);
            DrawText("Return", width / 2 - 70, height - 1.5 * space + 5, 40, YELLOW);
            if (CheckCollisionPointRec(GetMousePosition(), returnrec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                howtoplay = false;
            }

            EndDrawing();
            continue;
        }

        if (menu)
        {
            DrawTexturePro(logo, (Rectangle){0, 0, logo.width, logo.height}, (Rectangle){space, space, col * 25, 200}, origin, 0, WHITE);
            DrawTexturePro(hudai, (Rectangle){0, 0, hudai.width, hudai.height}, (Rectangle){space / 4, height - 4 * space, 300, 300}, origin, 0, WHITE);

            // Mode Selection
            DrawText("MODE", 2 * space, height - 5.5 * space, 50, BLUE);
            if (mode)
            {
                DrawText("Wormhole", 2 * space + 2, height - 5 * space + 10, 30, PINK);
                DrawText("<", 2 * space - 30, height - 5 * space, 50, PINK);
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){2 * space - 30, height - 5 * space, 150, 50}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    mode = !mode;
            }
            else
            {
                DrawText("Classic", 2 * space + 20, height - 5 * space + 10, 30, PINK);
                DrawText(">", 3.6 * space, height - 5 * space, 50, PINK);
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){2 * space + 50, height - 5 * space, 150, 50}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    mode = !mode;
            }

            // Theme Selection
            DrawText("THEME", width - 4 * space, height - 5.5 * space, 50, BLUE);
            if (skin)
            {
                DrawText("Tom & Jerry", width - 4 * space - 5, height - 5 * space + 10, 30, PINK);
                DrawText("<", width - 4 * space - 35, height - 5 * space, 50, PINK);
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){width - 4 * space - 35, height - 5 * space, 150, 50}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    skin = !skin;
            }
            else
            {
                DrawText("Classic", width - 4 * space + 45, height - 5 * space + 10, 30, PINK);
                DrawText(">", width - 2 * space, height - 5 * space, 50, PINK);
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){width - 3 * space - 20, height - 5 * space, 150, 50}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    skin = !skin;
            }

            // Play Button
            Rectangle button = {width / 2 - 125, height - 3.5 * space, 250, 100};
            DrawTexturePro(play, (Rectangle){0, 0, play.width, play.height}, button, origin, 0, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                menu = false;
                countdown = currenttime;
            }

            // How to play Button
            Rectangle howtoplayrec = {width / 2 - 150, height - 1.5 * space, 300, 50};
            DrawRectangleRoundedLines(howtoplayrec, 3, 10, YELLOW);
            DrawText("HOW TO PLAY", width / 2 - 107, height - 1.5 * space + 10, 30, YELLOW);
            if (CheckCollisionPointRec(GetMousePosition(), howtoplayrec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                howtoplay = true;
            }

            EndDrawing();
            continue;
        }

        if (!isalive)
        {
            DrawText("Game Over", 2 * space, 1.5 * space, 100, RED);

            DrawText(TextFormat("Your Score: %d", point), space, height - 5.5 * space, 70, RAYWHITE);
            if (hscore <= point)
            {
                DrawText("New Highest Score", space, height - 4.5 * space, 70, (Color){36, 249, 118, (alpha / 2 + 0.5) * 255});
                faah = freopen("record.txt", "w", faah);
                fputs(TextFormat("%d\n", point), faah);
            }
            else
            {
                DrawText(TextFormat("Highest Score: %d", hscore), space, height - 4.5 * space, 60, (Color){255, 99, 71, (alpha / 2 + 0.5) * 255});
            }
            if (faah)
                fclose(faah);

            // Menu Button
            Rectangle menurec = {width / 2 - 300, height - 2.5 * space, 150, 50};
            DrawRectangleRoundedLines(menurec, 3, 10, GREEN);
            DrawText("Menu", width / 2 - 275, height - 2.5 * space + 5, 40, GREEN);
            if (CheckCollisionPointRec(GetMousePosition(), menurec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                isalive = true;
                menu = true;
                life = 3;
                point = 0;
                goto restart;
            }

            // Restart Button
            Rectangle restartrec = {width / 2 - 75, height - 2.5 * space, 150, 50};
            DrawRectangleRoundedLines(restartrec, 3, 10, YELLOW);
            DrawText("Restart", width / 2 - 60, height - 2.5 * space + 10, 30, YELLOW);
            if (CheckCollisionPointRec(GetMousePosition(), restartrec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                isalive = true;
                countdown = currenttime;
                life = 3;
                point = 0;
                goto restart;
            }

            // Exit Button
            Rectangle exitrec = {width / 2 + 150, height - 2.5 * space, 150, 50};
            DrawRectangleRoundedLines(exitrec, 3, 10, RED);
            DrawText("Exit", width / 2 + 190, height - 2.5 * space + 5, 40, RED);
            if (CheckCollisionPointRec(GetMousePosition(), exitrec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                EndDrawing();
                break;
            }

            EndDrawing();
            continue;
        }

        if (hscore < point)
            hscore = point;

        DrawTexturePro(bg, (Rectangle){0, 0, bg.width, bg.height}, (Rectangle){space, space - 20, col * 25, row * 25 + 15}, origin, 0, WHITE);
        DrawText(TextFormat("Score: %d", point), space, space - 50, 50, RAYWHITE);
        if (faah)
            DrawText(TextFormat("Highest Score: %d", hscore), space, height - 50, 50, RAYWHITE);
        for (int i = 1; i <= life; i++)
            DrawTexturePro(heart, (Rectangle){0, 0, heart.width, heart.height}, (Rectangle){width - space - 60 * i, space - 50, 50, 50}, origin, 0, WHITE);

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
                if (mode && (i == 1 || i == 29) && (j == 1 || j == 26))
                    DrawTexturePro(hole, (Rectangle){0, 0, hole.width, hole.height}, (Rectangle){space + j * 25 - 5, space + i * 25 - 5, 35, 35}, origin, 0, WHITE);
                if (maze[i][j] == '.')
                    DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 3, RAYWHITE);
                if (maze[i][j] == 'o')
                    DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 10, (Color){255, 0, 0, alpha * 255});
                if (maze[i][j] == '-')
                    DrawRectangle(j * 25 + space, i * 25 + 18 + space, 25, 2, RAYWHITE);
            }
        }

        // invincible mode
        if (currenttime - invincible_time > 5)
        {
            invincible_mode = false;
            invincible_time = 0;
        }

        // sound system
        if (!IsMusicStreamPlaying(starting))
        {
            if (invincible_mode)
            {
                if (IsMusicStreamPlaying(ghost_siren))
                {
                    StopMusicStream(ghost_siren);
                }
                if (!IsMusicStreamPlaying(eating_ghost))
                {
                    PlayMusicStream(eating_ghost);
                }
            }
            else
            {
                if (IsMusicStreamPlaying(eating_ghost))
                {
                    StopMusicStream(eating_ghost);
                }
                if (!IsMusicStreamPlaying(ghost_siren))
                {
                    PlayMusicStream(ghost_siren);
                }
            }

            if (IsMusicStreamPlaying(ghost_siren))
            {
                UpdateMusicStream(ghost_siren);
            }
            if (IsMusicStreamPlaying(eating_ghost))
            {
                UpdateMusicStream(eating_ghost);
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
        double x, y, moe = dt * speed / 50;
        x = (pac_pos.x - space) / 25;
        y = (pac_pos.y - space) / 25;

        if (x + moe >= round(x) && x - moe <= round(x) && y + moe >= round(y) && y - moe <= round(y))
        {
            // Hole Jumping
            if (mode && ((int)round(x) == 1 || (int)round(x) == 26) && ((int)round(y) == 1 || (int)round(y) == 29))
            {
                if (maze[(int)round(y)][(int)round(x)] == '.')
                {
                    point += 10;
                    dots--;
                    maze[(int)round(y)][(int)round(x)] = ' ';
                    PlaySound(chomp);
                }
                x = 27 - x;
                y = 30 - y;
                pac_pos.x = 25 * x + space;
                pac_pos.y = 25 * y + space;
            }

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
            else if (pacmove == "down" && maze[(int)round(y) + 1][(int)round(x)] != '#' && maze[(int)round(y) + 1][(int)round(x)] != '-')
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
                invincible_mode = true;
                invincible_time = currenttime;
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

        // negative direction
        neg_pac_pos.x = -pac_pos.x;
        neg_pac_pos.y = -pac_pos.y;

        // blinky direction
        if (invincible_mode)
        {
            blinky_target = neg_pac_pos;
        }
        else if (currenttime - blinky_skatter < 3)
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
        if (invincible_mode)
            ghost_direction(maze, decision, blinky_pos, &blinky_speed, &blinkymove, blinky_target, moe * coeff, speed * (coeff - 0.2));
        else
            ghost_direction(maze, decision, blinky_pos, &blinky_speed, &blinkymove, blinky_target, moe * coeff, speed * coeff);

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
        if (invincible_mode)
        {
            pinky_target = neg_pac_pos;
        }
        else if (currenttime - pinky_skatter < 3)
        {
            pinky_target = (Vector2){0, 0};
        }
        else if (currenttime - pinky_skatter < 10)
        {
            pinky_target = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt * 30));
        }
        else
        {
            pinky_skatter = currenttime;
        }
        if (invincible_mode)
            ghost_direction(maze, decision, pinky_pos, &pinky_speed, &pinkymove, pinky_target, moe * coeff, speed * (coeff - 0.2));
        else
            ghost_direction(maze, decision, pinky_pos, &pinky_speed, &pinkymove, pinky_target, moe * coeff, speed * coeff);

        // Position Update
        if ((currenttime - countdown) > 14)
        {
            pinky_pos = Vector2Add(pinky_pos, Vector2Scale(pinky_speed, dt));
            if (pinky_pos.x + 12.5 < space)
                pinky_pos.x += col * 25;
            else if (pinky_pos.x + 12.5 > space + col * 25)
                pinky_pos.x -= col * 25;
        }

        // inky direction
        if (invincible_mode)
        {
            inky_target = neg_pac_pos;
        }
        else if (currenttime - inky_skatter < 3)
        {
            inky_target = (Vector2){width, height};
        }
        else if (currenttime - inky_skatter < 10)
        {
            inky_target = Vector2Add(pac_pos, (Vector2){pac_pos.x - blinky_pos.x, pac_pos.y - blinky_pos.y});
        }
        else
        {
            inky_skatter = currenttime;
        }
        if (invincible_mode)
            ghost_direction(maze, decision, inky_pos, &inky_speed, &inkymove, inky_target, moe * coeff, speed * (coeff - 0.2));
        else
            ghost_direction(maze, decision, inky_pos, &inky_speed, &inkymove, inky_target, moe * coeff, speed * coeff);

        // Position Update
        if ((currenttime - countdown) > 24)
        {
            inky_pos = Vector2Add(inky_pos, Vector2Scale(inky_speed, dt));
            if (inky_pos.x + 12.5 < space)
                inky_pos.x += col * 25;
            else if (inky_pos.x + 12.5 > space + col * 25)
                inky_pos.x -= col * 25;
        }

        // clyde direction
        if ((clyde_pos.x - blinky_pos.x) < 100 && (clyde_pos.x - blinky_pos.x) > -100 && (clyde_pos.y - blinky_pos.y) < 100 && (clyde_pos.y - blinky_pos.y) > -100)
        {
            clyde_target = (Vector2){0, height};
        }
        else if (invincible_mode)
        {
            clyde_target = neg_pac_pos;
        }
        else
        {
            clyde_target = pac_pos;
        }
        if (invincible_mode)
            ghost_direction(maze, decision, clyde_pos, &clyde_speed, &clydemove, clyde_target, moe * coeff, speed * (coeff - 0.2));
        else
            ghost_direction(maze, decision, clyde_pos, &clyde_speed, &clydemove, clyde_target, moe * coeff, speed * coeff);

        // Position Update
        if ((currenttime - countdown) > 34)
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
            if (play_starting_sound)
            {
                PlayMusicStream(starting);
            }
            if (IsMusicStreamPlaying(starting))
            {
                checking_starting_sound = true;
            }
            if (IsMusicStreamPlaying(starting))
            {
                UpdateMusicStream(starting);
            }
            if (!IsMusicStreamPlaying(starting) && checking_starting_sound)
            {
                play_starting_sound = false;
            }

            int cd = 3 - (int)(currenttime - countdown);
            if (cd)
                DrawText(TextFormat("%d", cd), width / 2 - 10, 17 * 25 + space - 10, 50, YELLOW);
            else
                DrawText("READY", width / 2 - 65, 17 * 25 + space - 5, 40, YELLOW);
            pac_speed = (Vector2){0, 0};
            blinky_skatter = currenttime;
            pinky_skatter = currenttime + 10;
            inky_skatter = currenttime + 20;
        }
        
        Color tint = WHITE;
        if (currenttime - invincible_time > 3.75)
            tint = (Color){255, 255, 255, (int)(alpha * 256 * 4) % 256};

        // Draw PacMan
        Rectangle pacpac = {pac_pos.x - 5, pac_pos.y - 5, 35, 35};
        if (skin && !invincible_mode)
            DrawTexturePro(jerry, (Rectangle){0, 0, jerry.width, jerry.height}, pacpac, origin, 0, WHITE);
        else if (skin)
            DrawTexturePro(bob, (Rectangle){0, 0, bob.width, bob.height}, pacpac, origin, 0, tint);
        else if (pac_speed.x < 0 && pac_speed.y == 0)
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
        if (skin)
            DrawTexturePro(tom_ghost[0], (Rectangle){0, 0, tom_ghost[0].width, tom_ghost[0].height}, blinkyblinky, origin, 0, WHITE);
        else if (invincible_mode)
            DrawTexturePro(blueghost, (Rectangle){0, 0, blinky.width, blinky.height}, blinkyblinky, origin, 0, tint);
        else
            DrawTexturePro(blinky, (Rectangle){0, 0, blinky.width, blinky.height}, blinkyblinky, origin, 0, WHITE);

        // pinky
        Rectangle pinkypinky = {pinky_pos.x - 5, pinky_pos.y - 5, 35, 35};
        if (skin)
            DrawTexturePro(tom_ghost[1], (Rectangle){0, 0, tom_ghost[1].width, tom_ghost[1].height}, pinkypinky, origin, 0, WHITE);
        else if (invincible_mode)
            DrawTexturePro(blueghost, (Rectangle){0, 0, pinky.width, pinky.height}, pinkypinky, origin, 0, tint);
        else
            DrawTexturePro(pinky, (Rectangle){0, 0, pinky.width, pinky.height}, pinkypinky, origin, 0, WHITE);

        // inky
        Rectangle inkyinky = {inky_pos.x - 5, inky_pos.y - 5, 35, 35};
        if (skin)
            DrawTexturePro(tom_ghost[2], (Rectangle){0, 0, tom_ghost[2].width, tom_ghost[2].height}, inkyinky, origin, 0, WHITE);
        else if (invincible_mode)
            DrawTexturePro(blueghost, (Rectangle){0, 0, inky.width, inky.height}, inkyinky, origin, 0, tint);
        else
            DrawTexturePro(inky, (Rectangle){0, 0, inky.width, inky.height}, inkyinky, origin, 0, WHITE);

        // clyde
        Rectangle clydeclyde = {clyde_pos.x - 5, clyde_pos.y - 5, 35, 35};
        if (skin)
            DrawTexturePro(tom_ghost[3], (Rectangle){0, 0, tom_ghost[3].width, tom_ghost[3].height}, clydeclyde, origin, 0, WHITE);
        else if (invincible_mode)
            DrawTexturePro(blueghost, (Rectangle){0, 0, clyde.width, clyde.height}, clydeclyde, origin, 0, tint);
        else
            DrawTexturePro(clyde, (Rectangle){0, 0, clyde.width, clyde.height}, clydeclyde, origin, 0, WHITE);

        if (currenttime - countdown > 15 && CheckCollisionRecs(blinkyblinky, pinkypinky))
            ghost_bounce(&blinky_speed, &pinky_speed, &blinkymove, &pinkymove);

        if (currenttime - countdown > 25 && CheckCollisionRecs(blinkyblinky, inkyinky))
            ghost_bounce(&blinky_speed, &inky_speed, &blinkymove, &inkymove);

        if (currenttime - countdown > 25 && CheckCollisionRecs(pinkypinky, inkyinky))
            ghost_bounce(&pinky_speed, &inky_speed, &pinkymove, &inkymove);

        if (currenttime - countdown > 35 && CheckCollisionRecs(blinkyblinky, clydeclyde))
            ghost_bounce(&blinky_speed, &clyde_speed, &blinkymove, &clydemove);

        if (currenttime - countdown > 35 && CheckCollisionRecs(clydeclyde, pinkypinky))
            ghost_bounce(&clyde_speed, &pinky_speed, &clydemove, &pinkymove);

        if (currenttime - countdown > 35 && CheckCollisionRecs(clydeclyde, inkyinky))
            ghost_bounce(&clyde_speed, &inky_speed, &clydemove, &inkymove);

        if ((CheckCollisionRecs(pacpac, blinkyblinky) || CheckCollisionRecs(pacpac, pinkypinky) || CheckCollisionRecs(pacpac, inkyinky) || CheckCollisionRecs(pacpac, clydeclyde)) && !invincible_mode)
        {
            life--;
            countdown = currenttime;
            blinky_pos = (Vector2){13 * 25 + space, 14 * 25 + space - 15};
            pinky_pos = (Vector2){14 * 25 + space, 14 * 25 + space - 15};
            inky_pos = (Vector2){13 * 25 + space, 15 * 25 + space - 5};
            clyde_pos = (Vector2){14 * 25 + space, 15 * 25 + space - 5};
            pac_pos = (Vector2){13 * 25 + space, 23 * 25 + space};
            pacmove = "null";
            blinkymove = "up";
            pinkymove = "up";
            inkymove = "up";
            clydemove = "up";
            blinky_speed = (Vector2){0, -speed * coeff};
            pinky_speed = (Vector2){0, -speed * coeff};
            inky_speed = (Vector2){0, -speed * coeff};
            clyde_speed = (Vector2){0, -speed * coeff};
            pac_speed = (Vector2){0, 0};
            appletime = currenttime - 8;
        }
        else if (invincible_mode)
        {
            if (CheckCollisionRecs(pacpac, blinkyblinky))
            {
                PlaySound(eatfruit);
                blinky_pos = (Vector2){13 * 25 + space, 14 * 25 + space - 15};
                blinkymove = "up";
                blinky_speed = (Vector2){0, -speed * 0.3};
                point += 200;
            }
            else if (CheckCollisionRecs(pacpac, pinkypinky))
            {
                PlaySound(eatfruit);
                pinky_pos = (Vector2){14 * 25 + space, 14 * 25 + space - 15};
                pinkymove = "up";
                pinky_speed = (Vector2){0, -speed * 0.3};
                point += 200;
            }
            else if (CheckCollisionRecs(pacpac, inkyinky))
            {
                PlaySound(eatfruit);
                inky_pos = (Vector2){13 * 25 + space, 15 * 25 + space - 5};
                inkymove = "up";
                inky_speed = (Vector2){0, -speed * 0.3};
                point += 200;
            }
            else if (CheckCollisionRecs(pacpac, clydeclyde))
            {
                PlaySound(eatfruit);
                clyde_pos = (Vector2){14 * 25 + space, 15 * 25 + space - 5};
                clydemove = "up";
                clyde_speed = (Vector2){0, -speed * 0.3};
                point += 200;
            }
        }

        EndDrawing();

        if (life == 0)
        {
            isalive = false;
        }
        if (dots == 0 && bigdots == 0)
        {
            if (coeff < 1)
                coeff += 0.05;
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

    for (int i = 0; i < 4; i++)
        UnloadTexture(tom_ghost[i]);

    UnloadTexture(bg);
    UnloadTexture(hole);
    UnloadTexture(controls);
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
    UnloadTexture(blueghost);
    UnloadTexture(jerry);
    UnloadTexture(bob);

    // Unload Sound
    UnloadSound(chomp);
    UnloadSound(eatfruit);
    UnloadMusicStream(starting);
    UnloadMusicStream(ghost_siren);
    UnloadMusicStream(eating_ghost);

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
            if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-' && target.y > pos.y)
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
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-')
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
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-' && target.y > pos.y)
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
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-')
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
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-' && target.y > pos.y)
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
            else if (maze[(int)round(gy) + 1][(int)round(gx)] != '#' && maze[(int)round(gy) + 1][(int)round(gx)] != '-')
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

void ghost_bounce(Vector2 *g1_speed, Vector2 *g2_speed, char **g1move, char **g2move)
{
    *g1_speed = Vector2Scale(*g1_speed, -1);
    *g2_speed = Vector2Scale(*g2_speed, -1);

    if (*g1move == "up")
        *g1move = "down";
    else if (*g1move == "down")
        *g1move = "up";
    else if (*g1move == "left")
        *g1move = "right";
    else
        *g1move = "left";

    if (*g2move == "up")
        *g2move = "down";
    else if (*g2move == "down")
        *g2move = "up";
    else if (*g2move == "left")
        *g2move = "right";
    else
        *g2move = "left";
}

/*
Credits:
    https://similarpng.com/pac-mans-energetic-chase-a-3d-arcade-adventure/
    https://www.wired.com/story/what-black-holes-explained/
*/
