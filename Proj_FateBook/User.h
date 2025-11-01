#pragma once
#include <string>
#include <vector>
#include "Date.h"
#include "DoublyList.h"
#include "Post.h"
using namespace std;
//Quddos,Fix the Feed logic for posts and Make the File_Manger to read from and Update files
//You can do it i m counting on You PARTTNER!!
struct FriendRequest;

class User
{
    DoublyList<User*> Friends;
    vector<FriendRequest*> FriendRequests;
    string GetCurrentTimeStamp() const;
public:
    DoublyList<Post*> Posts;

    string Name;
    string Email;
    string Password;
    string Location;
    char Gender;
    int Age;
    bool IsPublic;
    Date CreatedAt;



    User();
    User(const string& Name,
        const string& Email,
        const string& Password,
        const string& Location,
        char Gender,
        int Age,
        bool IsPublic,
        const Date& CreatedAt);
    ~User();
    string GetName() const;
    string GetEmail() const;
    bool CheckPassword(const string& Password) const;
    void SetPassword(const string& NewPassword);
    bool AddFriendRequest(User* FromUser);
    bool AcceptFriendRequest(FriendRequest* Request);
    bool DeclineFriendRequest(User* FromUser);
    vector<FriendRequest*> GetPendingFriendRequests() const;
    bool HasPendingRequestFrom(User* FromUser) const;
    void AddFriend(User* FriendUser);
    bool RemoveFriend(User* FriendUser);
    bool IsFriend(User* Other) const;
    int GetFriendCount() const;
    DoublyList<User*> GetFriendsList() const;
    Post* CreatePost(const string& Text, const vector<string>& Paths, bool HasImage, bool HasVideo, bool Private);
    bool DeletePost(Post* P);
    DoublyList<Post*> GetPosts() const;
    int GetPostCount() const;


};
