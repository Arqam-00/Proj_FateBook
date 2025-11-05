#include "Login_Page.h"
#include <iostream>
#include <algorithm>

LoginPage::LoginPage(Fate_Book& fb)
    : fateBook(fb), state(WELCOME), resultUser(nullptr),
    activeField(0), showPassword(false) {
    font = LoadFont("assets/static/Roboto_Condensed-Black.ttf");
    loginBtn = new Button("assets/utilities/login.png", { 190, 500 }, 0.6f);
    signupBtn = new Button("assets/utilities/signup.png", { 190, 600 }, 0.6f);
    nextBtn = new Button("assets/utilities/next.png", { 480, 950 }, 0.12f);
    backBtn = new Button("assets/utilities/back.png", { 40, 950 }, 0.12f);
}

LoginPage::~LoginPage() {
    delete loginBtn;
    delete signupBtn;
    delete nextBtn;
    delete backBtn;
}

void LoginPage::Load(Font f) {
    font = f;
}

void LoginPage::Reset() {
    state = WELCOME;
    resultUser = nullptr;
    activeField = 0;
    showPassword = false;
    signupData = { "", "", "", "", "01", "01", "2000", "", 'U', false };
    loginData = { "", "" };
}

bool LoginPage::IsDone() const {
    return state == DONE && resultUser != nullptr;
}

User* LoginPage::GetUser() const {
    return resultUser;
}

void LoginPage::Update() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (state != WELCOME && backBtn->isPressed(mouse, click)) {
        if (state == SIGNUP_STEP2) {
            state = SIGNUP_STEP1;
            activeField = 0;
        }
        else {
            state = WELCOME;
        }
        return;
    }

    if (nextBtn->isPressed(mouse, click)) {
        if (state == LOGIN) {
            resultUser = fateBook.Check(loginData.email.c_str(), loginData.password.c_str());
            if (resultUser) state = DONE;
        }
        else if (state == SIGNUP_STEP1 && ValidateCurrentStep()) {
            state = SIGNUP_STEP2;
            activeField = 0;
        }
        else if (state == SIGNUP_STEP2 && ValidateCurrentStep()) {
            if (fateBook.Check_By_Email(signupData.email.c_str())) return;

            int day = stoi(signupData.day);
            int month = stoi(signupData.month);
            int year = stoi(signupData.year);
            int age = stoi(signupData.age);

            Date dob(day, month, year);
            if (!dob.IsValid()) return;

            resultUser = fateBook.CreateUser(
                signupData.name.c_str(),
                signupData.email.c_str(),
                signupData.password.c_str(),
                signupData.address.c_str(),
                signupData.gender,
                age,
                signupData.isPrivate,
                dob
            );

            if (resultUser) {
                fateBook.GetUsers()->push_back(resultUser);
                state = DONE;
            }
        }
    }

    switch (state) {
    case WELCOME:
        HandleWelcomeInput();
        break;
    case LOGIN:
        HandleLoginInput();
        break;
    case SIGNUP_STEP1:
        HandleSignupStep1Input();
        break;
    case SIGNUP_STEP2:
        HandleSignupStep2Input();
        break;
    case DONE:
        break;
    }
}

void LoginPage::Draw() {
    ClearBackground({ 245, 245, 245, 255 });

    switch (state) {
    case WELCOME:
        DrawWelcome();
        break;
    case LOGIN:
        DrawLogin();
        break;
    case SIGNUP_STEP1:
        DrawSignupStep1();
        break;
    case SIGNUP_STEP2:
        DrawSignupStep2();
        break;
    case DONE:
        break;
    }

    if (state != WELCOME) {
        nextBtn->Draw();
        backBtn->Draw();
    }
}

void LoginPage::DrawWelcome() {
    DrawTextEx(font, "FateBook", { 1.0f * screenWidth / 2 - 100, 200 }, 48, 2, BLUE);
    DrawTextEx(font, "Connect with your destiny", { 1.0f * screenWidth / 2 - 150, 270 }, 20, 1, DARKGRAY);
    loginBtn->Draw();
    signupBtn->Draw();
    DrawTextEx(font, "New here? Create an account to get started!",
        { 1.0f * screenWidth / 2 - 200, 400 }, 18, 1, GRAY);
}

