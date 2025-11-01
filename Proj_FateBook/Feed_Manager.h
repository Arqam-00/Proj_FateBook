#pragma once
#include <queue>
#include <vector>
#include "Post.h"
using namespace std;

#include "Post.h"
class FeedManager{

    priority_queue<Post*, vector<Post*>> FeedHeap_;
public:
    FeedManager();
    ~FeedManager();
    void AddToFeed(Post* P);
    bool IsEmpty() const;
    Post* PeekTop() const;
    void PopTop();
    void Clear();
    size_t GetSize() const;
};
