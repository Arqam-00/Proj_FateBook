#include "Fate_Book.h"
#include "Login_Page.h"

int main() {
    InitWindow(360, 640, "Fate_Book");
    SetTargetFPS(60);

    Fate_Book fateBook;
    fateBook.Initialize();

    LoginPage loginPage(fateBook);

    while (!WindowShouldClose()) {
        loginPage.Update();
        BeginDrawing();
        loginPage.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}