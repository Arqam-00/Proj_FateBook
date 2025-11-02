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
    User* current_user;
    //File_Manager Storage_;
    int user_id_counter;
public:
    Fate_Book();
    ~Fate_Book();
    bool Initialize();
    User* CreateUser(const string& Name, const string& Email, const string& Password, const string& Location,
        char Gender, int Age, bool IsPublic, const Date& CreatedAt);
    DoublyList<User*>* GetUsers();
    PostManager* GetPostManager();
    void Signup();
    void Login();
    void Logout();
    void DeleteUser();
    User* Check_By_Email(const string& email);
    User* Check(const string& email, const string& password);
    void OpenFeed();

    void load_from_file(const string& filename);
    void write_into_file(const string& filename);
};