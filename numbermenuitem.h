#ifndef NUMBERMENUITEM_H
#define NUMBERMENUITEM_H
#include "menuitem.h"

using namespace std;

class NumberMenuItem : public MenuItem
{
public:
    NumberMenuItem(string title, int value = 0);
    void draw(int x, int y, bool hover);
    void addValueLabel(string label);
    void setValue(int value);
    int getValue();
    vector<string> getLabels();
private:
    int value;
    vector<string> labels;
};

#endif // NUMBERMENUITEM_H
