#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "User.h"
#include <raylib.h>
using namespace std;

struct Post
{
    User* Owner;
    string Text;
    Font f;
    Texture2D OwnerPfp;
    bool operator < (Post& other) {
        return this->TimeStamp < other.TimeStamp;
    }
    bool HasImage;
    bool HasVideo;
    bool Is_Private;
    bool IsDeleted;
    vector<string> Paths;
    time_t TimeStamp;
    int LikeCount;
    Post();
    Post(User* OwnerPtr, const string& TextContent, bool Private);
    void addImage(string Path);
    void addVideo(string Path);
    bool IsVisibleTo(const User* viewer) const {
        if (IsDeleted) return false;
        if (!Is_Private) return true;
        return viewer && Owner && Owner->IsFriend(const_cast<User*>(viewer));
    }

    int CountImages() const {
        int count = 0;
        for (const auto& path : Paths) {
            if (path.find(".jpg") != string::npos ||
                path.find(".png") != string::npos ||
                path.find(".jpeg") != string::npos ||
                path.find(".gif") != string::npos) {
                count++;
            }
        }
        return count;
    }

    int CountVideos() const {
        int count = 0;
        for (const auto& path : Paths) {
            if (path.find(".mp4") != string::npos ||
                path.find(".avi") != string::npos ||
                path.find(".mov") != string::npos ||
                path.find(".mkv") != string::npos) {
                count++;
            }
        }
        return count;
    }
    int GetHeight(int width) const {
        if (IsDeleted) return 0;
        return 200;
    }
    void DrawPost(int x, int y, int width, User* viewer);
    void Load_Font(string p) {
        f = LoadFont(p.c_str());
    }
};
