#include "Login_Page.h"
#include <iostream>
using namespace std;

#define FIELD_COUNT 7

LoginPage::LoginPage(Fate_Book& fb)
    : fateBook(fb), state(WELCOME), showPassword(false), resultUser(nullptr),
    genderChar('U'), age(0), day(1), month(1), year(2000), framesCounter(0), activeField(-1)
{
    loginEmail[0] = loginPassword[0] = name[0] = email[0] = password[0] = address[0] = '\0';
}

void LoginPage::Load(Font f) {
    font = f;
}


void LoginPage::Reset() {
    state = WELCOME;
    resultUser = nullptr;
    activeField = -1;
    framesCounter = 0;
    loginEmail[0] = loginPassword[0] = name[0] = email[0] = password[0] = address[0] = '\0';
    genderChar = 'U';
    age = 0;
    day = 1; month = 1; year = 2000;
}

bool LoginPage::AllFieldsFilled() const {
    return strlen(name) > 0 && strlen(email) > 0 && strlen(password) > 0 && strlen(address) > 0 && age > 0;
}

//=======================login-==================
void LoginPage::HandleLoginInput() {
    framesCounter++;

    Vector2 mouse = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Rectangle emailRect = { 30, 150, 360, 40 };
    Rectangle passRect = { 30, 230, 360, 40 };

    if (mousePressed) {
        if (CheckCollisionPointRec(mouse, emailRect)) activeField = 0;
        else if (CheckCollisionPointRec(mouse, passRect)) activeField = 1;
        else activeField = -1;
    }

    char* currentField = (activeField == 0) ? loginEmail : (activeField == 1) ? loginPassword : nullptr;
    if (currentField) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32 && key <= 125) && strlen(currentField) < 254) {
                int len = strlen(currentField);
                currentField[len] = (char)key;
                currentField[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(currentField);
            if (len > 0) currentField[len - 1] = '\0';
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        string emailStr(loginEmail), passStr(loginPassword);
        if (!emailStr.empty() && !passStr.empty()) {
            resultUser = fateBook.Check(emailStr, passStr);
            if (resultUser) state = DONE;
            else {
                cout << "Login failed!" << endl;
                loginEmail[0] = loginPassword[0] = '\0';
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state = WELCOME;
        activeField = -1;
    }
}

//===================signiup--=========================
void LoginPage::HandleSignupInput() {
    framesCounter++;
    Vector2 mouse = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    //============Field rectangles
    Rectangle fieldBoxes[FIELD_COUNT] = {
        { 30, 100, 360, 40 },
        { 30, 180, 360, 40 },
        { 30, 260, 360, 40 },
        { 30, 340, 360, 40 },
        { 30, 420, 360, 40 },
        { 30, 500, 360, 40 },
        { 30, 580, 360, 40 }
    };
    if (mousePressed) {
        activeField = -1;
        for (int i = 0; i < FIELD_COUNT; i++) {
            if (CheckCollisionPointRec(mouse, fieldBoxes[i])) {
                activeField = i;
                break;
            }
        }
    }
    char* currentField = nullptr;
    switch (activeField) {
    case 0: currentField = name; break;
    case 1: currentField = email; break;
    case 2: currentField = password; break;
    case 3: currentField = address; break;
    }
    if (currentField) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32 && key <= 125) && strlen(currentField) < 254) {
                int len = strlen(currentField);
                currentField[len] = (char)key;
                currentField[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(currentField);
            if (len > 0) currentField[len - 1] = '\0';
        }
    }
    if (activeField == 4) {
        if (IsKeyPressed(KEY_UP)) age++;
        if (IsKeyPressed(KEY_DOWN) && age > 0) age--;
    }
    if (activeField == 5) {
        if (IsKeyPressed(KEY_UP)) day++;
        if (IsKeyPressed(KEY_DOWN) && day > 1) day--;
        if (IsKeyPressed(KEY_RIGHT)) month++;
        if (IsKeyPressed(KEY_LEFT) && month > 1) month--;
        if (IsKeyPressed(KEY_PAGE_UP)) year++;
        if (IsKeyPressed(KEY_PAGE_DOWN) && year > 1900) year--;
    }
    if (activeField == 6) {
        if (IsKeyPressed(KEY_M)) genderChar = 'M';
        if (IsKeyPressed(KEY_F)) genderChar = 'F';
        if (IsKeyPressed(KEY_U)) genderChar = 'U';
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (!AllFieldsFilled()) {
            cout << "Please fill all fields!" << endl;
            return;
        }
        Date dob(day, month, year);
        if (!dob.IsValid()) {
            cout << "Invalid date!" << endl;
            return;
        }
        if (fateBook.Check_By_Email(email)) {
            cout << "Email exists!" << endl;
            return;
        }
        resultUser = fateBook.CreateUser(name, email, password, address,
            genderChar, age, true, dob);
        if (resultUser) {
            fateBook.GetUsers()->push_back(resultUser);
            state = DONE;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        state = WELCOME;
        activeField = -1;
    }
}

//========================update--======================
void LoginPage::Update() {
    if (state == WELCOME) {
        Vector2 mouse = GetMousePosition();
        bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        Rectangle loginBtn = { 110, 300, 200, 50 };
        Rectangle signupBtn = { 110, 380, 200, 50 };
        if (click && CheckCollisionPointRec(mouse, loginBtn)) {
            state = LOGIN; activeField = -1;
        }
        else if (click && CheckCollisionPointRec(mouse, signupBtn)) {
            state = SIGNUP; activeField = -1;
        }
    }
    else if (state == LOGIN) HandleLoginInput();
    else if (state == SIGNUP) HandleSignupInput();
}

//==========================================Draw------------------------------
void LoginPage::Draw() {
    ClearBackground(RAYWHITE);
    int centerX = 210;

    if (state == WELCOME) {
        DrawText("Fate_Book", centerX - 90, 120, 40, DARKBLUE);
        DrawRectangle(110, 300, 200, 50, BLUE);
        DrawText("Login", centerX - 40, 315, 25, WHITE);
        DrawRectangle(110, 380, 200, 50, DARKGREEN);
        DrawText("Sign Up", centerX - 55, 395, 25, WHITE);
        return;
    }

    if (state == LOGIN) {
        DrawText("Login", centerX - 35, 50, 30, DARKBLUE);
        DrawText("Email:", 30, 120, 18, BLACK);
        DrawRectangle(30, 150, 360, 40, (activeField == 0) ? BLUE : LIGHTGRAY);
        DrawText(loginEmail, 35, 160, 20, DARKGRAY);
        DrawText("Password:", 30, 200, 18, BLACK);
        DrawRectangle(30, 230, 360, 40, (activeField == 1) ? BLUE : LIGHTGRAY);
        string hidden = string(strlen(loginPassword), '*');
        DrawText(hidden.c_str(), 35, 240, 20, DARKGRAY);
        DrawText("Press ENTER to Login", 30, 300, 16, GRAY);
        DrawText("Press ESC to go back", 30, 320, 16, GRAY);
        return;
    }

    if (state == SIGNUP) {
        DrawText("Sign Up", centerX - 45, 50, 30, DARKBLUE);

        DrawText("Name:", 30, 80, 18, BLACK);
        DrawRectangle(30, 100, 360, 40, (activeField == 0) ? BLUE : LIGHTGRAY);
        DrawText(name, 35, 110, 20, DARKGRAY);

        DrawText("Email:", 30, 160, 18, BLACK);
        DrawRectangle(30, 180, 360, 40, (activeField == 1) ? BLUE : LIGHTGRAY);
        DrawText(email, 35, 190, 20, DARKGRAY);
        DrawText("Password:", 30, 240, 18, BLACK);
        DrawRectangle(30, 260, 360, 40, (activeField == 2) ? BLUE : LIGHTGRAY);
        DrawText(password, 35, 270, 20, DARKGRAY);
        DrawText("Address:", 30, 320, 18, BLACK);
        DrawRectangle(30, 340, 360, 40, (activeField == 3) ? BLUE : LIGHTGRAY);
        DrawText(address, 35, 350, 20, DARKGRAY);
        DrawText(TextFormat("Age: %d", age), 30, 400, 18, BLACK);
        DrawRectangle(30, 420, 360, 40, (activeField == 4) ? BLUE : LIGHTGRAY);
        DrawText("Use UP/DOWN to change", 35, 430, 18, GRAY);

        DrawText(TextFormat("DOB: %d/%d/%d", day, month, year), 30, 480, 18, BLACK);
        DrawRectangle(30, 500, 360, 40, (activeField == 5) ? BLUE : LIGHTGRAY);
        DrawText("UP/DOWN=Day  LEFT/RIGHT=Month  PGUP/PGDN=Year", 35, 510, 14, GRAY);
        DrawText(TextFormat("Gender: %c", genderChar), 30, 560, 18, BLACK);
        DrawRectangle(30, 580, 360, 40, (activeField == 6) ? BLUE : LIGHTGRAY);
        DrawText("Press M / F / U", 35, 590, 18, GRAY);
        DrawText("Press ENTER to Sign Up", 30, 650, 18, GRAY);
        DrawText("Press ESC to go back", 30, 670, 18, GRAY);
    }
}