void LoginPage::DrawLogin() {
    DrawTextEx(font, "Welcome Back", { 1.0f * screenWidth / 2 - 120, 120 }, 36, 2, DARKBLUE);

    Rectangle emailField = { 40, 220, 500, 60 };
    Rectangle passwordField = { 40, 310, 500, 60 };
    DrawInputField("Email", loginData.email, emailField, activeField == 0);
    DrawInputField("Password", loginData.password, passwordField, activeField == 1, true);

    Rectangle toggleRect = { 460, 325, 70, 30 };
    DrawRectangleRec(toggleRect, LIGHTGRAY);
    DrawTextEx(font, showPassword ? "Hide" : "Show", { 465, 330 }, 14, 1, DARKGRAY);

    bool canLogin = !loginData.email.empty() && !loginData.password.empty();
    DrawButton("Log In", { 40, 420, 500, 60 }, canLogin);
    DrawTextEx(font, "Don't have an account? Go back and sign up!",
        { 40, 500 }, 16, 1, GRAY);
}

void LoginPage::DrawSignupStep1() {
    DrawTextEx(font, "Create Account", {1.0f* screenWidth / 2 - 120, 80 }, 36, 2, DARKBLUE);
    DrawTextEx(font, "Step 1: Basic Information", { 1.0f * screenWidth / 2 - 140, 130 }, 20, 1, DARKGRAY);

    Rectangle nameField = { 40, 180, 500, 55 };
    Rectangle emailField = { 40, 260, 500, 55 };
    Rectangle passwordField = { 40, 340, 500, 55 };

    DrawInputField("Full Name", signupData.name, nameField, activeField == 0);
    DrawInputField("Email", signupData.email, emailField, activeField == 1);

    string passwordDisplay = showPassword ? signupData.password : string(signupData.password.length(), '*');
    DrawInputField("Password", passwordDisplay, passwordField, activeField == 2);

    if (!signupData.password.empty()) {
        Color strengthColor = signupData.password.length() >= 8 ? GREEN : ORANGE;
        DrawRectangle(40, 400, (signupData.password.length() * 500 / 16), 5, strengthColor);
        DrawTextEx(font, "Password strength", { 40, 410 }, 14, 1, DARKGRAY);
    }

    bool canContinue = !signupData.name.empty() && !signupData.email.empty() && signupData.password.length() >= 8;
    DrawButton("Continue", { 40, 450, 500, 60 }, canContinue);
}

void LoginPage::DrawSignupStep2() {
    DrawTextEx(font, "Create Account", { 1.0f * screenWidth / 2 - 120, 80 }, 36, 2, DARKBLUE);
    DrawTextEx(font, "Step 2: Additional Information", { 1.0f * screenWidth / 2 - 160, 130 }, 20, 1, DARKGRAY);

    Rectangle addressField = { 40, 180, 500, 55 };
    Rectangle ageField = { 40, 260, 500, 55 };
    Rectangle dayField = { 40, 340, 150, 55 };
    Rectangle monthField = { 210, 340, 150, 55 };
    Rectangle yearField = { 380, 340, 160, 55 };

    DrawInputField("Address", signupData.address, addressField, activeField == 0);
    DrawInputField("Age", signupData.age, ageField, activeField == 1, false, true);

    // Draw date fields exactly like email fields
    DrawInputField("Day", signupData.day, dayField, activeField == 2, false, stoi(signupData.day) < 1 || stoi(signupData.day) > 31);
    DrawInputField("Month", signupData.month, monthField, activeField == 3, false, stoi(signupData.month) < 1 || stoi(signupData.month) > 12);
    DrawInputField("Year", signupData.year, yearField, activeField == 4, false, stoi(signupData.year) < 1900);

    DrawTextEx(font, "Gender", { 40, 420 }, 20, 1, BLACK);
    DrawGenderField();

    DrawPrivacyField();

    bool canSignup = ValidateCurrentStep();
    DrawButton("Create Account", { 40, 650, 500, 60 }, canSignup);
}

void LoginPage::DrawInputField(const string& label, const string& value, Rectangle bounds, bool isActive, bool isPassword, bool showError) {
    DrawTextEx(font, label.c_str(), { bounds.x, bounds.y - 25 }, 18, 1, BLACK);
    Color borderColor = showError ? RED : (isActive ? BLUE : GRAY);
    DrawRectangleRec(bounds, WHITE);
    DrawRectangleLinesEx(bounds, 2, borderColor);
    Color textColor = value.empty() ? GRAY : BLACK;
    string displayText = value.empty() ? "Enter " + label : value;
    if (isPassword && !value.empty()) {
        displayText = string(value.length(), '*');
    }
    Vector2 textSize = MeasureTextEx(font, displayText.c_str(), 20, 1);
    float textY = bounds.y + (bounds.height - textSize.y) / 2;
    DrawTextEx(font, displayText.c_str(), { bounds.x + 10, textY }, 20, 1, textColor);
    if (showError) {
        DrawTextEx(font, "Invalid input", { bounds.x, bounds.y + bounds.height + 5 }, 14, 1, RED);
    }
}

