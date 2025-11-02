#include <raylib.h>
#include <vector>
#include "Login_Page.h"
#include "User.h"
#include "Date.h"

int main() {
    // Initialize raylib window
    InitWindow(800, 600, "Login Page Test");
    SetTargetFPS(60);

    // Load font (you can replace this with your own TTF)
    Font font = LoadFont("resources/arial.ttf");

    // Create a few dummy users to test login
    std::vector<User*> users;
    users.push_back(new User("","Abdul Quddos", "quddos@gmail.com", "1234", "Islamabad", 'M', 21,true, Date(15, 8, 2003)));
    users.push_back(new User("","Arqam", "arqam@gmail.com", "abcd", "Lahore", 'M', 22,true, Date(10, 4, 2002)));

    // Create login page
    LoginPage login(users);
    login.Load(font);

    // Main game loop
    while (!WindowShouldClose() && !login.IsDone()) {
        login.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        login.Draw();
        EndDrawing();
    }

    // Once login/signup done
    User* loggedUser = login.GetUser();
    if (loggedUser != nullptr) {
        // Example output in console
        TraceLog(LOG_INFO, TextFormat("Logged in as: %s", loggedUser->Name.c_str()));
    }

    // Cleanup
    for (auto u : users)
        delete u;

    UnloadFont(font);
    CloseWindow();
    return 0;
}
