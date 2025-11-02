#include <raylib.h>
#include "User.h"
#include "Post.h"
#include <vector>
using namespace std;

// Forward declaration
void View_Profile(User* profileOwner, User* viewer, Font font, int x, int y, int width);

int main() {
    InitWindow(1000, 700, "FakeBook - User Profile Test");
    SetTargetFPS(60);

    Font font = LoadFont("assets/static/Roboto_Condensed-SemiBold.ttf");

    // Create users
    User* arqam = new User("assets/pfps/arqam.png", "Arqam", "arqam@fb.com", "pass", "Kasur", 'M', 21, true, Date(1,2,200));
    User* ali = new User("assets/pfps/ali.png", "Ali", "ali@fb.com", "pass", "Lahore", 'M', 22, true, Date(3,4,2001));
    User* stranger = new User("assets/pfps/stranger.png", "Stranger", "stranger@fb.com", "pass", "Karachi", 'F', 23, true, Date(5,6,2001));

    ali->AddFriend(arqam); arqam->AddFriend(ali);
    stranger->AddFriendRequest(arqam);

    // Create posts
    arqam->CreatePost("Hello world!", {}, false, false, false);
    arqam->CreatePost("Private thoughts...", {}, false, false, true);
    arqam->Posts[0]->Load_Font("assets/static/Roboto_Condensed-SemiBold.ttf");
    arqam->Posts[1]->Load_Font("assets/static/Roboto_Condensed-SemiBold.ttf");
    // Viewer toggle
    User* viewer = arqam; // Change to ali or stranger to test different views

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        View_Profile(arqam, viewer, font, 50, 50, 900);

        EndDrawing();
    }

    // Cleanup
    delete arqam;
    delete ali;
    delete stranger;
    UnloadFont(font);
    CloseWindow();
    return 0;
}
