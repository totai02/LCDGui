#include "button.h"

Button::Button(string text, int x, int y, int image, int hover_image)
{
    this->text = text;
    this->image = image;
    this->hover_image = hover_image;
    this->x = x;
    this->y = y;
}

void Button::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Button::getImage()
{
    return image;
}

void Button::draw(bool hover)
{
    LCDsetTinyFont(true);

    if (image == NO_IMAGE)
    {
        if (hover)
        {
            LCDfillrect(x, y - 1, 5 * text.size()+ 1, 7, BLACK);
            LCDsetTextColor(WHITE);
            LCDdrawstring_P(x, y, text.c_str());
        }
        else
        {
            LCDsetTextColor(BLACK);
            LCDdrawstring_P(x, y, text.c_str());
        }
    }
    else
    {
        if (hover)
        {
            LCDdrawbitmap(x, y, IMG[hover_image], IMG_SIZE[hover_image].x, IMG_SIZE[hover_image].y, BLACK);
        }
        else
        {
            LCDdrawbitmap(x, y, IMG[image], IMG_SIZE[image].x, IMG_SIZE[image].y, BLACK);
        }
    }
}

void Button::setAction(function<void ()> cb)
{
    callback = cb;
}

void Button::click()
{
    callback();
}
