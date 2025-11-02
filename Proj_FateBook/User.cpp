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
    PfpPath = "assets/pfps/default.png";
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
    PfpPath = Pfp;
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
void User::View_Profile(User* profileOwner, User* viewer, Font font, int x, int y, int width)
{
    const int padding = 20;
    const int pfpSize = 100;
    const int scrollSpeed = 10;
    static int scrollOffset = 0;
    scrollOffset -= (int)(GetMouseWheelMove() * 40);
    if (IsKeyDown(KEY_DOWN)) scrollOffset += scrollSpeed;
    if (IsKeyDown(KEY_UP))   scrollOffset -= scrollSpeed;
    const int viewHeight = 800 - y - 10;
    if (viewHeight <= 0) return;
    int measureY = y + padding;

    measureY += pfpSize + 10;
    measureY += 40;
    //measureY += 10 + 100 + 20;
    measureY += 10 + 40; 
    if (viewer == profileOwner)
        measureY += (int)profileOwner->GetPendingFriendRequests().size() * 35;
    measureY += 20 + 30;
    measureY += (int)profileOwner->GetFriendsList().size() * 30;
    measureY += 20 + 30;
    for (auto& post : profileOwner->GetPosts()) {
        if (!post->IsVisibleTo(viewer)) continue;
        measureY += post->GetHeight(width - 2 * padding) + 15;
    }

    int totalHeight = measureY - (y + padding);
    if (totalHeight < viewHeight) totalHeight = viewHeight;

    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > totalHeight - viewHeight)
        scrollOffset = totalHeight - viewHeight;
    BeginScissorMode(x, y, width, viewHeight);

    int cursorY = y + padding - scrollOffset;

//
// ============pfp
      if (!IsTextureValid(profileOwner->PfpTexture) && !profileOwner->PfpPath.empty())
        profileOwner->PfpTexture = LoadTexture(profileOwner->PfpPath.c_str());

    Texture2D pfp = profileOwner->PfpTexture;
    Vector2 pfpPos = { (float)x + width / 2 - pfpSize / 2, (float)cursorY };
    if (IsTextureValid(pfp))
        DrawTexturePro(pfp, { 0, 0, (float)pfp.width, (float)pfp.height },
            { pfpPos.x, pfpPos.y, (float)pfpSize, (float)pfpSize }, { 0, 0 }, 0, WHITE);
    else
        DrawCircleLines(pfpPos.x + pfpSize / 2, pfpPos.y + pfpSize / 2, pfpSize / 2, GRAY);
    cursorY += pfpSize + 10;
    string name = profileOwner->GetName();
    float nameW = MeasureTextEx(font, name.c_str(), 28, 1).x;
    DrawTextEx(font, name.c_str(), { (float)x + width / 2 - nameW / 2, (float)cursorY }, 28, 1, DARKBLUE);
    cursorY += 40;
    DrawLine(x + padding, cursorY, x + width - padding, cursorY, Fade(GRAY, 0.3f));
    cursorY += 10;

    //===========user info-------------------
    Rectangle info = { (float)x + padding, (float)cursorY, (float)(width - 2 * padding), 100 };
    DrawRectangleRounded(info, 0.05f, 8, Fade(LIGHTGRAY, 0.2f));
    string s = (u8"\U0001F464 Gender: " + profileOwner->Gender);
    DrawTextEx(font, s.c_str(), { info.x + 10, info.y + 10 }, 20, 1, GRAY);
    DrawTextEx(font, (u8"\U0001F4C5 Age: " + std::to_string(profileOwner->Age)).c_str(), { info.x + 200, info.y + 10 }, 20, 1, GRAY);
    if (viewer == profileOwner || profileOwner->IsFriend(viewer))
        DrawTextEx(font, (u8"\U0001F4CD Location: " + profileOwner->Location).c_str(), { info.x + 10, info.y + 40 }, 20, 1, GRAY);
    if (viewer == profileOwner)
        DrawTextEx(font, (u8"\U0001F4E7 Email: " + profileOwner->GetEmail()).c_str(), { info.x + 10, info.y + 70 }, 20, 1, GRAY);

    cursorY += info.height + 20;

    //===============friend
    DrawLine(x + padding, cursorY, x + width - padding, cursorY, Fade(GRAY, 0.3f));
    cursorY += 10;
    DrawTextEx(font, (u8"\U0001F465 Friends: " + std::to_string(profileOwner->GetFriendCount())).c_str(), { (float)x + padding, (float)cursorY }, 20, 1, DARKGRAY);
    cursorY += 40;

    //==============posts
    DrawTextEx(font, "Posts", { (float)x + padding, (float)cursorY }, 22, 1, BLACK);
    cursorY += 30;
    for (auto& post : profileOwner->GetPosts()) {
        if (!post->IsVisibleTo(viewer)) continue;
        int postH = post->GetHeight(width - 2 * padding);
        Rectangle card = { (float)x + padding, (float)cursorY, (float)(width - 2 * padding), (float)(postH + 20) };
        DrawRectangleRounded(card, 0.05f, 8, Fade(LIGHTGRAY, 0.3f));
        DrawRectangleRoundedLines(card, 0.05f, 8, Fade(GRAY, 0.2f));
        post->DrawPost((int)card.x + 10, (int)card.y + 10, (int)card.width - 20, viewer);
        cursorY += card.height + 15;
    }
    EndScissorMode();
}
void User::SetID(int _id) { id = _id; }
