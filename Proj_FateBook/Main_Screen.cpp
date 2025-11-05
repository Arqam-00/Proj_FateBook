#include "Main_Screen.h"
#include <iostream>

MainApp::MainApp(Fate_Book& fb, User* user)
    : fateBook(fb), currentUser(user), currentScreen(FEED), previousScreen(FEED),
    viewingProfile(user) {

    feedManager = Feed_Manager(currentUser, 40, 120, 500, 800);

    homeBtn = new Button("assets/utilities/home.png", { 60, 940 }, 1.0f * 80, 1.0f * 80);
    friendsBtn = new Button("assets/utilities/friends.png", { 260, 940 }, 1.0f * 80, 1.0f * 80);

    if (currentUser && !currentUser->PfpPath.empty()) {
        profileBtn = new Button(currentUser->PfpPath.c_str(), { 460, 940 }, 70, 70, true);
    }
    else {
        profileBtn = new Button("assets/pfps/default.png", { 460, 940 }, 1.0f * 70, 1.0f * 70, true);
    }

    backBtn = new Button("assets/utilities/back.png", { 20, 50 },1.0f* 50, 1.0f * 50);
    settingsBtn = new Button("assets/utilities/settings.png", { 520, 50 }, 1.0f * 40, 1.0f * 40);
    logoutBtn = new Button("assets/utilities/logout.png", { 40, 300 }, 1.0f * 300, 1.0f * 60);
    deleteAccountBtn = new Button("assets/utilities/delete.png", { 40, 400 }, 1.0f * 400, 1.0f * 60);
}

MainApp::~MainApp() {
    delete homeBtn;
    delete friendsBtn;
    delete profileBtn;
    delete backBtn;
    delete settingsBtn;
    delete logoutBtn;
    delete deleteAccountBtn;
}

void MainApp::SetCurrentUser(User* user) {
    currentUser = user;
    viewingProfile = user;

    delete profileBtn;
    if (currentUser && !currentUser->PfpPath.empty()) {
        profileBtn = new Button(currentUser->PfpPath.c_str(), { 460, 940 }, 70, 70, true);
    }
    else {
        profileBtn = new Button("assets/utilities/default_pfp.png", { 460, 940 }, 70, 70, true);
    }
}

void MainApp::Update() {
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (currentScreen != FEED) {
        if (backBtn->isPressed(mousePos, mousePressed)) {
            currentScreen = previousScreen;
            return;
        }
    }

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

    if (currentScreen == FEED && settingsBtn->isPressed(mousePos, mousePressed)) {
        previousScreen = currentScreen;
        currentScreen = SETTINGS;
    }

    if (currentScreen == SETTINGS) {
        if (logoutBtn->isPressed(mousePos, mousePressed)) {
            if (Popup::Show("Logout", "Are you sure you want to logout?")) {
            }
        }
        if (deleteAccountBtn->isPressed(mousePos, mousePressed)) {
            if (Popup::Show("Delete Account", "This will permanently delete your account and all data. This action cannot be undone!")) {
                fateBook.DeleteUser();
            }
        }
    }

    switch (currentScreen) {
    case FEED:
        feedManager.Update();
        break;
    case PROFILE:
        break;
    case FRIENDS:
        break;
    case SETTINGS:
        break;
    }
}

void MainApp::Draw() {
    ClearBackground({ 245, 245, 245, 255 });

    switch (currentScreen) {
    case FEED:
        DrawFeedScreen();
        break;
    case FRIENDS:
        DrawFriendsScreen();
        break;
    case PROFILE:
        DrawProfileScreen();
        break;
    case SETTINGS:
        DrawSettingsScreen();
        break;
    }

    DrawNavigationBar();
}

void MainApp::DrawNavigationBar() {
    DrawRectangle(0, 900, screenWidth, 120, { 255, 255, 255, 255 });
    DrawLine(0, 900, screenWidth, 900, { 220, 220, 220, 255 });

    if (currentScreen == FEED) {
        DrawCircle(homeBtn->getPosition().x + homeBtn->getWidth() / 2,
            homeBtn->getPosition().y + homeBtn->getHeight() / 2,
            homeBtn->getWidth() / 2, { 200, 230, 255, 255 });
    }
    if (currentScreen == FRIENDS) {
        DrawCircle(friendsBtn->getPosition().x + friendsBtn->getWidth() / 2,
            friendsBtn->getPosition().y + friendsBtn->getHeight() / 2,
            friendsBtn->getWidth() / 2, { 200, 230, 255, 255 });
    }
    if (currentScreen == PROFILE) {
        DrawCircle(profileBtn->getPosition().x + profileBtn->getWidth() / 2,
            profileBtn->getPosition().y + profileBtn->getHeight() / 2,
            profileBtn->getWidth() / 2, { 200, 230, 255, 255 });
    }

    homeBtn->Draw();
    friendsBtn->Draw();
    profileBtn->Draw();

    if (currentScreen != FEED) {
        backBtn->Draw();
    }
    else {
        settingsBtn->Draw();
    }
}

void MainApp::DrawFeedScreen() {
    DrawText("Your Feed", 40, 80, 32, DARKBLUE);
    feedManager.Draw();
    DrawText("Scroll with mouse wheel or UP/DOWN keys", 120, 890, 14, GRAY);
}

void MainApp::DrawFriendsScreen() {
    DrawText("Friends", 40, 80, 32, DARKBLUE);
    DrawText("Friends feature coming soon...", 40, 150, 20, GRAY);
}

void MainApp::DrawProfileScreen() {
    if (viewingProfile && currentUser) {
        currentUser->View_Profile(viewingProfile, currentUser, GetFontDefault(), 40, 120, 500);
    }
    else {
        DrawText("No profile to display", screenWidth / 2 - 100, 400, 20, GRAY);
    }
}

void MainApp::DrawSettingsScreen() {
    DrawText("Settings", 40, 80, 32, DARKBLUE);

    if (currentUser) {
        DrawText(("User: " + currentUser->GetName()).c_str(), 40, 150, 20, BLACK);
        DrawText(("Email: " + string(currentUser->Email)).c_str(), 40, 180, 18, DARKGRAY);
    }

    DrawText("Account Options:", 40, 250, 22, DARKBLUE);

    logoutBtn->Draw();
    DrawText("Log Out", 250, 320, 22, BLACK);

    deleteAccountBtn->Draw();
    DrawText("Delete Account", 220, 420, 22, BLACK);
    DrawText("Warning: Deleting account cannot be undone", 40, 480, 16, RED);
}