void LoginPage::DrawGenderField() {
    Rectangle maleBtn = { 40, 450, 160, 50 };
    Rectangle femaleBtn = { 210, 450, 160, 50 };
    Rectangle otherBtn = { 380, 450, 160, 50 };

    Color maleColor = signupData.gender == 'M' ? BLUE : LIGHTGRAY;
    Color femaleColor = signupData.gender == 'F' ? BLUE : LIGHTGRAY;
    Color otherColor = signupData.gender == 'U' ? BLUE : LIGHTGRAY;

    DrawRectangleRec(maleBtn, maleColor);
    DrawRectangleRec(femaleBtn, femaleColor);
    DrawRectangleRec(otherBtn, otherColor);

    DrawRectangleLinesEx(maleBtn, 2, DARKGRAY);
    DrawRectangleLinesEx(femaleBtn, 2, DARKGRAY);
    DrawRectangleLinesEx(otherBtn, 2, DARKGRAY);

    Vector2 maleTextSize = MeasureTextEx(font, "Male", 18, 1);
    Vector2 femaleTextSize = MeasureTextEx(font, "Female", 18, 1);
    Vector2 otherTextSize = MeasureTextEx(font, "Other", 18, 1);

    DrawTextEx(font, "Male", { maleBtn.x + (maleBtn.width - maleTextSize.x) / 2, maleBtn.y + (maleBtn.height - maleTextSize.y) / 2 }, 18, 1, WHITE);
    DrawTextEx(font, "Female", { femaleBtn.x + (femaleBtn.width - femaleTextSize.x) / 2, femaleBtn.y + (femaleBtn.height - femaleTextSize.y) / 2 }, 18, 1, WHITE);
    DrawTextEx(font, "Other", { otherBtn.x + (otherBtn.width - otherTextSize.x) / 2, otherBtn.y + (otherBtn.height - otherTextSize.y) / 2 }, 18, 1, WHITE);
}

void LoginPage::DrawPrivacyField() {
    DrawTextEx(font, "Account Privacy", { 40, 520 }, 20, 1, BLACK);

    Rectangle checkbox = { 40, 550, 30, 30 };
    DrawRectangleRec(checkbox, WHITE);
    DrawRectangleLinesEx(checkbox, 2, GRAY);

    if (signupData.isPrivate) {
        DrawRectangle(checkbox.x + 5, checkbox.y + 5, 20, 20, BLUE);
    }

    DrawTextEx(font, "Private Account", { 80, 555 }, 18, 1, BLACK);
    DrawTextEx(font, "Only approved followers can see your content", { 40, 585 }, 14, 1, GRAY);
}

void LoginPage::DrawButton(const string& text, Rectangle bounds, bool enabled) {
    Color btnColor = enabled ? BLUE : GRAY;
    DrawRectangleRec(bounds, btnColor);
    DrawRectangleLinesEx(bounds, 2, enabled ? DARKBLUE : DARKGRAY);

    Vector2 textSize = MeasureTextEx(font, text.c_str(), 22, 1);
    DrawTextEx(font, text.c_str(),
        { bounds.x + (bounds.width / 2) - textSize.x / 2,
          bounds.y + (bounds.height / 2) - textSize.y / 2 },
        22, 1, WHITE);
}

void LoginPage::HandleWelcomeInput() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (loginBtn->isPressed(mouse, click)) {
        state = LOGIN;
        activeField = 0;
    }
    if (signupBtn->isPressed(mouse, click)) {
        state = SIGNUP_STEP1;
        activeField = 0;
    }
}

void LoginPage::HandleLoginInput() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Rectangle fields[] = { {40, 220, 500, 60}, {40, 310, 500, 60} };
    for (int i = 0; i < 2; i++) {
        if (click && CheckCollisionPointRec(mouse, fields[i])) {
            activeField = i;
        }
    }

    if (activeField == 0) ProcessTextInput(loginData.email, 255);
    if (activeField == 1) ProcessTextInput(loginData.password, 255);

    Rectangle toggleRect = { 460, 325, 70, 30 };
    if (click && CheckCollisionPointRec(mouse, toggleRect)) {
        showPassword = !showPassword;
    }

    Rectangle loginButton = { 40, 420, 500, 60 };
    if (click && CheckCollisionPointRec(mouse, loginButton) && !loginData.email.empty() && !loginData.password.empty()) {
        resultUser = fateBook.Check(loginData.email.c_str(), loginData.password.c_str());
        if (resultUser) {
            state = DONE;
        }
        else {
            loginData.password.clear();
        }
    }
}

