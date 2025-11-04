#include "Post.h"
#include <iostream>
using namespace std;
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
    const int height = 260;
    const float radius = 0.08f;
    const int padding = 10;
    const int pfpSize = 40;
    Color borderColor = { 210, 210, 210, 255 };
    Color cardColor = { 248, 248, 248, 255 };

    DrawRectangleRounded({ (float)x, (float)y, (float)width, (float)height }, radius, 8, cardColor);
    DrawRectangleRoundedLines({ (float)x, (float)y, (float)width, (float)height }, radius, 8, borderColor);
    Vector2 mousePos = GetMousePosition();
    Vector2 pfpPos = { (float)x + padding, (float)y + padding };
    if (OwnerPfp.id != 0) {
        DrawTexturePro(
            OwnerPfp,
            { 0, 0, (float)OwnerPfp.width, (float)OwnerPfp.height },
            { pfpPos.x, pfpPos.y, (float)pfpSize, (float)pfpSize },
            { 0, 0 },
            0,
            WHITE
        );
    }
    if (CheckCollisionPointRec(mousePos, { pfpPos.x, pfpPos.y, 1.0f*pfpSize, 1.0f * pfpSize }) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (Owner) Owner->View_Profile(Owner,viewer, GetFontDefault(), 10, 10, 400);

    }
    int textX = x + pfpSize + padding * 2;
    int textY = y + padding;
    string ownerText = Owner ? Owner->GetName() : "Unknown";
    DrawTextEx(f, ownerText.c_str(), { (float)textX, (float)textY }, 18, 1, DARKBLUE);
    textY += 22;

    char timeStr[26];
    ctime_s(timeStr, sizeof(timeStr), &TimeStamp);
    string timeString(timeStr);
    if (!timeString.empty()) timeString.pop_back();
    DrawTextEx(f, timeString.c_str(), { (float)textX, (float)textY }, 12, 1, GRAY);
    textY += 20;

    //======Post
    const int textWidth = width - (padding * 2);
    const int maxLineWidth = textWidth - 20;
    string wrappedText;
    string word;
    int lineWidth = 0;
    for (char c : Text) {
        if (c == ' ' || c == '\n') {
            int w = MeasureText((word + " ").c_str(), 15);
            if (lineWidth + w > maxLineWidth) {
                wrappedText += "\n";
                lineWidth = 0;
            }
            wrappedText += word + " ";
            lineWidth += w;
            word.clear();
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) wrappedText += word;
    DrawTextEx(f, wrappedText.c_str(), { (float)(x + padding), (float)textY }, 15, 1, BLACK);
    textY += 60;
    //=====Media
    if (HasImage || HasVideo) {
        string mediaInfo;
        if (HasImage) mediaInfo += to_string(CountImages()) + " Image(s)  ";
        if (HasVideo) mediaInfo += to_string(CountVideos()) + " Video(s)";
        DrawTextEx(f, mediaInfo.c_str(), { (float)x + padding, (float)textY }, 14, 1, DARKGRAY);
        textY += 22;
    }
    string likesText = "Likes " + to_string(LikeCount);
    DrawTextEx(f, likesText.c_str(), { (float)x + padding, (float)textY }, 14, 1, MAROON);
    string privacyText = Is_Private ? "Private" : "Public";
    DrawTextEx(f, privacyText.c_str(), { (float)(x + width - 90), (float)textY }, 14, 1,
        Is_Private ? ORANGE : GREEN);
}
