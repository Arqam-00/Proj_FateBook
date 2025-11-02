#include "button.h"
#include<string>


Button::Button(const char* imagePath, Vector2 imagePosition, float scale) {


    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);

    UnloadImage(image);
    position = imagePosition;
}


void Button::setPosition(Vector2 pos) {
    position = pos;
}

float Button::getWidth() const {
    return (float)texture.width;
}

float Button::getHeight() const {
    return (float)texture.height;
}

Button::~Button() {
    UnloadTexture(texture);
}

void Button::Draw() {
    DrawTextureV(texture, position, WHITE);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed) {
    Rectangle rect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
    {
        return true;
    }
    return false;
}
