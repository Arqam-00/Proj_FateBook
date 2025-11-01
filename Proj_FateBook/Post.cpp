#include "Post.h"

Post::Post()
{
    Owner = nullptr;
    Text = "";
    HasImage = false;
    HasVideo = false;
    Is_Private = false;
    IsDeleted = false;
    TimeStamp = time(0);
    LikeCount = 0;

}

Post::Post(User* OwnerPtr, const string& TextContent, bool Private)
{


    Owner = OwnerPtr;
    Text = TextContent;
    HasImage = false;
    HasVideo = false;
    Is_Private = Private;
    IsDeleted = false;
    TimeStamp = time(0);
    LikeCount = 0;
}
