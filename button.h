#ifndef BUTTON_H
#define BUTTON_H
#include <string>
#include <functional>
#include "PCD8544.h"

#include "define.h"

using namespace std;

class Button
{
public:
    Button(string text = "", int x = 0, int y = 0, int image = NO_IMAGE, int hover_image = NO_IMAGE);
    void setPosition(int x, int y);
    int getImage();
    void draw(bool hover=false);
    void setAction(function<void()>);
    void click();
protected:
    int x, y;
    int image = NO_IMAGE;
    int hover_image = NO_IMAGE;
    string text;
    function<void()> callback = [](){};
};

#endif // BUTTON_H
