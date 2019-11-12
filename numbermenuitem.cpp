#include "numbermenuitem.h"

NumberMenuItem::NumberMenuItem(string title, int value) : MenuItem(title)
{
    this->value = value;
}

void NumberMenuItem::draw(int x, int y, bool hover)
{
    string valueStr = "";

    if (labels.size() == 0) valueStr = to_string(value);
    else valueStr = labels[value];

    if (hover)
    {
        LCDsetTextColor(WHITE);
        LCDdrawstring_P(x + 3, y, title.substr(windowPos, min(title.size() - windowPos, 10)).c_str());
        LCDdrawstring_P(84 - valueStr.size() * 6 - 2, y, valueStr.c_str());

        if (title.size() > 10)
        {
            if (delayTime) delayTime--;
            if (!delayTime) windowPos++;
            if (windowPos > title.size() - 5)
            {
                windowPos = 0;
            }
        }
    }
    else
    {
        windowPos = 0;
        delayTime = 3;
        LCDsetTextColor(BLACK);
        LCDdrawstring_P(x + 3, y, title.substr(0, min(title.size(), 10)).c_str());
        LCDdrawstring_P(84 - valueStr.size() * 6 - 2, y, valueStr.c_str());
    }
}

void NumberMenuItem::addValueLabel(string label)
{
    labels.push_back(label);
}

void NumberMenuItem::setValue(int value)
{
    this->value = value;
}

int NumberMenuItem::getValue()
{
    return value;
}

vector<string> NumberMenuItem::getLabels()
{
    return labels;
}
