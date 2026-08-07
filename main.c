#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <math.h>

#define row 31
#define col 28
#define space 100
#define speed 150


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
  InitWindow(col * 25 + 2 * space, row * 25 + 2 * space, "PacMan");
  SetTargetFPS(60);

  // Texture Loading
  Texture2D pac_left[3], pac_right[3], pac_up[3], pac_down[3],bg;
  bg=LoadTexture("assets/bg.png");
  for (int i = 0; i < 3; i++)
  {
    pac_left[i] = LoadTexture(TextFormat("assets/pacman-left/%d.png", i + 1));
    pac_right[i] = LoadTexture(TextFormat("assets/pacman-right/%d.png", i + 1));
    pac_up[i] = LoadTexture(TextFormat("assets/pacman-up/%d.png", i + 1));
    pac_down[i] = LoadTexture(TextFormat("assets/pacman-down/%d.png", i + 1));
  }

  // Variables
  const Vector2 origin = {0, 0};
  Vector2 pac_pos = {13 * 25 + space, 23 * 25 + space};
  Vector2 pac_speed = {0, 0};
  // collision with maze
  char *nextmove = "null";

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK);

    float dt = GetFrameTime();
    int curr = GetTime() * 20;

    DrawTexturePro(bg,(Rectangle){0,0,bg.width,bg.height},(Rectangle){space,80,col*25,row*25+15},origin,0,WHITE);

    // Maze
    for (int i = 0; i < row; i++)
    {
      for (int j = 0; j < col; j++)
      {
        if (maze[i][j] == '#')
        {
          Rectangle cell = {j * 25 + space, i * 25 + space, 25, 25};
          DrawRectanglePro(cell, origin, 0, (Color){255,255,255,0});
        }
        if (maze[i][j] == '.')
          DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 3, RAYWHITE);
        if (maze[i][j] == 'o')
          DrawCircle(j * 25 + 12 + space, i * 25 + 12 + space, 10, RED);
      }
    }

    // Direction input
    if (IsKeyPressed(KEY_LEFT))
    {
      nextmove = "left";
    }
    else if (IsKeyPressed(KEY_RIGHT))
    {
      nextmove = "right";
    }
    else if (IsKeyPressed(KEY_UP))
    {
      nextmove = "up";
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
      nextmove = "down";
    }

    // Direction input
    double x, y;
    x = (pac_pos.x - space) / 25;
    y = (pac_pos.y - space) / 25;

    if ((x+0.1>=(int)x && x-0.1<=(int)x) && (y+0.1>=(int)y && y-0.1<=(int)y))
    {
      if (nextmove == "left")
      {
        if (maze[(int)round((pac_pos.y - space) / 25)][(int)round((pac_pos.x - space) / 25) - 1] != '#')
        {
          pac_speed.x = -speed;
          pac_speed.y = 0;
        }else{
          pac_speed.x=0;
        }
      }
      else if (nextmove == "right")
      {

        if (maze[(int)round((pac_pos.y - space) / 25)][(int)round((pac_pos.x - space) / 25) + 1] != '#')
        {
          pac_speed.x = speed;
          pac_speed.y = 0;
        }else{
          pac_speed.x=0;
        }
        
      }
      else if (nextmove == "up")
      {
        if (maze[(int)round((pac_pos.y - space) / 25) - 1][(int)round((pac_pos.x - space) / 25)] != '#')
        {
          pac_speed.x = 0;
          pac_speed.y = -speed;
        }else{
          pac_speed.y=0;
        }
      }
      else if (nextmove == "down")
      {
        if (maze[(int)round((pac_pos.y - space) / 25) + 1][(int)round((pac_pos.x - space) / 25)] != '#')
        {
          pac_speed.x = 0;
          pac_speed.y = speed;
        }else{
          pac_speed.y=0;
        }
      }
    }


    // Position Update
    pac_pos = Vector2Add(pac_pos, Vector2Scale(pac_speed, dt));
    if(pac_pos.x<space){
      pac_pos.x+=col*25;
    }else if(pac_pos.x>space+col*25){
      pac_pos.x-=col*25;
    }
    

    // Draw PacMan
    Rectangle pacpac = {pac_pos.x, pac_pos.y, 25, 25};
    if (pac_speed.x < 0 && pac_speed.y == 0)
      DrawTexturePro(pac_left[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
    else if (pac_speed.x > 0 && pac_speed.y == 0)
      DrawTexturePro(pac_right[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
    else if (pac_speed.x == 0 && pac_speed.y < 0)
      DrawTexturePro(pac_up[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
    else if (pac_speed.x == 0 && pac_speed.y > 0)
      DrawTexturePro(pac_down[curr % 3], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
    else{
      if(nextmove=="up"){
        DrawTexturePro(pac_up[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
      }
      else if(nextmove=="down") DrawTexturePro(pac_down[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
      else if(nextmove=="left") DrawTexturePro(pac_left[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
      else if(nextmove=="right") DrawTexturePro(pac_right[1], (Rectangle){0, 0, pac_left[0].width, pac_left[0].height}, pacpac, origin, 0, WHITE);
   
    }

    EndDrawing();
  }

  // Unload
  for (int i = 0; i < 3; i++)
  {
    UnloadTexture(pac_left[i]);
    UnloadTexture(pac_right[i]);
    UnloadTexture(pac_up[i]);
    UnloadTexture(pac_down[i]);
  }
  UnloadTexture(bg);

  CloseWindow();

  return 0;
}
