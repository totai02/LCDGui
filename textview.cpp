#include "textview.h"

TextView::TextView(string path)
{
    file.open(path);
    pageX = 0;
    pageY = 0;
}

void TextView::open()
{
    for (int i = 0; i < 8; i++)
    {
        string line;
        getline(file, line);
        lines.push_back(line);
    }

    draw();
    visible = true;
    while (digitalRead (BTN4) == LOW);
    delay(250);
    while (visible)
    {
        if (digitalRead (BTN1) == LOW && digitalRead (BTN4) == LOW)
        {
            int start = milisecond();
            while (digitalRead (BTN1) == LOW && digitalRead (BTN4) == LOW && milisecond() - start < 1000) delay(50);
            if (milisecond() - start >= 1000)
            {
                exit();
            }
        }
        else if(digitalRead (BTN1) == LOW)
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
            enter();
            delay(250);
        }
    }

}

void TextView::draw()
{
    LCDclear();
    LCDsetTinyFont(true);
    for (int i = 0; i < 8; i++)
    {
        if (pageX < lines[i + pageY].size()) LCDdrawstring_P(2, i * 6, lines[i + pageY].substr(pageX, 16).c_str());
    }
    LCDdisplay();
}

void TextView::up()
{
    if (pageY > 0) pageY--;
    else
    {
        while (readNewLine());
        pageY = lines.size() - 8;
    }
    draw();
}

void TextView::down()
{
    pageY++;
    if (pageY + 8 > lines.size())
    {
        if (!readNewLine()) pageY = 0;
    }
    draw();
}

void TextView::back()
{
    if (pageX > 0) pageX--;
    draw();
}

void TextView::enter()
{
    pageX++;
    draw();
}

void TextView::exit()
{
    file.close();
    visible = false;
}

bool TextView::readNewLine()
{
    if (file.eof()) return false;
    else
    {
        string line;
        getline(file, line);
        lines.push_back(line);
    }
    return true;
}
