#include "Main_Screen.h"
#include <iostream>

MainApp::MainApp(Fate_Book& fb, User* user)
    : fateBook(fb), currentUser(user), currentScreen(LOGIN), previousScreen(LOGIN),
    viewingProfile(nullptr), loginComplete(false) {

    feedManager = Feed_Manager(currentUser, 10, 10, 560, 850);

    homeBtn = new Button("assets/utilities/home.png", { 60, 940 }, 0.8f);
    friendsBtn = new Button("assets/utilities/friends.png", { 260, 940 }, 0.8f);
    profileBtn = new Button(currentUser->PfpPath.c_str(), { 460, 940 }, 0.8f, true);
    backBtn = new Button("assets/utilities/back.png", { 20, 20 }, 0.6f);
    loginBtn = new Button("assets/utilities/login.png", { 220, 600 }, 0.9f);
}

MainApp::~MainApp() {
    delete homeBtn;
    delete friendsBtn;
    delete profileBtn;
    delete backBtn;
    delete loginBtn;
}

void MainApp::SetCurrentUser(User* user) {
    currentUser = user;
    viewingProfile = user;
    delete profileBtn;
    profileBtn = new Button(currentUser->PfpPath.c_str(), { 460, 940 }, 0.8f, true);
}

void MainApp::Update() {
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (currentScreen == LOGIN) {
        if (fateBook.Initialize()) return;

        loginBtn->Draw();
        if (loginBtn->isPressed(mousePos, mousePressed)) {
            loginComplete = true;
            previousScreen = LOGIN;
            currentScreen = FEED;
        }
        return;
    }

    if (currentScreen != FEED && currentScreen != LOGIN) {
        backBtn->Draw();
        if (backBtn->isPressed(mousePos, mousePressed)) {
            currentScreen = previousScreen;
        }
    }

    homeBtn->Draw();
    friendsBtn->Draw();
    profileBtn->Draw();

    if (mousePressed) {
        if (homeBtn->isPressed(mousePos, mousePressed)) {
            previousScreen = currentScreen;
            currentScreen = FEED;
        }
        if (friendsBtn->isPressed(mousePos, mousePressed)) {
            previousScreen = currentScreen;
            currentScreen = FRIENDS;
        }
        if (profileBtn->isPressed(mousePos, mousePressed)) {
            previousScreen = currentScreen;
            viewingProfile = currentUser;
            currentScreen = PROFILE;
        }
    }

    switch (currentScreen) {
    case FEED: feedManager.Update(); break;
    case PROFILE: break;
    case FRIENDS: break;
    case SETTINGS: break;
    case SIGNUP_PAGE1: break;
    case SIGNUP_PAGE2: break;
    default: break;
    }
}

void MainApp::Draw() {
    ClearBackground(RAYWHITE);

    switch (currentScreen) {
    case LOGIN: DrawLoginScreen(); break;
    case SIGNUP_PAGE1: DrawSignupPage1(); break;
    case SIGNUP_PAGE2: DrawSignupPage2(); break;
    case FEED: if (loginComplete) DrawFeedScreen(); break;
    case FRIENDS: DrawFriendsScreen(); break;
    case PROFILE: DrawProfileScreen(); break;
    case SETTINGS: DrawSettingsScreen(); break;
    }

    if (currentScreen != FEED && currentScreen != LOGIN) {
        backBtn->Draw();
    }

    if (currentScreen != LOGIN) {
        homeBtn->Draw();
        friendsBtn->Draw();
        profileBtn->Draw();
    }
}

void MainApp::DrawLoginScreen() {
    if (!fateBook.Initialize()) {
        DrawText("Initializing FateBook...", 180, 500, 20, DARKGRAY);
        return;
    }

    DrawText("Login", 240, 150, 30, DARKBLUE);
    DrawText("Click to continue", 200, 550, 20, GRAY);
    loginBtn->Draw();
}

void MainApp::DrawSignupPage1() {
    DrawText("Signup - Step 1", 180, 100, 28, DARKBLUE);
}

void MainApp::DrawSignupPage2() {
    DrawText("Signup - Step 2", 180, 100, 28, DARKBLUE);
}

void MainApp::DrawFeedScreen() {
    DrawText("PUBLIC FEED", 200, 40, 24, DARKBLUE);
    feedManager.Draw();
    DrawText("Scroll with mouse wheel or UP/DOWN keys", 100, 900, 14, GRAY);
}

void MainApp::DrawFriendsScreen() {
    DrawText("FRIENDS", 220, 40, 24, DARKBLUE);
}

void MainApp::DrawProfileScreen() {
    if (viewingProfile) {
        currentUser->View_Profile(viewingProfile, currentUser, GetFontDefault(), 10, 10, 560);
    }
    else {
        DrawText("No profile to display", 200, 500, 20, GRAY);
    }
}

void MainApp::DrawSettingsScreen() {
    DrawText("SETTINGS", 200, 50, 30, DARKBLUE);
    DrawText("Settings screen coming soon...", 150, 200, 20, GRAY);
}
