#pragma once
#include <string>
#include "DoublyList.h"
#include "Post.h"
#include "Feed_Manager.h"
using namespace std;

class PostManager
{
    DoublyList<Post*> AllPosts_;
    Feed_Manager Feed_;
    void RemoveFromAllPosts(Post* P);
    void FinalizeDelete(Post* P);

public:
    PostManager();
    ~PostManager();
    Post* CreatePost(User* Owner, const string& Text, const vector<string>& Paths, bool HasImage, bool HasVideo, bool Private);
    bool MarkPostDeleted(Post* P);

    Post* GetNextFeedPost();
    void BuildFeedFromAllPosts(DoublyList<User*>& Users);
    DoublyList<Post*> GetAllPosts() const;


};
