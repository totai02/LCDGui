#ifndef MENUBAR_H
#define MENUBAR_H

#include <iostream>
#include <vector>
#include "PCD8544.h"

#include "imagemenuitem.h"
#include "numbermenuitem.h"
#include "keyboard.h"
#include "dialog.h"
#include "editdialog.h"

#define ITEM_START_POS 13
#define ITEM_SIZE 8

using namespace std;

class MenuBar
{
public:
    MenuBar(string title = "Main Menu");
    void addItem(MenuItem *item);
    void setTitle(string title);
    void enter();
    void up();
    void down();
    void back();
    void update();
    string keyboard(string text = "Keyboard");
    void waitScreen();
    int showDialog(string title="Dialog", string message="Message", int flag = MESSAGE_DIALOG, int duration = 2000);
    int showDialog(string title, int value = 0, function<void(int)> cb = [](int){}, int step = 1);
    int showDialog(string title, int value, vector<string> labels);

private:
    int cursorPos = 0;
    int windowPos = 0;
    MenuItem root;
    MenuItem* currentMenu;
    KeyBoard kb;
    Dialog dialog;
    EditDialog editDialog;
};

#endif // MENUBAR_H
