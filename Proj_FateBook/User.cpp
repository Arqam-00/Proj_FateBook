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

int User::NextID = 1;

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
    id = NextID++;
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
    int scrollSpeed = 10;
    static int scrollOffset = 0; // Make static to persist between frames

    // Handle scrolling input
    scrollOffset += (GetMouseWheelMove() * 30);
    if (IsKeyDown(KEY_DOWN)) scrollOffset -= scrollSpeed;
    if (IsKeyDown(KEY_UP)) scrollOffset += scrollSpeed;

    // Constrain scroll offset to prevent scrolling past content
    scrollOffset = scrollOffset; // We'll add constraints later

    BeginScissorMode(x, y, width, GetScreenHeight());

    // Apply scroll offset to the starting Y position
    int cursorY = y + padding - scrollOffset;

    // Profile Picture
    if (!IsTextureValid(profileOwner->PfpTexture)) {
        profileOwner->PfpTexture = LoadTexture(profileOwner->PfpPath.c_str());
    }
    Texture2D pfp = profileOwner->PfpTexture;

    Vector2 pfpPos = { (float)x + width / 2 - pfpSize / 2, (float)cursorY };
    DrawCircle(pfpPos.x + pfpSize / 2, pfpPos.y + pfpSize / 2, pfpSize / 2 + 2, Fade(GRAY, 0.3f));
    DrawTexturePro(pfp, { 0, 0, (float)pfp.width, (float)pfp.height }, { pfpPos.x, pfpPos.y, (float)pfpSize, (float)pfpSize }, { 0, 0 }, 0, WHITE);
    cursorY += pfpSize + 10;

    // Name
    std::string nameStr = profileOwner->GetName();
    float nameWidth = MeasureTextEx(font, nameStr.c_str(), 28, 1).x;
    DrawTextEx(font, nameStr.c_str(), { (float)x + width / 2 - nameWidth / 2, (float)cursorY }, 28, 1, DARKBLUE);
    cursorY += 40;

    DrawLine(x + padding, cursorY, x + width - padding, cursorY, Fade(GRAY, 0.3f));
    cursorY += 10;

    // Info Box
    Rectangle infoBox = { (float)x + padding, (float)cursorY, (float)(width - 2 * padding), 100 };
    DrawRectangleRounded(infoBox, 0.05f, 8, Fade(LIGHTGRAY, 0.2f));
    string s = (u8"\U0001F464 Gender: " + profileOwner->Gender);
    DrawTextEx(font, s.c_str(), { infoBox.x + 10, infoBox.y + 10 }, 20, 1, GRAY);
    DrawTextEx(font, (u8"\U0001F4C5 Age: " + std::to_string(profileOwner->Age)).c_str(), { infoBox.x + 200, infoBox.y + 10 }, 20, 1, GRAY);
    if (viewer == profileOwner || profileOwner->IsFriend(viewer)) {
        DrawTextEx(font, (u8"\U0001F4CD Location: " + profileOwner->Location).c_str(), { infoBox.x + 10, infoBox.y + 40 }, 20, 1, GRAY);
    }
    if (viewer == profileOwner) {
        DrawTextEx(font, (u8"\U0001F4E7 Email: " + profileOwner->GetEmail()).c_str(), { infoBox.x + 10, infoBox.y + 70 }, 20, 1, GRAY);
    }
    cursorY += infoBox.height + 20;

    // Friend Status
    DrawLine(x + padding, cursorY, x + width - padding, cursorY, Fade(GRAY, 0.3f));
    cursorY += 10;
    DrawTextEx(font, (u8"\U0001F465 Friends: " + std::to_string(profileOwner->GetFriendCount())).c_str(), { (float)x + padding, (float)cursorY }, 20, 1, DARKGRAY);

    if (viewer != profileOwner) {
        if (profileOwner->IsFriend(viewer)) {
            DrawTextEx(font, u8"\U00002705 You are friends", { (float)x + 250, (float)cursorY }, 20, 1, GREEN);
        }
        else if (profileOwner->HasPendingRequestFrom(viewer)) {
            DrawTextEx(font, u8"\U0001F4E9 Friend request sent", { (float)x + 250, (float)cursorY }, 20, 1, ORANGE);
        }
        else {
            Rectangle btn = { (float)x + 250, (float)cursorY, 160, 30 };
            DrawRectangleRounded(btn, 0.2f, 5, BLUE);
            DrawTextEx(font, u8"\U0001F91D Send Friend Request", { btn.x + 10, btn.y + 5 }, 16, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                viewer->AddFriendRequest(profileOwner);
            }
        }
    }
    cursorY += 40;

    // Friend Requests
    if (viewer == profileOwner) {
        auto requests = profileOwner->GetPendingFriendRequests();
        for (auto& req : requests) {
            std::string reqText = u8"\U0001F464 Request from: " + req->From->GetName();
            DrawTextEx(font, reqText.c_str(), { (float)x + padding, (float)cursorY }, 18, 1, DARKGRAY);

            Rectangle acceptBtn = { (float)x + 300, (float)cursorY, 60, 25 };
            DrawRectangleRounded(acceptBtn, 0.2f, 5, GREEN);
            DrawTextEx(font, u8"\U00002705 Accept", { acceptBtn.x + 5, acceptBtn.y + 5 }, 14, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), acceptBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                profileOwner->AcceptFriendRequest(req);
            }

            Rectangle rejectBtn = { (float)x + 370, (float)cursorY, 60, 25 };
            DrawRectangleRounded(rejectBtn, 0.2f, 5, RED);
            DrawTextEx(font, u8"\U0000274C Reject", { rejectBtn.x + 5, rejectBtn.y + 5 }, 14, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), rejectBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                profileOwner->DeclineFriendRequest(req->From);
            }

            cursorY += 35;
        }
    }

    // Friend List
    cursorY += 20;
    DrawTextEx(font, "Friends", { (float)x + padding, (float)cursorY }, 22, 1, BLACK);
    cursorY += 30;

    for (User* friendUser : profileOwner->GetFriendsList()) {
        std::string friendName = friendUser->GetName();
        DrawTextEx(font, friendName.c_str(), { (float)x + padding, (float)cursorY }, 18, 1, DARKGRAY);

        Rectangle unfriendBtn = { (float)x + width - 100, (float)cursorY, 80, 25 };
        DrawRectangleRounded(unfriendBtn, 0.2f, 5, RED);
        DrawTextEx(font, "Unfriend", { unfriendBtn.x + 10, unfriendBtn.y + 5 }, 14, 1, WHITE);
        if (CheckCollisionPointRec(GetMousePosition(), unfriendBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            profileOwner->RemoveFriend(friendUser);
        }

        cursorY += 30;
    }

    // Posts
    cursorY += 20;
    DrawTextEx(font, u8"\U0001F4DD Posts", { (float)x + padding, (float)cursorY }, 22, 1, BLACK);
    cursorY += 30;
    for (auto& post : profileOwner->GetPosts()) {
        if (!post->IsVisibleTo(viewer)) continue;
        int postHeight = post->GetHeight(width - 2 * padding);
        Rectangle card = { (float)x + padding, (float)cursorY, (float)(width - 2 * padding), (float)(postHeight + 20) };
        DrawRectangleRounded(card, 0.05f, 8, Fade(LIGHTGRAY, 0.3f));
        DrawRectangleRoundedLines(card, 0.05f, 8, Fade(GRAY, 0.2f));
        post->DrawPost((int)card.x + 10, (int)card.y + 10, (int)card.width - 20, viewer);
        if (viewer == profileOwner) {
            Rectangle delBtn = { card.x + card.width - 90, card.y + card.height - 35, 80, 25 };
            DrawRectangleRounded(delBtn, 0.2f, 5, RED);
            DrawTextEx(font, u8"\U0001F5D1 Delete", { delBtn.x + 10, delBtn.y + 5 }, 14, 1, WHITE);
            if (CheckCollisionPointRec(GetMousePosition(), delBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                profileOwner->DeletePost(post);
            }
        }
        cursorY += card.height + 15;
    }
    int totalContentHeight = cursorY - (y + padding) + scrollOffset;
    int screenHeight = GetScreenHeight();
    if (totalContentHeight < screenHeight) {
        scrollOffset = 0;
    }
    else {
        scrollOffset = Clamp(scrollOffset, 0, totalContentHeight - screenHeight + padding);
    }

    //UnloadTexture(pfp);
    EndScissorMode();
}

void User::SetID(int _id) { id = _id; }