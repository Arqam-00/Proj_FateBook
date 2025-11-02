#pragma once
#include <raylib.h>

class Button {
private:
    Texture2D texture;
    Vector2 position;
public:
    Button(const char* imagePath, Vector2 imagePosition, float scale);
    void setPosition(Vector2 pos);
    float getWidth() const;
    float getHeight() const;
    ~Button();
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);
};