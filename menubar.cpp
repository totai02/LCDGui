#include "menubar.h"

int min(int a, int b)
{
    return a <= b ? a : b;
}

MenuBar::MenuBar(string title) : currentMenu(nullptr),
                                 root(MenuItem(title))
{
    currentMenu = &root;
}

void MenuBar::addItem(MenuItem *item)
{
    root.addItem(item);
    update();
}

void MenuBar::enter()
{
    MenuItem *selected = currentMenu->getItems()[cursorPos];
    if (selected->getItems().size() > 0)
    {
        currentMenu = selected;
        cursorPos = 0;
        windowPos = 0;
        update();
    }
    else
    {
        selected->select();
    }
}

void MenuBar::up()
{
    if (cursorPos > 0) cursorPos--;
    else cursorPos = currentMenu->getItems().size() - 1;

    if (cursorPos < windowPos) windowPos = cursorPos;
    if (cursorPos > windowPos + 3) windowPos = cursorPos - 3;

    update();
}

void MenuBar::down()
{
    if (cursorPos < currentMenu->getItems().size() - 1) cursorPos++;
    else cursorPos = 0;

    if (cursorPos < windowPos) windowPos = cursorPos;
    if (cursorPos > windowPos + 3) windowPos = cursorPos - 3;

    update();
}

void MenuBar::back()
{
    if (currentMenu->parent != nullptr)
    {
        currentMenu = currentMenu->parent;
        cursorPos = 0;
        windowPos = 0;
        update();
    }
}

void MenuBar::update()
{
    LCDclear();
    LCDsetTinyFont(false);

    // draw title
    string title = currentMenu->getTitle().substr(0, 14);
    LCDsetTextColor(WHITE);
    LCDfillrect(0, 0, 84, 11, BLACK);
    LCDdrawstring_P((84 - title.size() * 6) / 2, 2, title.c_str());

    // draw background cursor
    LCDfillrect(2, ITEM_START_POS + ITEM_SIZE * (cursorPos - windowPos), 80, 9, BLACK);

    // draw item
    for (int i = windowPos; i < min(windowPos + 4, currentMenu->getItems().size()); i++)
    {
        if (i == cursorPos)
        {
            currentMenu->getItems()[i]->draw(0, ITEM_START_POS + ITEM_SIZE * (i - windowPos) + 1, true);
        }
        else if (i > cursorPos)
        {
            currentMenu->getItems()[i]->draw(0, ITEM_START_POS + ITEM_SIZE * (i - windowPos) + 2);
        }
        else
        {
            currentMenu->getItems()[i]->draw(0, ITEM_START_POS + ITEM_SIZE * (i - windowPos));
        }
    }

    LCDdrawrect(0, 0, 84, 48, BLACK);
    LCDsetTextColor(BLACK);
    LCDdisplay();
}

string MenuBar::keyboard(string title)
{
    return kb.popup(title);
}

void MenuBar::waitScreen()
{
    LCDclear();
    LCDdrawbitmap(34, 12, IMG[CLOCK], IMG_SIZE[CLOCK].x, IMG_SIZE[CLOCK].y, BLACK);
    LCDdisplay();
}

int MenuBar::showDialog(string title, string message, int flag, int duration)
{
    if (flag == POWER_DIALOG)
    {
        dialog.clearButton();
        dialog.addButton(new Button("", 0, 0, SHUTDOWN, SHUTDOWN_HOVER));
        dialog.addButton(new Button("", 0, 0, RESTART, RESTART_HOVER));
        dialog.addButton(new Button("", 0, 0, CANCEL, CANCEL_HOVER));
    }
    else if (flag == YESNO_DIALOG)
    {
        dialog.clearButton();
        dialog.addButton(new Button("", 0, 0, ACCEPT, ACCEPT_HOVER));
        dialog.addButton(new Button("", 0, 0, CANCEL, CANCEL_HOVER));
    }
    else if (flag == OPENFILE_DIALOG)
    {
        dialog.clearButton();
        dialog.addButton(new Button("", 0, 0, OPEN, OPEN_HOVER));
        dialog.addButton(new Button("", 0, 0, RUN, RUN_HOVER));
    }
    else if (flag == ACCEPT_DIALOG)
    {
        dialog.clearButton();
        dialog.addButton(new Button("", 0, 0, ACCEPT, ACCEPT_HOVER));
    }
    int status = dialog.show(title, message, duration);
    if (flag != MESSAGE_DIALOG) update();
    return status;
}
