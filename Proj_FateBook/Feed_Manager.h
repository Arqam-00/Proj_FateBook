#pragma once
#include <vector>
#include <queue>
#include "Post.h"
#include "User.h"
#include <raylib.h>
using namespace std;
class Feed_Manager {
private:
    priority_queue<Post*> feedHeap;
    vector<Post*> feedList;
    User* viewer;

    float scrollOffset;
    int feedX, feedY, feedWidth, feedHeight;

public:
    Feed_Manager(User* viewerUser, int x, int y, int width, int height);
    Feed_Manager() {}
    void Add(Post* post);
    void Clear();
    bool empty() const;
    size_t Size() const;
    Post* PeekTop() const;
    void PopTop();
    void Update();
    void Draw();
};