void LoginPage::HandleSignupStep1Input() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Rectangle fields[] = { {40, 180, 500, 55}, {40, 260, 500, 55}, {40, 340, 500, 55} };
    for (int i = 0; i < 3; i++) {
        if (click && CheckCollisionPointRec(mouse, fields[i])) {
            activeField = i;
        }
    }

    if (activeField == 0) ProcessTextInput(signupData.name, 255);
    if (activeField == 1) ProcessTextInput(signupData.email, 255);
    if (activeField == 2) ProcessTextInput(signupData.password, 255);

    Rectangle continueButton = { 40, 450, 500, 60 };
    if (click && CheckCollisionPointRec(mouse, continueButton) && ValidateCurrentStep()) {
        state = SIGNUP_STEP2;
        activeField = 0;
    }
}

void LoginPage::HandleSignupStep2Input() {
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    Rectangle maleBtn = { 40, 450, 160, 50 };
    Rectangle femaleBtn = { 210, 450, 160, 50 };
    Rectangle otherBtn = { 380, 450, 160, 50 };
    Rectangle privacyBox = { 40, 550, 30, 30 };

    if (click && CheckCollisionPointRec(mouse, maleBtn)) signupData.gender = 'M';
    if (click && CheckCollisionPointRec(mouse, femaleBtn)) signupData.gender = 'F';
    if (click && CheckCollisionPointRec(mouse, otherBtn)) signupData.gender = 'U';
    if (click && CheckCollisionPointRec(mouse, privacyBox)) signupData.isPrivate = !signupData.isPrivate;

    Rectangle fields[] = {
        {40, 180, 500, 55},    // address
        {40, 260, 500, 55},    // age
        {40, 340, 150, 55},    // day
        {210, 340, 150, 55},   // month
        {380, 340, 160, 55}    // year
    };

    for (int i = 0; i < 5; i++) {
        if (click && CheckCollisionPointRec(mouse, fields[i])) {
            activeField = i;
        }
    }

    if (activeField == 0) ProcessTextInput(signupData.address, 255);
    if (activeField == 1) ProcessTextInput(signupData.age, 3, true);
    if (activeField == 2) ProcessTextInput(signupData.day, 2, true);
    if (activeField == 3) ProcessTextInput(signupData.month, 2, true);
    if (activeField == 4) ProcessTextInput(signupData.year, 4, true);

    Rectangle createButton = { 40, 650, 500, 60 };
    if (click && CheckCollisionPointRec(mouse, createButton) && ValidateCurrentStep()) {
        if (fateBook.Check_By_Email(signupData.email.c_str())) return;

        int day = stoi(signupData.day);
        int month = stoi(signupData.month);
        int year = stoi(signupData.year);
        int age = stoi(signupData.age);

        Date dob(day, month, year);
        if (!dob.IsValid()) return;

        resultUser = fateBook.CreateUser(
            signupData.name.c_str(),
            signupData.email.c_str(),
            signupData.password.c_str(),
            signupData.address.c_str(),
            signupData.gender,
            age,
            signupData.isPrivate,
            dob
        );

        if (resultUser) {
            fateBook.GetUsers()->push_back(resultUser);
            state = DONE;
        }
    }
}

void LoginPage::ProcessTextInput(string& field, int maxLength, bool numbersOnly) {
    int key = GetCharPressed();
    while (key > 0) {
        if (field.length() < maxLength) {
            if (numbersOnly) {
                if (key >= '0' && key <= '9') {
                    field += (char)key;
                }
            }
            else {
                if (key >= 32 && key <= 125) {
                    field += (char)key;
                }
            }
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !field.empty()) {
        field.pop_back();
    }
}

bool LoginPage::ValidateCurrentStep() {
    switch (state) {
    case SIGNUP_STEP1:
        return !signupData.name.empty() &&
            !signupData.email.empty() &&
            IsValidEmail(signupData.email) &&
            signupData.password.length() >= 8;

    case SIGNUP_STEP2:
        return !signupData.address.empty() &&
            !signupData.age.empty() &&
            IsValidDate() &&
            stoi(signupData.age) > 0;

    default:
        return true;
    }
}

bool LoginPage::IsValidEmail(const string& email) {
    return email.find('@') != string::npos &&
        email.find('.') != string::npos &&
        email.length() > 5;
}

bool LoginPage::IsValidDate() {
    try {
        int day = stoi(signupData.day);
        int month = stoi(signupData.month);
        int year = stoi(signupData.year);

        return day >= 1 && day <= 31 &&
            month >= 1 && month <= 12 &&
            year >= 1900 && year <= 2024;
    }
    catch (...) {
        return false;
    }
}