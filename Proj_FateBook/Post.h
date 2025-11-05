#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "User.h"
#include <raylib.h>
#include "button.h"
#include <list>
using namespace std;

struct Post
{
    int Ownerid;
    User* Owner;
    string Text;
    Font f = {0};
    Texture2D OwnerPfp;
    bool Is_Private;
    bool IsDeleted;
    vector<string> Media_Paths;
    time_t TimeStamp;
    list<int> LikedBy;
    Button* ownerAvatarBtn = nullptr;
    Button* likeBtn = nullptr;
    Button* lockBtn = nullptr;

    Post();
    Post(User* OwnerPtr, const string& TextContent, bool Private = false);
    void AddImage(string Path) {
        Media_Paths.push_back(Path);
    }
    bool IsVisibleTo(const User* viewer) const {
        if (IsDeleted) return false;
        if (!Is_Private) return true;
        return viewer && Owner && Owner->IsFriend(const_cast<User*>(viewer));
    }
    bool IsLikedBy(User*& U) {
        for (auto i = LikedBy.begin(); i != LikedBy.end(); i++) {
            if (*i == U->id)return true;;
        }
        return false;
    }
    void Unlike(User*& U) {
        for (auto i = LikedBy.begin(); i != LikedBy.end(); i++) {
            if (*i == U->id)LikedBy.erase(i);
        }
    }
    void like(User*& U) {
        LikedBy.push_back(U->id);
    }
    void InitButtons(User* viewer) {
        if (!ownerAvatarBtn)
            ownerAvatarBtn = new Button((Owner->PfpPath).c_str(), {0, 0}, 1.0f, true);

        if (!likeBtn)
            likeBtn = new Button(IsLikedBy(viewer) ? "assets/utilities/like.png" : "assets/utilities/liked.png", {0, 0}, 0.8f);

        if (Owner == viewer && !lockBtn)
            lockBtn = new Button(Is_Private ? "assets/utilities/lock.png" : "assets/utilities/unlock.png", { 0, 0 }, 0.8f);
    }
    int GetHeight(int width) const {
        if (IsDeleted) return 0;
        const int padding = 10;
        const int pfpSize = 40;
        const int headerHeight = pfpSize + 10 + 22 + 20;
        const int buttonHeight = 40;
        int maxLineWidth = width - padding * 2 - 20;
        int lineWidth = 0;
        int lineCount = 1;
        string word;
        for (char c : Text) {
            if (c == ' ' || c == '\n') {
                int w = MeasureText((word + " ").c_str(), 15);
                if (lineWidth + w > maxLineWidth) {
                    lineCount++;
                    lineWidth = 0;
                }
                lineWidth += w;
                word.clear();
            }
            else {
                word += c;
            }
        }
        if (!word.empty()) lineCount++;
        int textHeight = lineCount * 18;
        int mediaHeight = (Media_Paths.size()) * 22;
        return padding + headerHeight + textHeight + mediaHeight + buttonHeight + padding;
    }

