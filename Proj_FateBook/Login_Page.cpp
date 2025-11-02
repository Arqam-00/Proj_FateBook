#include "Login_Page.h"
#include <iostream>
using namespace std;

LoginPage::LoginPage(vector<User*>& allUsers)
    : users(allUsers), state(WELCOME), showPassword(false), resultUser(nullptr),
    genderChar('U'), age(0), day(1), month(1), year(2000)
{
    loginEmail[0] = loginPassword[0] = name[0] = email[0] = password[0] = location[0] = '\0';
}

void LoginPage::Load(Font f) {
    font = f;
}

bool LoginPage::IsDone() const {
    return state == DONE && resultUser != nullptr;
}

User* LoginPage::GetUser() const {
    return resultUser;
}

void LoginPage::Update() {
    Vector2 mouse = GetMousePosition();

    static int activeField = 0; // 0=email, 1=password for login, for signup we’ll reuse later

    int key = GetCharPressed();
    if (state == WELCOME) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mouse.y > 250 && mouse.y < 300) {
                state = LOGIN;
                activeField = 0;
            }
            else if (mouse.y > 320 && mouse.y < 370) {
                state = SIGNUP;
                activeField = 0;
            }
        }
    }

    else if (state == LOGIN) {
        // Switch fields with TAB
        if (IsKeyPressed(KEY_TAB)) activeField = (activeField + 1) % 2;

        // Backspace logic
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (activeField == 0 && strlen(loginEmail) > 0)
                loginEmail[strlen(loginEmail) - 1] = '\0';
            else if (activeField == 1 && strlen(loginPassword) > 0)
                loginPassword[strlen(loginPassword) - 1] = '\0';
        }

        // Character input
        while (key > 0) {
            if (activeField == 0 && strlen(loginEmail) < 63 && key >= 32 && key <= 125)
                loginEmail[strlen(loginEmail)] = (char)key, loginEmail[strlen(loginEmail) + 1] = '\0';
            else if (activeField == 1 && strlen(loginPassword) < 63 && key >= 32 && key <= 125)
                loginPassword[strlen(loginPassword)] = (char)key, loginPassword[strlen(loginPassword) + 1] = '\0';
            key = GetCharPressed(); // get next char (Raylib queues multiple)
        }

        // Enter to login
        if (IsKeyPressed(KEY_ENTER)) {
            for (auto& u : users) {
                if (u->GetEmail() == loginEmail && u->CheckPassword(loginPassword)) {
                    resultUser = u;
                    state = DONE;
                    break;
                }
            }
        }
    }

    else if (state == SIGNUP) {
        // Similar typing logic for signup can be added later (we’ll do it step-by-step)
        if (IsKeyPressed(KEY_ENTER)) {
            Date dob(day, month, year);
            if (!dob.IsValid()) return;

            User* newUser = new User("assets/pfps/default.png", name, email, password, location, genderChar, age, true, dob);
            users.push_back(newUser);
            resultUser = newUser;
            state = DONE;
        }
    }
}

void LoginPage::Draw() {
    ClearBackground(RAYWHITE);

    if (state == WELCOME) {
        DrawTextEx(font, "Fate_Book", { 100, 100 }, 40, 2, DARKBLUE);
        DrawRectangle(100, 250, 200, 50, BLUE);
        DrawText("Login", 140, 265, 20, WHITE);
        DrawRectangle(100, 320, 200, 50, DARKGREEN);
        DrawText("Signup", 130, 335, 20, WHITE);
    }

    else if (state == LOGIN) {
        DrawText("Email:", 100, 100, 20, BLACK);
        DrawText(loginEmail, 100, 130, 20, DARKGRAY);

        DrawText("Password:", 100, 180, 20, BLACK);
        string hidden = showPassword ? loginPassword : string(strlen(loginPassword), '*');
        DrawText(hidden.c_str(), 100, 210, 20, DARKGRAY);

        DrawText("Press TAB to switch fields", 100, 250, 20, GRAY);
        DrawText("Press ENTER to login", 100, 270, 20, GRAY);
    }

    else if (state == SIGNUP) {
        DrawText("Signup screen (coming soon typing support)", 100, 100, 20, DARKGRAY);
    }
}
