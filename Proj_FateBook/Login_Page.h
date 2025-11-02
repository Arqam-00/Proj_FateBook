#pragma once
#include "raylib.h"
#include "Fate_Book.h"
#include "Date.h"
#include <string>

class LoginPage {
public:
    enum PageState {
        WELCOME,
        LOGIN,
        SIGNUP,
        DONE
    };
private:
    void HandleLoginInput();
    void HandleSignupInput();
    bool AllFieldsFilled() const;
    Fate_Book& fateBook;
    Font font;
    PageState state;
    bool showPassword;
    User* resultUser;
    char loginEmail[256];
    char loginPassword[256];
    char name[256];
    char email[256];
    char password[256];
    char address[256];
    char genderChar;
    int age;
    int day, month, year;
    int framesCounter;
    int activeField;

public:
    LoginPage(Fate_Book& fb);

    void Load(Font f);
    void Reset();
    void Update();
    void Draw();
    bool IsDone() const { return state == DONE && resultUser != nullptr; }
    User* GetUser() const { return resultUser; }
};
