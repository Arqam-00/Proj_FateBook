// Login_Page.h
#pragma once
#include "Fate_Book.h"
#include <raylib.h>
#include <string>
using namespace std;

class TextInput {
private:
    char* buffer;
    int maxLength;
    int& framesCounter;

public:
    TextInput(char* buf, int maxLen, int& framesCounterRef)
        : buffer(buf), maxLength(maxLen), framesCounter(framesCounterRef) {
    }

    void Update() {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (strlen(buffer) < maxLength - 1)) {
                int len = strlen(buffer);
                buffer[len] = (char)key;
                buffer[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int length = strlen(buffer);
            if (length > 0) {
                buffer[length - 1] = '\0';
            }
        }
    }

    void Draw(int x, int y, int fontSize, Color color) {
        DrawText(buffer, x, y, fontSize, color);
        if (((framesCounter / 20) % 2) == 0) {
            int textWidth = MeasureText(buffer, fontSize);
            DrawText("_", x + textWidth, y, fontSize, color);
        }
    }
};

class LoginPage {
public:
    enum State { WELCOME, LOGIN, SIGNUP, DONE };

private:
    Fate_Book& fateBook;
    State state;

    char loginEmail[256];
    char loginPassword[256];

    char name[256];
    char email[256];
    char password[256];
    char location[256];
    char genderChar;
    int age;
    int day, month, year;

    bool showPassword;
    User* resultUser;
    Font font;
    int framesCounter;
    int activeField; 
public:
    LoginPage(Fate_Book& fb);
    void Load(Font f);
    bool IsDone() const;
    User* GetUser() const;
    void Reset();
    void Update();
    void Draw();

private:
    void HandleLoginInput();
    void HandleSignupInput();
};