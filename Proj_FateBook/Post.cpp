#include "Post.h"

int Post::NextPostID = 1;
Post::Post()
{
    Owner = nullptr;
    Text = "";
    OwnerPfp = {0};
    HasImage = false;
    HasVideo = false;
    Is_Private = false;
    IsDeleted = false;
    TimeStamp = time(0);
    LikeCount = 0;
    f;
}

Post::Post(User* OwnerPtr, const string& TextContent, bool Private)
{
    Owner = OwnerPtr;
    Text = TextContent;
    OwnerPfp = {0};
    HasImage = false;
    HasVideo = false;
    Is_Private = Private;
    IsDeleted = false;
    TimeStamp = time(0);
    LikeCount = 0;
    PostID = NextPostID++;
}

void Post::addImage(string Path) {
    Paths.push_back(Path);
    HasImage = true;
}

void Post::addVideo(string Path) {
    Paths.push_back(Path);
    HasVideo = true;
}

void Post::DrawPost(int x, int y, int width, User* viewer)
{
    if (IsDeleted) return;
    const int height = 220;
    const float radius = 0.08f;
    const int padding = 10;
    const int pfpSize = 50;
    Color bgColor = Fade(BLACK, 0.05f);
    Color borderColor = { 200, 200, 200, 255 };
    Color cardColor = { 250, 250, 250, 255 };
    DrawRectangleRounded({ (float)x, (float)y, (float)width, (float)height }, radius, 8, cardColor);
    DrawRectangleRoundedLines({ (float)x, (float)y, (float)width, (float)height }, radius, 4, borderColor);
    Vector2 pfpPos = { (float)x + padding, (float)y + padding };
    if (OwnerPfp.id != 0) {
        DrawTexturePro(OwnerPfp,
            { 0, 0, (float)OwnerPfp.width, (float)OwnerPfp.height },
            { pfpPos.x, pfpPos.y, (float)pfpSize, (float)pfpSize },
            { 0, 0 }, 0, WHITE);
    }
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointCircle(mousePos, { pfpPos.x + pfpSize / 2, pfpPos.y + pfpSize / 2 }, pfpSize / 2) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (Owner) Owner->View_Profile(viewer);
    }
    int textX = x + padding + pfpSize + 10;
    int textY = y + padding;
    string ownerText = Owner ? Owner->GetName() : "Unknown";
    if (f.texture.id != 0) DrawTextEx(f, ownerText.c_str(), { (float)textX, (float)textY }, 20, 1, DARKBLUE);
    textY += 24;
    char timeStr[26];
    ctime_s(timeStr, sizeof(timeStr), &TimeStamp);
    string timeString(timeStr);
    timeString.pop_back();
    DrawTextEx(f, timeString.c_str(), { (float)textX, (float)textY }, 14, 1, GRAY);
    textY += 22;
    DrawTextEx(f, Text.c_str(), { (float)textX, (float)textY }, 16, 1, BLACK);
    textY += 40;
    if (HasImage || HasVideo) {
        string mediaInfo = "";
        if (HasImage) mediaInfo += " " + to_string(CountImages()) + " Images ";
        if (HasVideo) mediaInfo += " " + to_string(CountVideos()) + " Videos";
        DrawTextEx(f, mediaInfo.c_str(), { (float)textX, (float)textY }, 16, 1, DARKGRAY);
        textY += 25;
    }
    string likesText = " " + to_string(LikeCount) + " Likes";
    DrawTextEx(f, likesText.c_str(), { (float)textX, (float)textY }, 16, 1, MAROON);
    string privacyText = " " + string(Is_Private ? "Private" : "Public");
    DrawTextEx(f, privacyText.c_str(), { (float)textX + 200, (float)textY }, 14, 1, Is_Private ? ORANGE : GREEN);
}
