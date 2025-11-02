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
    PfpPath = "";
    Name = "";
    Email = "";
    Password = "";
    Location = "";
    Gender = 'U';
    Age = 0;
    IsPublic = true;
    CreatedAt = Date();
}

User::User(
    const string& Pfp,
    const string& NameStr,
    const string& EmailStr,
    const string& PasswordStr,
    const string& LocationStr,
    char GenderChar,
    int AgeInt,
    bool IsPublicBool,
    const Date& CreatedAtDate)
{
    PfpPath=Pfp;
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
bool User::CheckPassword(const string& _Password) const { return Password == _Password; }
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

Post* User::CreatePost(const string& _Text, const vector<string>& _Paths, bool _HasImage, bool _HasVideo, bool _Private)
{
    Post* P = new Post(this, _Text, _Private);
    P->HasImage = _HasImage;

    P->HasVideo = _HasVideo;
    P->Paths = _Paths;
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
void View_Profile(User* profileOwner, User* viewer, Font font, int x, int y, int width) {
    const int padding = 20;
    const int pfpSize = 100;
    int cursorY = y + padding;
    Texture2D pfp = LoadTexture(profileOwner->PfpPath.c_str());
    DrawTextureEx(pfp, { (float)x + padding, (float)cursorY }, 0, (float)pfpSize / pfp.width, WHITE);
    DrawTextEx(font, profileOwner->GetName().c_str(), { (float)x + pfpSize + 2 * padding, (float)cursorY }, 28, 1, DARKBLUE);
    cursorY += pfpSize + padding;
    string genderStr = string("Gender: ") + profileOwner->Gender;
    string ageStr = "Age: " + to_string(profileOwner->Age);
    DrawTextEx(font, genderStr.c_str(), { (float)x + padding, (float)cursorY }, 20, 1, GRAY);
    DrawTextEx(font, ageStr.c_str(), { (float)x + padding + 200, (float)cursorY }, 20, 1, GRAY);
    cursorY += 30;
    if (viewer == profileOwner || profileOwner->IsFriend(viewer)) {
        string locStr = "Location: " + profileOwner->Location;
        DrawTextEx(font, locStr.c_str(), { (float)x + padding, (float)cursorY }, 20, 1, GRAY);
        cursorY += 30;
    }
    if (viewer == profileOwner) {
        string emailStr = "Email: " + profileOwner->GetEmail();
        DrawTextEx(font, emailStr.c_str(), { (float)x + padding, (float)cursorY }, 20, 1, GRAY);
        cursorY += 30;
    }
    string friendCount = "Friends: " + to_string(profileOwner->GetFriendCount());
    DrawTextEx(font, friendCount.c_str(), { (float)x + padding, (float)cursorY }, 20, 1, DARKGRAY);
    if (viewer != profileOwner) {
        if (profileOwner->IsFriend(viewer)) {
            DrawTextEx(font, "You are friends", { (float)x + 250, (float)cursorY }, 20, 1, GREEN);
        }
        else if (profileOwner->HasPendingRequestFrom(viewer)) {
            DrawTextEx(font, "Friend request sent", { (float)x + 250, (float)cursorY }, 20, 1, ORANGE);
        }
        else {
            Rectangle btn = { (float)x + 250, (float)cursorY, 160, 30 };
            DrawRectangleRounded(btn, 0.2f, 5, BLUE);
            DrawTextEx(font, "Send Friend Request", { btn.x + 10, btn.y + 5 }, 16, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                viewer->AddFriendRequest(profileOwner);
            }
        }
    }
    cursorY += 40;
    if (viewer == profileOwner) {
        auto requests = profileOwner->GetPendingFriendRequests();
        for (auto& req : requests) {
            string reqText = "Request from: " + req->From->GetName();
            DrawTextEx(font, reqText.c_str(), { (float)x + padding, (float)cursorY }, 18, 1, DARKGRAY);
            Rectangle acceptBtn = { (float)x + 300, (float)cursorY, 60, 25 };
            DrawRectangleRounded(acceptBtn, 0.2f, 5, GREEN);
            DrawTextEx(font, "Accept", { acceptBtn.x + 5, acceptBtn.y + 5 }, 14, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), acceptBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                profileOwner->AcceptFriendRequest(req);
            }
            Rectangle rejectBtn = { (float)x + 370, (float)cursorY, 60, 25 };
            DrawRectangleRounded(rejectBtn, 0.2f, 5, RED);
            DrawTextEx(font, "Reject", { rejectBtn.x + 5, rejectBtn.y + 5 }, 14, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), rejectBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                profileOwner->DeclineFriendRequest(req->From);
            }
            cursorY += 35;
        }
    }
    cursorY += 20;
    DrawTextEx(font, "Posts:", { (float)x + padding, (float)cursorY }, 22, 1, BLACK);
    cursorY += 30;

    auto posts = profileOwner->GetPosts();
    for (auto& post : posts) {
        if (post->IsVisibleTo(viewer)) {
            post->DrawPost(x + padding, cursorY, width - 2 * padding, viewer);
            if (viewer == profileOwner) {
                Rectangle delBtn = { (float)x + width - 100, (float)cursorY + 10, 80, 25 };
                DrawRectangleRounded(delBtn, 0.2f, 5, RED);
                DrawTextEx(font, "Delete", { delBtn.x + 10, delBtn.y + 5 }, 14, 1, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), delBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    profileOwner->DeletePost(post);
                }
            }
            cursorY += post->GetHeight(width) + 20;
        }
    }
    UnloadTexture(pfp);
}

void User::SetID(int _id) { id = _id; }
