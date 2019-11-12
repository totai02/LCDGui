#include "editdialog.h"

EditDialog::EditDialog()
{
    width = 70;
    height = 32;
    step = 1;
}

void EditDialog::draw(bool hoverUp, bool hoverDown)
{
    int x = (84 - width) / 2;
    int y = (48 - height) / 2;
    LCDfillrect(x, y, width, height, WHITE);
    LCDdrawrect(x + 1, y + 1, width - 2, height - 2, BLACK);
    LCDdrawline(x + 1, y + 9, 82 - x, y + 9, BLACK);
    LCDsetTinyFont(true);
    LCDdrawstring_P((84 - title.substr(0, 14).size() * 5) / 2, y + 3, title.substr(0, 14).c_str());

    string valueStr;
    if (labels.size() == 0) valueStr = to_string(value);
    else valueStr = labels[value];

    LCDsetTinyFont(false);
    LCDdrawstring_P((84 - valueStr.substr(0, 14).size() * 6) / 2, 24, valueStr.substr(0, 14).c_str());

    if (hoverDown) LCDdrawbitmap(x + 5, 24, IMG[DEC_HOVER], IMG_SIZE[DEC_HOVER].x, IMG_SIZE[DEC_HOVER].y, BLACK);
    else LCDdrawbitmap(x + 5, 24, IMG[DEC], IMG_SIZE[DEC].x, IMG_SIZE[DEC].y, BLACK);

    if (hoverUp) LCDdrawbitmap(84 - x - 10, 24, IMG[INC_HOVER], IMG_SIZE[INC_HOVER].x, IMG_SIZE[INC_HOVER].y, BLACK);
    LCDdrawbitmap(84 - x - 10, 24, IMG[INC], IMG_SIZE[INC].x, IMG_SIZE[INC].y, BLACK);

    LCDdisplay();
}

void EditDialog::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

int EditDialog::show(string title, int value, int step)
{
    this->title = title;
    this->value = value;
    this->step = step;

    bool firstPress = true;
    visible = true;
    draw();
    while (digitalRead (BTN4) == LOW);
    delay(250);
    while (visible)
    {
        if(digitalRead (BTN1) == LOW)
        {
            this->editCallback = [](int){};
            this->labels.clear();
            return value;
        }
        else if(digitalRead (BTN2) == LOW)
        {
            this->value -= step;
            if (labels.size() > 0 && this->value < 0) this->value = labels.size() - 1;
            editCallback(this->value);
            draw(false, true);
            if (firstPress) delay(250);
            else delay(100);
            draw();
            firstPress = false;
        }
        else if(digitalRead (BTN3) == LOW)
        {
            this->value += step;
            if (labels.size() > 0 && this->value >= labels.size()) this->value = 0;
            editCallback(this->value);
            draw(true, false);
            if (firstPress) delay(250);
            else delay(100);
            draw();
            firstPress = false;
        }
        else if(digitalRead (BTN4) == LOW)
        {
            break;
        }
        else
        {
            firstPress = true;
        }
    }

    this->editCallback = [](int){};
    this->labels.clear();
    return this->value;
}

int EditDialog::show(string title, int value, vector<string> labels)
{
    this->labels = labels;
    return this->show(title, value, 1);
}

void EditDialog::onEdit(function<void (int)> cb)
{
    this->editCallback = cb;
}
