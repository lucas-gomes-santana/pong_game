#include "raylib.h"

// OBS: X = posição horizontal, Y = posição vertical

int main(void) {

  const int screenWidth = 800;
  const int screenHeight = 500;

  InitWindow(screenWidth, screenHeight, "Pong Game");

  Vector2 rectanglePosition = {400, 250};

  SetTargetFPS(60);

  // Roda o jogo até que seja fechado
  while (!WindowShouldClose()) {

    rectanglePosition = GetMousePosition();

    // Desenha a tela e outros componentes a partir daqui
    BeginDrawing();

    DrawRectangleV(rectanglePosition, (Vector2){15, 60}, RED);

    ClearBackground(WHITE); // Cor de fundo

    // Termina o desenho da tela e componentes
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
