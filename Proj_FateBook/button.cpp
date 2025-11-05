#include "Button.h"
#include <cmath>
#include <algorithm>

Button::Button(const char* imagePath, Vector2 pos, float width, float height, bool round)
    : position(pos), width(width), height(height), isRound(round) {

    Image image = LoadImage(imagePath);
    if (round) {
        MakeRound(image);
    }
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

Button::Button(const char* imagePath, Vector2 pos, float scale, bool round)
    : position(pos), isRound(round) {

    Image image = LoadImage(imagePath);
    if (round) {
        MakeRound(image);
    }
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    this->width = texture.width * scale;
    this->height = texture.height * scale;
}

Button::~Button() {
    UnloadTexture(texture);
}

void Button::setPosition(Vector2 pos) {
    position = pos;
}

void Button::setSize(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

float Button::getWidth() const {
    return width;
}

float Button::getHeight() const {
    return height;
}

Vector2 Button::getPosition() const {
    return position;
}

void Button::Draw() {
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, width, height },
        { 0, 0 }, 0, WHITE);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed) {
    Rectangle bounds = { position.x, position.y, width, height };
    return mousePressed && CheckCollisionPointRec(mousePos, bounds);
}

void Button::MakeRound(Image& image) {
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    int centerX = image.width / 2;
    int centerY = image.height / 2;
    float radius = min(centerX, centerY);
    Image roundImage = GenImageColor(image.width, image.height, BLANK);
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance <= radius) {
                Color pixel = GetImageColor(image, x, y);
                ImageDrawPixel(&roundImage, x, y, pixel);
            }
        }
    }
    UnloadImage(image);
    image = roundImage;
}

bool Popup::Show(const std::string& title, const std::string& message) {
    int popupWidth = 400;
    int popupHeight = 200;
    int popupX = (GetScreenWidth() - popupWidth) / 2;
    int popupY = (GetScreenHeight() - popupHeight) / 2;

    bool result = false;
    bool popupActive = true;

    while (popupActive && !WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        Rectangle confirmBtn = { popupX + 80, popupY + 140, 100, 40 };
        Rectangle cancelBtn = { popupX + 220, popupY + 140, 100, 40 };

        if (mousePressed) {
            if (CheckCollisionPointRec(mousePos, confirmBtn)) {
                result = true;
                popupActive = false;
            }
            else if (CheckCollisionPointRec(mousePos, cancelBtn)) {
                result = false;
                popupActive = false;
            }
        }

        BeginDrawing();
        ClearBackground({ 245, 245, 245, 255 });
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 128 });
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, { 245, 245, 245, 255 });
        DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText(title.c_str(), popupX + 20, popupY + 20, 24, DARKBLUE);
        DrawText(message.c_str(), popupX + 20, popupY + 70, 18, BLACK);
        DrawRectangleRec(confirmBtn, RED);
        DrawText("Confirm", popupX + 100, popupY + 150, 18, WHITE);

        DrawRectangleRec(cancelBtn, GRAY);
        DrawText("Cancel", popupX + 245, popupY + 150, 18, WHITE);

        EndDrawing();
    }
    return result;
}