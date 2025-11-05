#pragma once
#include "Fate_Book.h"
#include "Button.h"
#include <raylib.h>
#include <string>
using namespace std;

class LoginPage {
private:
    Fate_Book& fateBook;
    enum State { WELCOME, LOGIN, SIGNUP_STEP1, SIGNUP_STEP2, DONE };
    State state;
    User* resultUser;

    struct SignupData {
        string name;
        string email;
        string password;
        string address;
        string day = "01";
        string month = "01";
        string year = "2000";
        string age;
        char gender = 'U';
        bool isPrivate = false;
    } 
    signupData;

    struct LoginData {
        string email;
        string password;
    } loginData;

    int activeField;
    bool showPassword;
    Font font;

    Button* loginBtn;
    Button* signupBtn;
    Button* nextBtn;
    Button* backBtn;

    const int screenWidth = 580;
    const int screenHeight = 1020;

public:
    LoginPage(Fate_Book& fb);
    ~LoginPage();

    void Load(Font f);
    void Reset();
    bool IsDone() const;
    User* GetUser() const;
    void Update();
    void Draw();

private:
    void DrawWelcome();
    void DrawLogin();
    void DrawSignupStep1();
    void DrawSignupStep2();

    void DrawInputField(const string& label, const string& value, Rectangle bounds, bool isActive, bool isPassword = false, bool showError = false);
    void DrawGenderField();
    void DrawPrivacyField();
    void DrawButton(const string& text, Rectangle bounds, bool enabled = true);

    void HandleWelcomeInput();
    void HandleLoginInput();
    void HandleSignupStep1Input();
    void HandleSignupStep2Input();

    void ProcessTextInput(string& field, int maxLength, bool numbersOnly = false);
    bool ValidateCurrentStep();
    bool IsValidEmail(const string& email);
    bool IsValidDate();
};