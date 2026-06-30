// File: Viewer.c
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Error: Usage: ./Viewer.out (filename)\n");
    return 1;
  }

  const int T_FPS = 60;
  const int W_WIDTH = 800;
  const int W_HEIGHT = 600;
  const int FONT_SIZE = 20;
  const int TEXT_OFFSET = 10;
  const Color BG_COLOR = (Color) {25, 25, 25, 255};
  const float MOVE_SPEED = 300.0 / T_FPS;

  const float SCALE_MIN = 0.05;
  const float SCALE_DEFAULT = 1.0;
  const float SCALE_MAX = 100.0;
  const float SCALE_VALUES[] =
  {
    SCALE_MIN, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
    SCALE_DEFAULT, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0,
    7.0, 8.0, 9.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 24.0, 28.0, 32.0,
    36.0, 40.0, 48.0, 56.0, 64.0, 72.0, 80.0, 90.0, SCALE_MAX
  };

  char zoom_text[32];
  char rotation_text[32];
  char position_text[64];
  bool show_text = true;

  Vector2 center_pos;
  Vector2 offset_pos = (Vector2) {0.0, 0.0};
  float rotation = 0.0;
  int scale_idx = 0;
  while (SCALE_VALUES[scale_idx] < SCALE_DEFAULT) scale_idx++;

  SetWindowState(FLAG_WINDOW_RESIZABLE);
  InitWindow(W_WIDTH, W_HEIGHT, "Viewer");
  SetTargetFPS(T_FPS);
  Texture2D texture = LoadTexture(argv[1]);

  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_TAB)) show_text = !show_text;

    if (IsKeyPressed(KEY_SPACE))
    {
      offset_pos = (Vector2) {0.0, 0.0};
      rotation = 0.0;
      scale_idx = 0;
      while (SCALE_VALUES[scale_idx] < SCALE_DEFAULT) scale_idx++;
    }

    float scale = SCALE_VALUES[scale_idx];
    if (IsKeyPressed(KEY_MINUS) && scale > SCALE_MIN) scale_idx--;
    if (IsKeyPressed(KEY_EQUAL) && scale < SCALE_MAX) scale_idx++;

    if (IsKeyPressed(KEY_NINE) || IsKeyPressed(KEY_LEFT_BRACKET))
    {
      rotation -= 90.0;
      if (rotation < 0.0) rotation += 360.0;
    }

    if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_RIGHT_BRACKET))
    {
      rotation += 90.0;
      if (rotation >= 360.0) rotation -= 360.0;
    }

    if (rotation < 45.0)
    {
      center_pos = (Vector2)
      {
        (GetScreenWidth() - texture.width * scale) / 2.0,
        (GetScreenHeight() - texture.height * scale) / 2.0
      };
    }
    else if (rotation < 135.0)
    {
      center_pos = (Vector2)
      {
        (GetScreenWidth() + texture.height * scale) / 2.0,
        (GetScreenHeight() - texture.width * scale) / 2.0
      };
    }
    else if (rotation < 225.0)
    {
      center_pos = (Vector2)
      {
        (GetScreenWidth() + texture.width * scale) / 2.0,
        (GetScreenHeight() + texture.height * scale) / 2.0
      };
    }
    else
    {
      center_pos = (Vector2)
      {
        (GetScreenWidth() - texture.height * scale) / 2.0,
        (GetScreenHeight() + texture.width * scale) / 2.0
      };
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      offset_pos.y -= MOVE_SPEED * scale;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      offset_pos.x -= MOVE_SPEED * scale;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      offset_pos.y += MOVE_SPEED * scale;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      offset_pos.x += MOVE_SPEED * scale;

    Vector2 draw_pos = (Vector2)
    {
      center_pos.x + offset_pos.x,
      center_pos.y + offset_pos.y
    };

    sprintf(zoom_text, "Zoom: %d%%", (int) (scale * 100));
    sprintf(rotation_text, "Rotation: %.1f", rotation);
    sprintf(position_text, "(%d, %d)", (int) offset_pos.x, (int) offset_pos.y);

    BeginDrawing();
    ClearBackground(BG_COLOR);
    DrawTextureEx(texture, draw_pos, rotation, scale, WHITE);
    if (show_text)
    {
      DrawText(zoom_text, TEXT_OFFSET, TEXT_OFFSET, FONT_SIZE, WHITE);
      DrawText(rotation_text, TEXT_OFFSET, TEXT_OFFSET * 2 + FONT_SIZE,
        FONT_SIZE, WHITE);
      DrawText(position_text, TEXT_OFFSET, TEXT_OFFSET * 3 + FONT_SIZE * 2,
        FONT_SIZE, WHITE);
    }
    EndDrawing();
  }

  UnloadTexture(texture);
  CloseWindow();
  return 0;
}
