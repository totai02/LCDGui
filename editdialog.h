#ifndef EDITDIALOG_H
#define EDITDIALOG_H
#include <wiringPi.h>
#include <PCD8544.h>
#include <vector>
#include <string>
#include <functional>

#include "define.h"

using namespace std;

class EditDialog
{
public:
    EditDialog();
    void draw(bool hoverUp = false, bool hoverDown = false);
    void setSize(int width, int height);
    int show(string title="Edit Dialog", int value = 0, int step = 1);
    int show(string title, int value, vector<string> labels);
    void onEdit(function<void(int)> cb);

protected:
    vector<string> labels;
    function<void(int)> editCallback = [](int){};
    int value, step;
    int width, height;
    int visible;
    string title;
};

#endif // EDITDIALOG_H
