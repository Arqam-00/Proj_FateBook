#pragma once
#include <string>
#include <vector>
#include <ctime>
using namespace std;

struct User;

struct Post
{
    User* Owner;
    string Text;

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
};
