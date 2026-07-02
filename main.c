#include "raylib.h"

// OBS: X = posição horizontal, Y = posição vertical

int main(void) {

  const int screenWidth = 800;
  const int screenHeight = 500;

  SetConfigFlags(FLAG_MSAA_4X_HINT); // Anti-aliasing
  InitWindow(screenWidth, screenHeight, "Pong Game");

  Vector2 rectanglePosition = {400, 250};
  Vector2 ballPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  Vector2 ballSpeed = {5.0f, 5.0f};
  int ballRadius = 10;
  bool pause = 0;
  int framesCounter = 0;

  SetTargetFPS(60);

  // Loop principal do jogo
  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_SPACE))
      pause = !pause;

    if (!pause) {
      ballPosition.x += ballSpeed.x;
      ballPosition.y += ballSpeed.y;

      // Verificar colisão com as paredes para a bola quicar
      if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) ||
          (ballPosition.x <= ballRadius))
        ballSpeed.x *= -1.0f;
      if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) ||
          (ballPosition.y <= ballRadius))
        ballSpeed.y *= -0.95f;
    }

    else
      framesCounter++;

    rectanglePosition = GetMousePosition();

    // Desenha a tela e outros componentes a partir daqui
    BeginDrawing();

    ClearBackground(WHITE);

    DrawRectangleV(rectanglePosition, (Vector2){15, 60}, RED);
    DrawCircleV(ballPosition, (float)ballRadius, MAROON);
    DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25,
             20, LIGHTGRAY);

    // Mensagem de pausa que pisca a cada 0.5 segundos
    // Se o resto da divisão for 1, exibe o texto. Caso contrário, não exibe
    if (pause && ((framesCounter / 30) % 2))
      DrawText("PAUSED", 350, 200, 30, GRAY);

    DrawFPS(10, 10); // Exibe o fps no canto superiror esquerdo da tela

    // Termina o desenho da tela e componentes
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
