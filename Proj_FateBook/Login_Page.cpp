#include "Login_Page.h"
#include <iostream>
using namespace std;

LoginPage::LoginPage(Fate_Book& fb)
    : fateBook(fb), state(WELCOME), showPassword(false), resultUser(nullptr),
    genderChar('U'), age(0), day(1), month(1), year(2000), framesCounter(0), activeField(0)
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

void LoginPage::Reset() {
    state = WELCOME;
    resultUser = nullptr;
    activeField = 0;
    framesCounter = 0;
    loginEmail[0] = loginPassword[0] = name[0] = email[0] = password[0] = location[0] = '\0';
    genderChar = 'U';
    age = 0;
    day = 1; month = 1; year = 2000;
}

void LoginPage::HandleLoginInput() {
    framesCounter++;

    if (IsKeyPressed(KEY_TAB)) {
        activeField = (activeField + 1) % 2;
    }

    char* currentField = (activeField == 0) ? loginEmail : loginPassword;
    int maxLen = 255;

    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (strlen(currentField) < maxLen)) {
            int len = strlen(currentField);
            currentField[len] = (char)key;
            currentField[len + 1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        int length = strlen(currentField);
        if (length > 0) {
            currentField[length - 1] = '\0';
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        string emailStr(loginEmail);
        string passwordStr(loginPassword);

        if (!emailStr.empty() && !passwordStr.empty()) {
            resultUser = fateBook.Check(emailStr, passwordStr);
            if (resultUser) {
                state = DONE;
            }
            else {
                cout << "Login failed!" << endl;
                loginEmail[0] = loginPassword[0] = '\0';
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state = WELCOME;
        activeField = 0;
        loginEmail[0] = loginPassword[0] = '\0';
    }
}

void LoginPage::HandleSignupInput() {
    framesCounter++;

    if (IsKeyPressed(KEY_TAB)) {
        activeField = (activeField + 1) % 7;
    }

    char* currentField = nullptr;
    int maxLen = 255;

    switch (activeField) {
    case 0: currentField = name; break;
    case 1: currentField = email; break;
    case 2: currentField = password; break;
    case 3: currentField = location; break;
    }

    if (currentField && activeField <= 3) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (strlen(currentField) < maxLen)) {
                int len = strlen(currentField);
                currentField[len] = (char)key;
                currentField[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int length = strlen(currentField);
            if (length > 0) {
                currentField[length - 1] = '\0';
            }
        }
    }

    if (activeField == 4) {
        if (IsKeyPressed(KEY_UP)) age++;
        if (IsKeyPressed(KEY_DOWN) && age > 0) age--;
    }
    else if (activeField == 5) {
        if (IsKeyPressed(KEY_UP)) day++;
        if (IsKeyPressed(KEY_DOWN) && day > 1) day--;
        if (day > 31) day = 1;
    }
    else if (activeField == 6) {
        if (IsKeyPressed(KEY_UP)) month++;
        if (IsKeyPressed(KEY_DOWN) && month > 1) month--;
        if (month > 12) month = 1;
    }

    if (IsKeyPressed(KEY_M)) genderChar = 'M';
    if (IsKeyPressed(KEY_F)) genderChar = 'F';
    if (IsKeyPressed(KEY_U)) genderChar = 'U';

    if (IsKeyPressed(KEY_ENTER)) {
        string nameStr(name);
        string emailStr(email);
        string passwordStr(password);
        string locationStr(location);

        if (!nameStr.empty() && !emailStr.empty() && !passwordStr.empty()) {
            Date dob(day, month, year);
            if (!dob.IsValid()) {
                cout << "Invalid date!" << endl;
                return;
            }

            if (fateBook.Check_By_Email(emailStr)) {
                cout << "Email exists!" << endl;
                return;
            }

            resultUser = fateBook.CreateUser(nameStr, emailStr, passwordStr,
                locationStr, genderChar, age, true, dob);
            if (resultUser) {
                fateBook.GetUsers()->push_back(resultUser);
                state = DONE;
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state = WELCOME;
        activeField = 0;
        name[0] = email[0] = password[0] = location[0] = '\0';
        genderChar = 'U';
        age = 0;
        day = 1; month = 1; year = 2000;
    }
}

void LoginPage::Update() {
    Vector2 mouse = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (state == WELCOME) {
        if (mousePressed && mouse.x > 80 && mouse.x < 280 &&
            mouse.y > 250 && mouse.y < 300) {
            state = LOGIN;
            activeField = 0;
        }
        else if (mousePressed && mouse.x > 80 && mouse.x < 280 &&
            mouse.y > 320 && mouse.y < 370) {
            state = SIGNUP;
            activeField = 0;
        }
    }
    else if (state == LOGIN) {
        HandleLoginInput();
    }
    else if (state == SIGNUP) {
        HandleSignupInput();
    }
}

void LoginPage::Draw() {
    ClearBackground(RAYWHITE);

    int centerX = 180;
    int screenWidth = 360;

    if (state == WELCOME) {
        DrawText("Fate_Book", centerX - 70, 100, 30, DARKBLUE);

        DrawRectangle(80, 250, 200, 50, BLUE);
        DrawText("Login", centerX - 25, 265, 20, WHITE);

        DrawRectangle(80, 320, 200, 50, DARKGREEN);
        DrawText("Signup", centerX - 30, 335, 20, WHITE);
    }
    else if (state == LOGIN) {
        DrawText("Login", centerX - 30, 50, 25, DARKBLUE);

        Color emailColor = (activeField == 0) ? BLUE : LIGHTGRAY;
        DrawRectangle(30, 100, 300, 30, emailColor);
        DrawText("Email:", 30, 70, 18, BLACK);
        DrawText(loginEmail, 35, 105, 18, DARKGRAY);

        if (activeField == 0 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(loginEmail, 18);
            DrawText("_", 35 + textWidth, 105, 18, BLACK);
        }
        Color passColor = (activeField == 1) ? BLUE : LIGHTGRAY;
        DrawRectangle(30, 170, 300, 30, passColor);
        DrawText("Password:", 30, 140, 18, BLACK);
        string hiddenPassword = string(strlen(loginPassword), '*');
        DrawText(hiddenPassword.c_str(), 35, 175, 18, DARKGRAY);
        if (activeField == 1 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(hiddenPassword.c_str(), 18);
            DrawText("_", 35 + textWidth, 175, 18, BLACK);
        }

        DrawText("Press TAB to switch fields", 30, 220, 16, GRAY);
        DrawText("Press ENTER to login", 30, 240, 16, GRAY);
        DrawText("Press ESC to go back", 30, 260, 16, GRAY);
    }
    else if (state == SIGNUP) {
        DrawText("Sign Up", centerX - 35, 20, 25, DARKBLUE);

        int y = 50;
        string fieldLabels[] = { "Name:", "Email:", "Password:", "Location:", "Age:", "Birth Day:", "Birth Month:" };

        for (int i = 0; i < 4; i++) {
            Color fieldColor = (activeField == i) ? BLUE : LIGHTGRAY;
            DrawRectangle(30, y + 25, 300, 25, fieldColor);
            DrawText(fieldLabels[i].c_str(), 30, y, 16, BLACK);
            y += 50;
        }
        y = 75;
        DrawText(name, 35, y, 16, DARKGRAY);
        if (activeField == 0 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(name, 16);
            DrawText("_", 35 + textWidth, y, 16, BLACK);
        }

        y += 50;
        DrawText(email, 35, y, 16, DARKGRAY);
        if (activeField == 1 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(email, 16);
            DrawText("_", 35 + textWidth, y, 16, BLACK);
        }

        y += 50;
        DrawText(password, 35, y, 16, DARKGRAY);
        if (activeField == 2 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(password, 16);
            DrawText("_", 35 + textWidth, y, 16, BLACK);
        }

        y += 50;
        DrawText(location, 35, y, 16, DARKGRAY);
        if (activeField == 3 && ((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(location, 16);
            DrawText("_", 35 + textWidth, y, 16, BLACK);
        }
        y += 50;
        DrawText(TextFormat("Gender: %c (Press M/F/U)", genderChar), 30, y, 16, BLACK);
        y += 30;
        DrawText(TextFormat("Age: %d (Press UP/DOWN)", age), 30, y, 16, BLACK);
        y += 30;
        DrawText(TextFormat("Birth: %d/%d/%d (Press UP/DOWN)", day, month, year), 30, y, 16, BLACK);
        y += 40;

        DrawText("Press TAB to switch fields", 30, y, 14, GRAY);
        DrawText("Press ENTER to signup", 30, y + 20, 14, GRAY);
        DrawText("Press ESC to go back", 30, y + 40, 14, GRAY);
    }
}