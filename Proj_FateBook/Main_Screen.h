#pragma once
#include "Fate_Book.h"
#include "Feed_Manager.h"
#include "User.h"
#include "Button.h"
#include <raylib.h>
#include <string>
using namespace std;

class MainApp {
private:
    Fate_Book& fateBook;
    User* currentUser;
    Feed_Manager feedManager;

    enum Screen { FEED, FRIENDS, PROFILE, SETTINGS };
    Screen currentScreen;
    Screen previousScreen;
    User* viewingProfile;

    const int screenWidth = 580;
    const int screenHeight = 1020;

    Button* homeBtn;
    Button* friendsBtn;
    Button* profileBtn;
    Button* backBtn;
    Button* settingsBtn;
    Button* logoutBtn;
    Button* deleteAccountBtn;
    void DrawFeedScreen();
    void DrawFriendsScreen();
    void DrawProfileScreen();
    void DrawSettingsScreen();
    void DrawNavigationBar();
public:
    MainApp(Fate_Book& fb, User* user);
    ~MainApp();

    void Update();
    void Draw();
    void SetCurrentUser(User* user);
   
};