    void DrawPost(int x, int y, int width, User* viewer) {
        if (IsDeleted) return;
        const int padding = 10;
        const int pfpSize = 40;
        const float radius = 0.08f;
        Color borderColor = { 210, 210, 210, 255 };
        Color cardColor = { 248, 248, 248, 255 };
        int cardHeight = GetHeight(width);
        Rectangle card = { (float)x, (float)y, (float)width, (float)cardHeight };
        DrawRectangleRounded(card, radius, 8, cardColor);
        DrawRectangleRoundedLines(card, radius, 8, borderColor);
        //=== Avatar Button ===
        Vector2 avatarPos = { (float)x + padding, (float)y + padding };
        if (ownerAvatarBtn) {
            ownerAvatarBtn->setPosition(avatarPos);
            ownerAvatarBtn->Draw();
        }
        //=== Click to view profile ===
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (ownerAvatarBtn && ownerAvatarBtn->isPressed(mousePos, mousePressed)) {
            if (Owner) Owner->View_Profile(Owner, viewer, GetFontDefault(), 10, 10, 400);
        }
        //=== Owner name and timestamp ===
        if (f.texture.id == 0) {
            LoadFont("assets/static/Roboto_SemiCondensed-BlackItalic.ttf");
        }
        int textX = x + pfpSize + padding * 2;
        int textY = y + padding;
        string ownerText = Owner ? Owner->GetName() : "Unknown";
        DrawTextEx(f, ownerText.c_str(), { (float)textX, (float)textY }, 18, 1, DARKBLUE);
        textY += 22;

        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &TimeStamp);
        string timeString(timeStr);
        if (!timeString.empty()) timeString.pop_back();
        DrawTextEx(f, timeString.c_str(), { (float)textX, (float)textY }, 12, 1, GRAY);
        textY += 20;
        //=== Wrapped post text ===
        const int maxLineWidth = width - padding * 2 - 20;
        string wrappedText, word;
        int lineWidth = 0;
        for (char c : Text) {
            if (c == ' ' || c == '\n') {
                int w = MeasureText((word + " ").c_str(), 15);
                if (lineWidth + w > maxLineWidth) {
                    wrappedText += "\n";
                    lineWidth = 0;
                }
                wrappedText += word + " ";
                lineWidth += w;
                word.clear();
            }
            else {
                word += c;
            }
        }
        if (!word.empty()) wrappedText += word;
        DrawTextEx(f, wrappedText.c_str(), { (float)(x + padding), (float)textY }, 15, 1, BLACK);
        textY += 18 * (wrappedText.length() / maxLineWidth + 1);
        //=== Media info ===
        if (!Media_Paths.empty()) {
            string mediaInfo = to_string(Media_Paths.size()) + " Media file(s)";
            DrawTextEx(f, mediaInfo.c_str(), { (float)x + padding, (float)textY }, 14, 1, DARKGRAY);
            textY += 22;
        }
        //=== Likes and privacy text ===
        string likesText = u8"\U0001F44D " + to_string(LikedBy.size());
        DrawTextEx(f, likesText.c_str(), { (float)x + padding, (float)textY }, 14, 1, MAROON);
        string privacyText = Is_Private ? u8"\U0001F512 Private" : u8"\U0001F310 Public";
        DrawTextEx(f, privacyText.c_str(), { (float)(x + width - 110), (float)textY }, 14, 1,
            Is_Private ? ORANGE : GREEN);
        //=== Like Button ===
        if (likeBtn) {
            Vector2 likePos = { (float)x + padding, (float)y + cardHeight - 35 };
            likeBtn->setPosition(likePos);
            likeBtn->Draw();

            if (likeBtn->isPressed(mousePos, mousePressed)) {
                if (IsLikedBy(viewer)) {
                    Unlike(viewer);
                }
                else {
                    like(viewer);
                }
                delete likeBtn;
                likeBtn = new Button(IsLikedBy(viewer) ? "assets/utilities/liked.png" : "assets/utilities/like.png", likePos, 0.8f);
            }
        }
        //=== Lock Button (owner only) ===
        if (Owner == viewer && lockBtn) {
            Vector2 lockPos = { (float)x + width - 50, (float)y + cardHeight - 35 };
            lockBtn->setPosition(lockPos);
            lockBtn->Draw();

            if (lockBtn->isPressed(mousePos, mousePressed)) {
                Is_Private = !Is_Private;
                delete lockBtn;
                lockBtn = new Button(Is_Private ? "assets/utilities/lock.png" : "assets/utilities/unlock.png", lockPos, 0.8f);
            }
        }
    }


    void Load_Font(string p) {
        f = LoadFont(p.c_str());
    }
    ~Post() {
        if (ownerAvatarBtn) delete ownerAvatarBtn;
        if (likeBtn) delete likeBtn;
        if (lockBtn) delete lockBtn;
    }

};
