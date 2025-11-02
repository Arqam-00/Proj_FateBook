#include "Post_Manager.h"
#include "User.h"

PostManager::PostManager() {}
PostManager::~PostManager() {}

Post* PostManager::CreatePost(User* Owner, const string& Text, const vector<string>& Paths, bool HasImage, bool HasVideo, bool Private) {
    if (!Owner) return nullptr;
    Post* P = Owner->CreatePost(Text, Paths, HasImage, HasVideo, Private);
    AllPosts_.push_back(P);
    Feed_.Add(P);
    return P;
}

bool PostManager::MarkPostDeleted(Post* P) {
    if (!P) return false;
    P->IsDeleted = true;
    return true;
}

void PostManager::RemoveFromAllPosts(Post* P) {
    for (auto it = AllPosts_.begin(); it != AllPosts_.end(); ++it) {
        if (*it == P) {
            AllPosts_.erase_at(it);
            return;
        }
    }
}

void PostManager::FinalizeDelete(Post* P) {
    if (!P) return;
    // Remove from owner's post list
    if (P->Owner) {
        auto& posts = P->Owner->Posts;
        for (auto it = posts.begin(); it != posts.end(); ++it) {
            if (*it == P) {
                posts.erase_at(it);
                cout << "\ne\nj\nhf\nw\nu\ne\nugf\nkewkjfbjkfvbkewjbfkjwbefjkew";
                break;
            }
        }
    }

    RemoveFromAllPosts(P);
    delete P;
}

Post* PostManager::GetNextFeedPost() {
    while (!Feed_.empty()) {
        Post* Top = Feed_.PeekTop();
        Feed_.PopTop();

        if (!Top || Top->IsDeleted) {
            FinalizeDelete(Top);
            continue;
        }

        return Top;
    }
    return nullptr;
}

void PostManager::BuildFeedFromAllPosts(DoublyList<User*>& Users) {
    Feed_.Clear();
    for (auto uit = Users.begin(); uit != Users.end(); ++uit) {
        User* U = *uit;
        for (auto pit = U->Posts.begin(); pit != U->Posts.end(); ++pit) {
            Post* P = *pit;
            if (!P->IsDeleted) Feed_.Add(P);
        }
    }
}

DoublyList<Post*> PostManager::GetAllPosts() const {
    return AllPosts_;
}
