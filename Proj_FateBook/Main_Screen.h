#pragma once
#include "Fate_Book.h"
#include "Feed_Manager.h"
#include "User.h"
#include "button.h"
#include <raylib.h>

class MainApp {
private:
    Fate_Book& fateBook;
    User* currentUser;
    Feed_Manager feedManager;
    User* viewingProfile;

    enum Screen {
        LOGIN,
        SIGNUP_PAGE1,
        SIGNUP_PAGE2,
        FEED,
        FRIENDS,
        PROFILE,
        SETTINGS
    };
    Screen currentScreen;
    Screen previousScreen;

    bool loginComplete;

    Button* homeBtn;
    Button* friendsBtn;
    Button* profileBtn;
    Button* backBtn;
    Button* loginBtn;

public:
    MainApp(Fate_Book& fb, User* user);
    ~MainApp();

    void Update();
    void Draw();
    void SetCurrentUser(User* user);

private:
    void DrawLoginScreen();
    void DrawSignupPage1();
    void DrawSignupPage2();
    void DrawFeedScreen();
    void DrawFriendsScreen();
    void DrawProfileScreen();
    void DrawSettingsScreen();
};
