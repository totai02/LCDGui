#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <vector>
#include <wiringPi.h>
#include "PCD8544.h"
#include "button.h"

#include "define.h"

using namespace std;

struct Point{
    int x;
    int y;
};

class KeyBoard
{
public:
    KeyBoard();
    void setTitle(string);
    void draw();
    void up();
    void down();
    void back();
    void press();
    string popup(string title = "Keyboard");

    void initPin();

private:
    void genKeySet(int idx);
    void del();
    void upper();
    void symbol();
    void cancel();
    void enter();

    bool visible;
    string title;
    string text;
    Point hoverItem;
    vector<Button*> buttons[3];
    int currentKeySet;
    Button *keyMap[3][4][11];
};

#endif // KEYBOARD_H
