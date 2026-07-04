#include "raylib.h"
#include <math.h>
#include <stdbool.h>

// OBS: X = posição horizontal, Y = posição vertical

typedef enum { SCREEN_MENU, SCREEN_DIFFICULTY, SCREEN_GAMEPLAY } GameScreen;

typedef enum { DIFF_EASY, DIFF_MEDIUM, DIFF_HARD } Difficulty;

bool ButtonPressed(Rectangle bounds) {
  bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
  return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// Função personalizada de botão pois o raylib não possui uma pronta
void DrawButton(Rectangle bounds, const char *text, int fontSize,
                Color baseColor, Color textColor) {
  bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
  Color fill = hovered ? Fade(baseColor, 0.8f) : baseColor;

  DrawRectangleRec(bounds, fill);
  DrawRectangleLinesEx(bounds, 2, DARKGRAY);

  int textWidth = MeasureText(text, fontSize);
  int textX = bounds.x + (bounds.width - textWidth) / 2;
  int textY = bounds.y + (bounds.height - fontSize) / 2;
  DrawText(text, textX, textY, fontSize, textColor);
}

int main(void) {

  const int screenWidth = 800;
  const int screenHeight = 500;

  SetConfigFlags(FLAG_MSAA_4X_HINT); // Aplica Anti-aliasing
  InitWindow(screenWidth, screenHeight, "Pong Game");

  GameScreen currentScreen = SCREEN_MENU;

  Vector2 rectanglePosition = {50, 250};
  Vector2 rectangle2Position = {750, 250};
  Vector2 ballPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  Vector2 ballSpeed = {4.0f, 4.0f};
  int ballRadius = 10;
  bool pause = 0;
  int framesCounter = 0;
  int playerScore = 0;
  int botScore = 0;

  float ballBaseSpeed = 4.0f;
  float botSpeed = 5.0f;
  float speedIncrease = 1.1f; // 10% de aumento de velocidade por rebatida
  float maxSpeed = 7.0f;

  SetExitKey(KEY_NULL);
  bool exitWindowRequested = false;
  bool exitWindow = false;

  SetTargetFPS(60);

  while (!exitWindow) {

    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
      exitWindowRequested = true;

    if (exitWindowRequested) {
      if (IsKeyPressed(KEY_Y))
        exitWindow = true;
      else if (IsKeyPressed(KEY_N))
        exitWindowRequested = false;
    }

    switch (currentScreen) {

    case SCREEN_MENU: {
      Rectangle startBtn = {(screenWidth - 220) / 2.0f, 220, 220, 60};
      Rectangle exitBtn = {(screenWidth - 220) / 2.0f, 310, 220, 60};

      if (!exitWindowRequested) {
        if (ButtonPressed(startBtn))
          currentScreen = SCREEN_DIFFICULTY;

        if (ButtonPressed(exitBtn))
          exitWindowRequested = true;
      }

      BeginDrawing();
      ClearBackground(WHITE);

      const char *title = "PONG";
      int titleSize = 60;
      int titleWidth = MeasureText(title, titleSize);
      DrawText(title, (screenWidth - titleWidth) / 2, 90, titleSize, MAROON);

      DrawButton(startBtn, "START GAME", 20, LIGHTGRAY, BLACK);
      DrawButton(exitBtn, "EXIT", 20, LIGHTGRAY, BLACK);

      if (exitWindowRequested) {
        DrawRectangle(0, 100, screenWidth, 200, BLACK);
        DrawText("DO YOU WANT TO QUIT? [Y/N]", 40, 180, 30, WHITE);
      }

      EndDrawing();
    } break;

    case SCREEN_DIFFICULTY: {
      Rectangle easyBtn = {(screenWidth - 220) / 2.0f, 150, 220, 60};
      Rectangle mediumBtn = {(screenWidth - 220) / 2.0f, 230, 220, 60};
      Rectangle hardBtn = {(screenWidth - 220) / 2.0f, 310, 220, 60};

      if (!exitWindowRequested) {
        Difficulty chosen = -1;

        if (ButtonPressed(easyBtn))
          chosen = DIFF_EASY;
        else if (ButtonPressed(mediumBtn))
          chosen = DIFF_MEDIUM;
        else if (ButtonPressed(hardBtn))
          chosen = DIFF_HARD;

        if (chosen != -1) {
          switch (chosen) {
          case DIFF_EASY:
            ballBaseSpeed = 3.0f;
            botSpeed = 3.5f;
            speedIncrease = 1.05f;
            maxSpeed = 6.0f;
            break;
          case DIFF_MEDIUM:
            ballBaseSpeed = 4.0f;
            botSpeed = 5.0f;
            speedIncrease = 1.10f;
            maxSpeed = 7.0f;
            break;
          case DIFF_HARD:
            ballBaseSpeed = 5.5f;
            botSpeed = 6.5f;
            speedIncrease = 1.18f;
            maxSpeed = 9.0f;
            break;
          }

          // Reseta o estado da partida com os novos parâmetros
          rectanglePosition = (Vector2){50, 250};
          rectangle2Position = (Vector2){750, 250};
          ballPosition =
              (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
          ballSpeed = (Vector2){ballBaseSpeed, ballBaseSpeed};
          playerScore = 0;
          botScore = 0;
          pause = false;
          framesCounter = 0;

          currentScreen = SCREEN_GAMEPLAY;
        }
      }

      BeginDrawing();
      ClearBackground(WHITE);

      const char *title = "CHOOSE DIFFICULTY";
      int titleSize = 30;
      int titleWidth = MeasureText(title, titleSize);
      DrawText(title, (screenWidth - titleWidth) / 2, 70, titleSize, DARKGRAY);

      DrawButton(easyBtn, "EASY", 20, GREEN, BLACK);
      DrawButton(mediumBtn, "MEDIUM", 20, ORANGE, BLACK);
      DrawButton(hardBtn, "HARD", 20, RED, BLACK);

      if (exitWindowRequested) {
        DrawRectangle(0, 100, screenWidth, 200, BLACK);
        DrawText("DO YOU WANT TO QUIT? [Y/N]", 40, 180, 30, WHITE);
      }

      EndDrawing();
    } break;

    case SCREEN_GAMEPLAY: {
      Rectangle retangulo = {rectanglePosition.x, rectanglePosition.y, 15,
                             60}; // Jogador
      Rectangle retangulo2 = {rectangle2Position.x, rectangle2Position.y, 15,
                              60}; // Bot

      if (IsKeyPressed(KEY_SPACE))
        pause = !pause;

      if (!pause && !exitWindowRequested) {
        // Movimento do retângulo do jogador travado na direção vertical
        rectanglePosition.y = GetMousePosition().y;

        // Aplica velocidade na bola
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        // Adiciona pontuação e reinicia a bola para o centro após sair da tela
        if (ballPosition.x <= ballRadius) {
          botScore++;
          ballPosition.x = GetScreenWidth() / 2.0f;
          ballPosition.y = GetScreenHeight() / 2.0f;
          ballSpeed.x = ballBaseSpeed;
          ballSpeed.y = ballBaseSpeed;
        }

        if (ballPosition.x >= (GetScreenWidth() - ballRadius)) {
          playerScore++;
          ballPosition.x = GetScreenWidth() / 2.0f;
          ballPosition.y = GetScreenHeight() / 2.0f;
          ballSpeed.x = ballBaseSpeed;
          ballSpeed.y = ballBaseSpeed;
        }

        // Colisão da bola em cima e baixo da tela
        if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) ||
            (ballPosition.y <= ballRadius))
          ballSpeed.y *= -1.0f;

        // Aplica a colisão da bola com os retângulos e aumenta a velocidade
        // dela
        if (CheckCollisionCircleRec(ballPosition, (float)ballRadius,
                                    retangulo)) {
          ballSpeed.x *= -1.0f;
          if (fabsf(ballSpeed.x) < maxSpeed) {
            ballSpeed.x *= speedIncrease;
            ballSpeed.y *= speedIncrease;
          }
        }

        if (CheckCollisionCircleRec(ballPosition, (float)ballRadius,
                                    retangulo2)) {
          ballSpeed.x *= -1.0f;
          if (fabsf(ballSpeed.x) < maxSpeed) {
            ballSpeed.x *= speedIncrease;
            ballSpeed.y *= speedIncrease;
          }
        }

        // Lógica de movimento do bot para seguir a bola
        if (ballPosition.y < rectangle2Position.y)
          rectangle2Position.y -= botSpeed;
        else if (ballPosition.y > rectangle2Position.y)
          rectangle2Position.y += botSpeed;

        // Impede que o bot saia da tela
        if (rectangle2Position.y < 0)
          rectangle2Position.y = 0;
        if (rectangle2Position.y > GetScreenHeight() - 60)
          rectangle2Position.y = GetScreenHeight() - 60;
      }

      if (!exitWindowRequested)
        framesCounter++;

      BeginDrawing();

      ClearBackground(WHITE);

      DrawRectangleV(rectanglePosition, (Vector2){15, 60}, RED);
      DrawRectangleV(rectangle2Position, (Vector2){15, 60}, BLUE);
      DrawCircleV(ballPosition, (float)ballRadius, MAROON);

      DrawText(TextFormat("%i", playerScore), 100, 20, 40, RED);
      DrawText(TextFormat("%i", botScore), 700, 20, 40, BLUE);
      DrawText("PRESS ESCAPE TO EXIT", 10, GetScreenHeight() - 45, 17,
               LIGHTGRAY);
      DrawText("PRESS SPACE TO PAUSE THE GAME", 10, GetScreenHeight() - 25, 17,
               LIGHTGRAY);

      // Mensagem de pausa que pisca a cada 0.5 segundos
      if (pause && ((framesCounter / 30) % 2))
        DrawText("PAUSED", 350, 200, 30, GRAY);

      DrawFPS(10, 10);

      if (exitWindowRequested) {
        DrawRectangle(0, 100, screenWidth, 200, BLACK);
        DrawText("DO YOU WANT TO QUIT? [Y/N]", 40, 180, 30, WHITE);
      }

      EndDrawing();
    } break;
    }
  }

  CloseWindow();

  return 0;
}
