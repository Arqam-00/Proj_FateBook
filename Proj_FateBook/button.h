#pragma once
#include <raylib.h>
#include <raymath.h>
#include <cmath>
class Button {
private:
    Texture2D texture;
    Vector2 position;
    void MakeRound(Image& image);
public:
    Button(const char* imagePath, Vector2 imagePosition, float scale,bool Round = false);
    void setPosition(Vector2 pos);
    float getWidth() const;
    float getHeight() const;
    ~Button();
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);

    
};