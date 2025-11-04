#include "Post.h"
#include <iostream>
using namespace std;
Post::Post()
{
    Ownerid = 0;
    Owner = nullptr;
    Text = "";
    OwnerPfp = {0};
    Is_Private = false;
    IsDeleted = false;
    TimeStamp = time(0);
    f;
}

Post::Post(User* OwnerPtr, const string& TextContent, bool Private)
{
    Ownerid = OwnerPtr->id;
    Owner = OwnerPtr;
    Text = TextContent;
    OwnerPfp = {0};
    Is_Private = Private;
    IsDeleted = false;
    TimeStamp = time(0);
}



