#ifndef MENUFILEITEM_H
#define MENUFILEITEM_H
#include "menuitem.h"

class ImageMenuItem : public MenuItem
{
public:
    ImageMenuItem(string title, int image = FILE_IMG);
    void draw(int x, int y, bool hover);
    int getImage();
protected:
    int image;
};

#endif // MENUFILEITEM_H
