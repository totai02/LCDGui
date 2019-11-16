#include "menuitem.h"

MenuItem::MenuItem(string title) : parent(nullptr)
{
    this->title = title;
}

MenuItem::MenuItem(string title, string content)
{
    this->title = title;
    this->content = content;
}

void MenuItem::addItem(MenuItem *item)
{
    items.push_back(item);
    item->parent = this;
}

void MenuItem::clearItem()
{
    items.clear();
}

vector<MenuItem *> MenuItem::getItems() const
{
    return items;
}

string MenuItem::getTitle()
{
    return title;
}

void MenuItem::setTitle(string title)
{
    this->title = title;
}

void MenuItem::draw(int x, int y, bool hover)
{
    if (hover)
    {
        if (content == "")
        {
            LCDsetTextColor(WHITE);
            LCDdrawstring_P(x + 3, y, title.substr(windowPos, min(title.size() - windowPos, 13)).c_str());
            if (title.size() > 13)
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
            string cutTitle = title.substr(0, 7) + ":";
            LCDsetTextColor(WHITE);
            LCDdrawstring_P(x + 3, y, cutTitle.c_str());
            LCDdrawstring_P(x + 3 + cutTitle.size() * 6, y, content.substr(windowPos, min(content.size() - windowPos, 14 - cutTitle.size())).c_str());
            if (content.size() > 13 - cutTitle.size())
            {
                if (delayTime) delayTime--;
                if (!delayTime) windowPos++;
                if (windowPos > content.size() - 5)
                {
                    windowPos = 0;
                }
            }
        }
    }
    else
    {
        windowPos = 0;
        delayTime = 3;
        LCDsetTextColor(BLACK);
        if (content == "") LCDdrawstring_P(x + 3, y, title.substr(0, 13).c_str());
        else
        {
            string cutTitle = title.substr(0, 7) + ":" + content;
            LCDdrawstring_P(x + 3, y, cutTitle.substr(0, 13).c_str());
        }
    }
}

void MenuItem::select()
{
    callback();
}

void MenuItem::exit()
{
    exitCb();
}

void MenuItem::setAction(function<void()> cb)
{
    callback = cb;
}

void MenuItem::setExitAction(function<void ()> cb)
{
    exitCb = cb;
}
