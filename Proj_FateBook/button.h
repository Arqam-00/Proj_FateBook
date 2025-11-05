#pragma once
#include <raylib.h>
#include <string>
using namespace std;

class Popup {
public:
    static bool Show(const std::string& title, const std::string& message);
};

class Button {
private:
    Texture2D texture;
    Vector2 position;
    float width;
    float height;
    bool isRound;
    void MakeRound(Image& image);
public:
    Button(const char* imagePath, Vector2 pos, float width, float height, bool round = false);
    Button(const char* imagePath, Vector2 pos, float scale, bool round = false); // For backward compatibility
    ~Button();
    void setPosition(Vector2 pos);
    void setSize(float newWidth, float newHeight);
    float getWidth() const;
    float getHeight() const;
    Vector2 getPosition() const;
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);

};