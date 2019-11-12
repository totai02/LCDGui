#include "dialog.h"

extern void split(std::string const &str, const char delim,
            std::vector<std::string> &out);

extern string trim(const string& str);

Dialog::Dialog()
{
    this->title = "Dialog";
    this->message = "Message";
    width = 76;
    height = 40;
}

void Dialog::draw()
{
    int x = (84 - width) / 2;
    int y = (48 - height) / 2;
    LCDfillrect(x, y, width, height, WHITE);
    LCDdrawrect(x + 1, y + 1, width - 2, height - 2, BLACK);
    LCDdrawline(x + 1, y + 9, 82 - x, y + 9, BLACK);
    LCDsetTinyFont(true);
    LCDdrawstring_P((84 - title.substr(0, 14).size() * 5) / 2, y + 3, title.substr(0, 14).c_str());
    vector<string> words;
    split(message, ' ', words);
    int line = 0, idx = 0, length = 0, numLine = 3;
    if (buttons.size() != 0) numLine = 2;
    string word = "";
    while (line < numLine && idx < words.size())
    {
        if (words[idx].size() > 15) words[idx] = words[idx].substr(0, 15);
        if (length + words[idx].size() + 1 > 14)
        {
            LCDdrawstring_P((84 - word.size() * 5) / 2, y + 14 + line * 7, word.c_str());
            line++;
            word = words[idx];
            length = words[idx].size();
        }
        else
        {
            if (word.size() != 0)
            {
                word += " " + words[idx];
                length += words[idx].size() + 1;
            }
            else
            {
                length = words[idx].size();
                word = words[idx];
            }
        }
        if (idx == words.size() - 1 && line < numLine) LCDdrawstring_P((84 - word.size() * 5) / 2, y + 14 + line * 7, word.c_str());
        idx++;
    }

    int space = 84 - width - 4;

    for (int i = 0; i < buttons.size(); i++)
    {
        x = i * (space / 3) + (space / 3 - 15) / 2;
        y = 28;
        if (i == cursor) buttons[i]->draw(true);
        else buttons[i]->draw(false);
    }

    LCDdisplay();
}

void Dialog::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

int Dialog::show(string title, string message, int duration)
{
    if (buttons.size() == 0)
    {
        this->title = title;
        this->message = message;
        draw();
        delay(duration);
        clearButton();
        return 0;
    }
    else
    {
        this->title = title;
        this->message = message;

        alignButton();
        selectedButton = -1;
        cursor = 0;
        visible = true;
        draw();
        while (digitalRead (BTN4) == LOW);
        delay(250);
        while (visible)
        {
            if(digitalRead (BTN1) == LOW)
            {
                back();
                delay(250);
            }
            if(digitalRead (BTN2) == LOW)
            {
                up();
                delay(250);
            }
            if(digitalRead (BTN3) == LOW)
            {
                down();
                delay(250);
            }
            if(digitalRead (BTN4) == LOW)
            {
                press();
                delay(250);
            }
        }
        clearButton();
        return selectedButton;
    }
}

void Dialog::clearButton()
{
    buttons.clear();
}

void Dialog::addButton(Button *btn)
{
    buttons.push_back(btn);
}

void Dialog::up()
{
    cursor--;
    if (cursor < 0) cursor = buttons.size() - 1;
    draw();
}

void Dialog::down()
{
    cursor++;
    if (cursor >= buttons.size()) cursor = 0;
    draw();
}

void Dialog::back()
{
    visible = false;
}

void Dialog::press()
{
    selectedButton = buttons[cursor]->getImage();
    visible = false;
}

void Dialog::alignButton()
{
    int space = 72 / buttons.size();
    for (int i = 0; i < buttons.size(); i++)
    {
        int x = (space - IMG_SIZE[buttons[i]->getImage()].x) / 2 + space * i + 6;
        int y = 32;
        buttons[i]->setPosition(x, y);
    }
}
