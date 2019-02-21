#include "imagemenuitem.h"

ImageMenuItem::ImageMenuItem(string title, int image) : MenuItem(title)
{
    this->image = image;
}

void ImageMenuItem::draw(int x, int y, bool hover)
{
    if (hover)
    {
        LCDsetTextColor(WHITE);
        LCDdrawbitmap(x + 3, y, IMG[image], IMG_SIZE[image].x, IMG_SIZE[image].x, WHITE);

        if (IMG_SIZE[image].x <= 7) LCDdrawstring_P(x + 12, y, title.substr(windowPos, min(title.size(), windowPos + 11)).c_str());
        else LCDdrawstring_P(x + 14, y, title.substr(windowPos, min(title.size(), windowPos + 11)).c_str());

        if (title.size() > 11)
        {
            if (delayTime) delayTime--;
            if (!delayTime) windowPos++;
            if (windowPos > title.size() - 10)
            {
                windowPos = 0;
            }
        }
    }
    else
    {
        windowPos = 0;
        delayTime = 3;
        LCDsetTextColor(BLACK);
        LCDdrawbitmap(x + 3, y, IMG[image], IMG_SIZE[image].x, IMG_SIZE[image].x, BLACK);

        if (IMG_SIZE[image].x <= 7) LCDdrawstring_P(x + 12, y, title.substr(windowPos, min(title.size(), windowPos + 11)).c_str());
        else LCDdrawstring_P(x + 14, y, title.substr(windowPos, min(title.size(), windowPos + 11)).c_str());
    }
}

int ImageMenuItem::getImage()
{
    return image;
}

