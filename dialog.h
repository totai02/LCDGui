#ifndef DIALOG_H
#define DIALOG_H
#include <wiringPi.h>
#include "PCD8544.h"
#include <vector>

#include "button.h"
#include "define.h"

using namespace std;

class Dialog
{
public:
    Dialog();
    void draw();
    void setSize(int width, int height);
    int show(string title="Dialog", string message="Message", int duration = 2000);
    void clearButton();
    void addButton(Button*);

    void up();
    void down();
    void back();
    void press();

protected:
    void alignButton();

    vector<Button*> buttons;
    int width, height, cursor = 0;
    int selectedButton = -1;
    int visible;
    string title;
    string message;
};

#endif // DIALOG_H
