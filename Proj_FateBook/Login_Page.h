#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include "User.h"
#include "Date.h"
using namespace std;

enum LoginScreenState { WELCOME, LOGIN, SIGNUP, DONE };

class LoginPage {
private:
    LoginScreenState state;
    Font font;
    bool showPassword;

    char loginEmail[64];
    char loginPassword[64];
    char name[64];
    char email[64];
    char password[64];
    char location[64];
    char genderChar;
    int age;
    int day, month, year;

    vector<User*>& users;
    User* resultUser;

public:
    LoginPage(vector<User*>& allUsers);
    void Load(Font f);
    void Update();
    void Draw();
    bool IsDone() const;
    User* GetUser() const;
};
