#include "Fate_Book.h"
#include "Login_Page.h"
#include "Main_Screen.h"

int main() {
    InitWindow(420, 800, "Fate_Book");
    SetTargetFPS(60);

    Fate_Book fateBook;
    fateBook.Initialize();
    LoginPage loginPage(fateBook);
    MainApp* mainApp = nullptr;

    while (!WindowShouldClose()) {
        if (!loginPage.IsDone()) {
            loginPage.Update();

            BeginDrawing();
            loginPage.Draw();
            EndDrawing();
        }
        else {
            if (!mainApp) {
                mainApp = new MainApp(fateBook, loginPage.GetUser());
            }

            mainApp->Update();

            BeginDrawing();
            mainApp->Draw();
            EndDrawing();
        }
    }
    if (mainApp) delete mainApp;
    CloseWindow();
    return 0;
}