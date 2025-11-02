#include "Main_Screen.h"
#include <iostream>

MainApp::MainApp(Fate_Book& fb, User* user)
    : fateBook(fb), currentUser(user), currentScreen(FEED), viewingProfile(nullptr) {
    feedManager = Feed_Manager(currentUser, 10, 10, 400, 650);
    fateBook.GetPostManager()->BuildFeedFromAllPosts(*fateBook.GetUsers());
    int btnSize = 60;
    int padding = 20;
    feedBtn = { (float)padding, (float)(800 - btnSize - padding), (float)btnSize, (float)btnSize };
    profileBtn = { (float)(420 - btnSize - padding), (float)(800 - btnSize - padding), (float)btnSize, (float)btnSize };
}

void MainApp::Update() {
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    if (currentScreen == FEED) {
        feedManager.Update();
    }
    if (mousePressed) {
        if (CheckCollisionPointRec(mousePos, feedBtn)) {
            currentScreen = FEED;
            viewingProfile = nullptr;
        }
        else if (CheckCollisionPointRec(mousePos, profileBtn)) {
            currentScreen = PROFILE;
            viewingProfile = currentUser;
        }
    }
}

void MainApp::Draw() {
    ClearBackground(RAYWHITE);

    switch (currentScreen) {
    case FEED:
        DrawFeedScreen();
        break;
    case PROFILE:
        DrawProfileScreen();
        break;
    case SETTINGS:
        DrawSettingsScreen();
        break;
    }

    DrawBottomBar();
}

void MainApp::DrawBottomBar() {
    DrawRectangle(0, 700, 420, 100, LIGHTGRAY);

    DrawLine(0, 700, 420, 700, DARKGRAY);

    Color feedColor = (currentScreen == FEED) ? BLUE : GRAY;
    DrawRectangleRec(feedBtn, feedColor);
    DrawText("Feed", feedBtn.x + 10, feedBtn.y + 20, 16, WHITE);

    Color profileColor = (currentScreen == PROFILE) ? GREEN : GRAY;

    Vector2 profileCenter = { profileBtn.x + profileBtn.width / 2, profileBtn.y + profileBtn.height / 2 };
    DrawCircleV(profileCenter, profileBtn.width / 2, profileColor);
    if (currentUser && currentUser->PfpTexture.id != 0) {
        DrawTexturePro(currentUser->PfpTexture,
            { 0, 0, (float)currentUser->PfpTexture.width, (float)currentUser->PfpTexture.height },
            { profileBtn.x + 5, profileBtn.y + 5, profileBtn.width - 10, profileBtn.height - 10 },
            { 0, 0 }, 0, WHITE);
    }
    else {
        DrawText("PFP", profileBtn.x + 15, profileBtn.y + 20, 16, WHITE);
    }
    DrawText(TextFormat("Screen: %s",
        currentScreen == FEED ? "Feed" :
        currentScreen == PROFILE ? "Profile" : "Settings"),
        150, 730, 16, DARKGRAY);
}

void MainApp::DrawFeedScreen() {
    DrawText("PUBLIC FEED", 150, 20, 24, DARKBLUE);
    feedManager.Draw();
    DrawText("Scroll with mouse wheel or UP/DOWN keys", 50, 670, 14, GRAY);
}

void MainApp::DrawProfileScreen() {
    if (viewingProfile) {
        currentUser->View_Profile(viewingProfile, currentUser, GetFontDefault(), 10, 10, 400);
    }
    else {
        DrawText("No profile to display", 150, 350, 20, GRAY);
    }
}

void MainApp::DrawSettingsScreen() {
    DrawText("SETTINGS", 150, 50, 30, DARKBLUE);
    DrawText("Settings screen coming soon...", 100, 200, 20, GRAY);
}

void MainApp::SetCurrentUser(User* user) {
    currentUser = user;
    viewingProfile = user;
}