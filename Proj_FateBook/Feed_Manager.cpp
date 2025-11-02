#include "Feed_Manager.h"

Feed_Manager::Feed_Manager(User* viewerUser, int x, int y, int width, int height)
    : viewer(viewerUser), scrollOffset(0), feedX(x), feedY(y), feedWidth(width), feedHeight(height) {
}

void Feed_Manager::Add(Post* post) {
    feedHeap.push(post);
    feedList.push_back(post);
}

void Feed_Manager::Clear() {
    while (!feedHeap.empty()) feedHeap.pop();
    feedList.clear();
}

bool Feed_Manager::empty() const {
    return feedHeap.empty();
}

size_t Feed_Manager::Size() const {
    return feedList.size();
}

void Feed_Manager::Update() {
    if (IsKeyDown(KEY_DOWN)) scrollOffset -= 10;
    if (IsKeyDown(KEY_UP)) scrollOffset += 10;
}
Post* Feed_Manager::PeekTop() const { 
    if (feedHeap.empty()) return nullptr; return feedHeap.top(); }
void Feed_Manager::PopTop() { if (!feedHeap.empty()) feedHeap.pop(); }
void Feed_Manager::Draw() {
    BeginScissorMode(feedX, feedY, feedWidth, feedHeight);
    float yOffset = feedY + scrollOffset;
    for (Post* post : feedList) {
        if (post->IsVisibleTo(viewer)) {
            post->DrawPost(feedX + 10, (int)yOffset, feedWidth - 20, viewer);
            yOffset += post->GetHeight(feedWidth) + 15;
        }
    }

    EndScissorMode();
}
