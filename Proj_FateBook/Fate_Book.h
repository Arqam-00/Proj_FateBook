#pragma once
#include "DoublyList.h"
#include "User.h"
#include "Post_Manager.h"
//#include "File_Manager.h"
using namespace std;

class Fate_Book
{
    DoublyList<User*> Users_;
    PostManager PostMgr_;
    //File_Manager Storage_;
public:
    Fate_Book();
    ~Fate_Book();
    bool Initialize();
    User* CreateUser(const string& Name, const string& Email, const string& Password, const string& Location, char Gender, int Age, bool IsPublic, const Date& CreatedAt);
    bool DeleteUser(User* U);
    User* FindUserByEmail(const string& Email) const;
    DoublyList<User*>* GetUsers();
    PostManager* GetPostManager();
    //File_Manager* GetStorageManager();

};
