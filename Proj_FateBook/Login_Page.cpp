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

    if (state == WELCOME) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mouse.y > 250 && mouse.y < 300) state = LOGIN;
            else if (mouse.y > 320 && mouse.y < 370) state = SIGNUP;
        }
    }
    else if (state == LOGIN) {
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
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(loginEmail) > 0) loginEmail[strlen(loginEmail) - 1] = '\0';
        else if (GetCharPressed()) loginEmail[strlen(loginEmail)] = GetCharPressed();

        DrawText("Password:", 100, 180, 20, BLACK);
        string hidden = showPassword ? loginPassword : string(strlen(loginPassword), '*');
        DrawText(hidden.c_str(), 100, 210, 20, DARKGRAY);
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(loginPassword) > 0) loginPassword[strlen(loginPassword) - 1] = '\0';
        else if (GetCharPressed()) loginPassword[strlen(loginPassword)] = GetCharPressed();
        DrawText("Press Enter to Login", 100, 270, 20, GRAY);
    }
    else if (state == SIGNUP) {
        DrawText("Name:", 100, 100, 20, BLACK);
        DrawText(name, 100, 130, 20, DARKGRAY);
        DrawText("Email:", 100, 160, 20, BLACK);
        DrawText(email, 100, 190, 20, DARKGRAY);
        DrawText("Password:", 100, 220, 20, BLACK);
        DrawText(password, 100, 250, 20, DARKGRAY);
        DrawText("Location:", 100, 280, 20, BLACK);
        DrawText(location, 100, 310, 20, DARKGRAY);
        DrawText("Gender (M/F):", 100, 340, 20, BLACK);
        DrawText(&genderChar, 100, 370, 20, DARKGRAY);
        DrawText("Age:", 100, 400, 20, BLACK);
        DrawText(TextFormat("%d", age), 100, 430, 20, DARKGRAY);
        DrawText("Date of Birth (D/M/Y):", 100, 460, 20, BLACK);
        DrawText(TextFormat("%d/%d/%d", day, month, year), 100, 490, 20, DARKGRAY);
        DrawText("Press Enter to Signup", 100, 530, 20, GRAY);
    }
}
