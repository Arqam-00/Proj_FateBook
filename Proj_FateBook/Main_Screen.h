#pragma once
#include "Fate_Book.h"
#include "Feed_Manager.h"
#include "User.h"
#include <raylib.h>

class MainApp {
private:
    Fate_Book& fateBook;
    User* currentUser;
    Feed_Manager feedManager;
    Rectangle feedBtn;
    Rectangle profileBtn;
    Rectangle settingsBtn;
    enum Screen { FEED, PROFILE, SETTINGS };
    Screen currentScreen;
    User* viewingProfile;
public:
    MainApp(Fate_Book& fb, User* user);
    void Update();
    void Draw();
    void SetCurrentUser(User* user);

private:
    void DrawBottomBar();
    void DrawFeedScreen();
    void DrawProfileScreen();
    void DrawSettingsScreen();
};