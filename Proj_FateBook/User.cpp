#include "User.h"
#include "Post.h"
#include <ctime>

struct FriendRequest
{
    User* From;
    string TimeStamp;       //To store when the request was sent
    bool IsAccepted;
    bool IsRejected;
    FriendRequest() { From = nullptr; TimeStamp = ""; IsAccepted = false; IsRejected = false; }
};
User::User()
{
    Name = "";
    Email = "";
    Password = "";
    Location = "";
    Gender = 'U';
    Age = 0;
    IsPublic = true;
    CreatedAt = Date();
}

User::User(const string& NameStr,
    const string& EmailStr,
    const string& PasswordStr,
    const string& LocationStr,
    char GenderChar,
    int AgeInt,
    bool IsPublicBool,
    const Date& CreatedAtDate)
{
    Name = NameStr;
    Email = EmailStr;
    Password = PasswordStr;
    Location = LocationStr;
    Gender = GenderChar;
    Age = AgeInt;
    IsPublic = IsPublicBool;
    CreatedAt = CreatedAtDate;
}
User::~User()
{
    for (auto it = Posts.begin(); it != Posts.end(); ++it)
    {
        delete* it;
    }
    for (auto it = FriendRequests.begin(); it != FriendRequests.end(); ++it)
    {
        delete* it;
    }
}
string User::GetName() const { return Name; }
string User::GetEmail() const { return Email; }
bool User::CheckPassword(const string& Pwd) const { return Password == Pwd; }
void User::SetPassword(const string& NewPassword) { Password = NewPassword; }
bool User::AddFriendRequest(User* FromUser)     //Controlled by Fake_Book
{
    if (!FromUser || FromUser == this) return false;
    for (auto r : FriendRequests)
    {
        if (r->From == FromUser && !r->IsRejected && !r->IsAccepted) return false;
    }
    FriendRequest* FR = new FriendRequest();
    FR->From = FromUser;
    time_t t = time(0);
    FR->TimeStamp = to_string(t);
    FR->IsAccepted = false;
    FR->IsRejected = false;
    FriendRequests.push_back(FR);
    return true;
}
bool User::AcceptFriendRequest(FriendRequest* Request)  //When a user presses Accept on a friend request
{
    if (!Request) return false;
    if (Request->IsAccepted || Request->IsRejected) return false;
    Request->IsAccepted = true;
    AddFriend(Request->From);
    Request->From->AddFriend(this);
    return true;
}
bool User::DeclineFriendRequest(User* FromUser)  //When a user declines one,cmon the names make it easy to understand yet you read the whole comment ,huh Quddos?

{
    for (int i = 0; i < FriendRequests.size(); i++)
    {
        if (FriendRequests[i]->From == FromUser && !FriendRequests[i]->IsRejected && !FriendRequests[i]->IsAccepted)
        {
            FriendRequests[i]->IsRejected = true;
            return true;
        }
    }
    return false;
}

vector<FriendRequest*> User::GetPendingFriendRequests() const  //Do i even  have to explain this one?
{
    vector<FriendRequest*> R;
    for (auto r : FriendRequests)
    {
        if (!r->IsAccepted && !r->IsRejected) R.push_back(r);
    }
    return R;
}
bool User::HasPendingRequestFrom(User* FromUser) const
{
    for (auto r : FriendRequests)
    {
        if (r->From == FromUser && !r->IsAccepted && !r->IsRejected) return true;
    }
    return false;
}
void User::AddFriend(User* FriendUser)
{
    if (!FriendUser || FriendUser == this) return;
    for (auto it = Friends.begin(); it != Friends.end(); ++it)
    {
        if (*it == FriendUser) return;
    }
    Friends.push_back(FriendUser);
}
bool User::RemoveFriend(User* FriendUser)
{
    for (auto it = Friends.begin(); it != Friends.end(); ++it)
    {
        if (*it == FriendUser)
        {
            Friends.erase_at(it);
            return true;
        }
    }
    return false;
}

bool User::IsFriend(User* Other) const
{
    for (auto it = Friends.begin(); it != Friends.end(); ++it)
    {
        if (*it == Other) return true;
    }
    return false;
}
int User::GetFriendCount() const { return Friends.size(); }
DoublyList<User*> User::GetFriendsList() const { return Friends; }

Post* User::CreatePost(const string& Text, const vector<string>& Paths, bool HasImage, bool HasVideo, bool Private)
{
    Post* P = new Post(this, Text, Private);
    P->HasImage = HasImage;

    P->HasVideo = HasVideo;
    P->Paths = Paths;
    Posts.push_back(P);
    return P;
}
bool User::DeletePost(Post* P)
{
    if (!P) return false;
    for (auto it = Posts.begin(); it != Posts.end(); ++it)
    {
        if (*it == P)
        {
            Posts.erase_at(it);
            P->IsDeleted = true;
            return true;
        }
    }
    return false;
}

DoublyList<Post*> User::GetPosts() const { return Posts; }
int User::GetPostCount() const { return Posts.size(); }
string User::GetCurrentTimeStamp() const
{
    time_t t = time(0);
    return to_string(t);
}
