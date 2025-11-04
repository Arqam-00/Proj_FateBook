#include "button.h"
#include<string>


Button::Button(const char* imagePath, Vector2 imagePosition, float scale,bool Round) {

    
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    if (Round) {
        MakeRound(image);
    }
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

void Button::MakeRound(Image& image) {
    ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    int centerX = image.width / 2;
    int centerY = image.height / 2;
    float radius = std::min(centerX, centerY);
    Color* pixels = LoadImageColors(image);
    Image roundImage = GenImageColor(image.width, image.height, BLANK);
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance <= radius) {
                ImageDrawPixel(&roundImage, x, y, pixels[y * image.width + x]);
            }
        }
    }
    UnloadImage(image);
    image = roundImage;
    UnloadImageColors(pixels);
}
