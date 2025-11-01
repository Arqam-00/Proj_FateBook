#include "Post_Manager.h"
#include "User.h"

//LOGIC is BAD fix it Quddos

PostManager::PostManager() {}
PostManager::~PostManager() {}
Post* PostManager::CreatePost(User* Owner, const string& Text, const vector<string>& Paths, bool HasImage, bool HasVideo, bool Private)
{
    if (!Owner) return nullptr;
    Post* P = Owner->CreatePost(Text, Paths, HasImage, HasVideo, Private);
    AllPosts_.push_back(P);
    Feed_.AddToFeed(P);
    return P;
}
bool PostManager::MarkPostDeleted(Post* P) //Marking it delete because it could be in the feed
{
    if (!P) return false;
    P->IsDeleted = true;
    return true;
}

void PostManager::RemoveFromAllPosts(Post* P)
{
    for (auto it = AllPosts_.begin(); it != AllPosts_.end(); ++it)
    {
        if (*it == P)
        {
            AllPosts_.erase_at(it);
            return;
        }
    }
}
void PostManager::FinalizeDelete(Post* P) // Delete when removed from The FEEEEEDDD
{
    if (!P) return;
    RemoveFromAllPosts(P);


    delete P;
}

Post* PostManager::GetNextFeedPost()
{
    while (!Feed_.IsEmpty())
    {
        Post* Top = Feed_.PeekTop();
        Feed_.PopTop();
        if (!Top) continue;

        if (Top->IsDeleted)
        {
            if (Top->Owner)
            {
                for (auto it = Top->Owner->Posts.begin(); it != Top->Owner->Posts.end(); ++it)
                {
                    if (*it == Top)
                    {
                        Top->Owner->Posts.erase_at(it);
                        break;

                    }
                }
            }
            FinalizeDelete(Top);
            continue;
        }

        return Top;
    }
    return nullptr;
}

void PostManager::BuildFeedFromAllPosts(DoublyList<User*>& Users)
{
    Feed_.Clear();
    for (auto uit = Users.begin(); uit != Users.end(); ++uit)
    {
        User* U = *uit;
        for (auto pit = U->Posts.begin(); pit != U->Posts.end(); ++pit)
        {
            Post* P = *pit;
            if (!P->IsDeleted) Feed_.AddToFeed(P);
        }
    }
}

DoublyList<Post*> PostManager::GetAllPosts() const { return AllPosts_; }
