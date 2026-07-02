#include "raylib.h"

// OBS: X = posição horizontal, Y = posição vertical

int main(void) {

  const int screenWidth = 800;
  const int screenHeight = 500;

  SetConfigFlags(FLAG_MSAA_4X_HINT); // Aplica Anti-aliasing
  InitWindow(screenWidth, screenHeight, "Pong Game");

  Vector2 rectanglePosition = {50, 250};
  Vector2 rectangle2Position = {750, 250};
  Vector2 ballPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  Vector2 ballSpeed = {5.0f, 5.0f};
  int ballRadius = 10;
  bool pause = 0;
  int framesCounter = 0;
  float botSpeed = 4.5f;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    Rectangle retangulo = {rectanglePosition.x, rectanglePosition.y, 15,
                           60}; // Jogador
    Rectangle retangulo2 = {rectangle2Position.x, rectangle2Position.y, 15,
                            60}; // Bot

    if (IsKeyPressed(KEY_SPACE))
      pause = !pause;

    if (!pause) {
      // Movimento do retângulo do jogador travado na direção vertical
      rectanglePosition.y = GetMousePosition().y;

      // Aplica velocidade na bola
      ballPosition.x += ballSpeed.x;
      ballPosition.y += ballSpeed.y;

      // Verificar colisão com as paredes para a bola quicar
      if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) ||
          (ballPosition.x <= ballRadius))
        ballSpeed.x *= -1.0f;
      if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) ||
          (ballPosition.y <= ballRadius))
        ballSpeed.y *= -1.0f;

      // Aplica a colisão da bola com os retângulos
      if (CheckCollisionCircleRec(ballPosition, (float)ballRadius, retangulo))
        ballSpeed.x *= -1.0f;

      if (CheckCollisionCircleRec(ballPosition, (float)ballRadius, retangulo2))
        ballSpeed.x *= -1.0f;

      // Lógica de movimento do bot para seguir a bola
      if (ballPosition.y < rectangle2Position.y)
        rectangle2Position.y -= botSpeed;
      else if (ballPosition.y > rectangle2Position.y)
        rectangle2Position.y += botSpeed;

      // Impede que o bot saida da tela
      if (rectangle2Position.y < 0)
        rectangle2Position.y = 0;
      if (rectangle2Position.y > GetScreenHeight() - 60)
        rectangle2Position.y = GetScreenHeight() - 60;
    }

    else {
      framesCounter++;
    }

    // Desenha a tela e outros componentes a partir daqui
    BeginDrawing();

    ClearBackground(WHITE);

    DrawRectangleV(rectanglePosition, (Vector2){15, 60}, RED);
    DrawRectangleV(rectangle2Position, (Vector2){15, 60}, BLUE);
    DrawCircleV(ballPosition, (float)ballRadius, MAROON);

    DrawText("PRESS SPACE TO PAUSE THE GAME", 10, GetScreenHeight() - 25, 20,
             LIGHTGRAY);

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
