#include "keyboard.h"

KeyBoard::KeyBoard()
{
    visible = false;
    currentKeySet = 0;
    hoverItem.x = 0;
    hoverItem.y = 0;
    genKeySet(0);
    genKeySet(1);
    genKeySet(2);
    
}

void KeyBoard::setTitle(string title)
{
    this->title = title;
}

void KeyBoard::draw()
{
    LCDclear();
    LCDdrawrect(1, 9, 82, 9, BLACK);

    LCDsetTinyFont(false);
    LCDdrawstring_P((84 - title.substr(0, 14).size() * 6) / 2, 1, title.substr(0, 14).c_str());

    string displayText = text + "_";
    if (displayText.size() > 16)
    {
        displayText = displayText.substr(displayText.size() - 16, 17);
    }
    LCDsetTinyFont(true);
    LCDdrawstring_P(3, 11, displayText.c_str());
    for (int i = 0; i < buttons[currentKeySet].size(); i++)
    {
        if (buttons[currentKeySet][i] == keyMap[currentKeySet][hoverItem.x][hoverItem.y]) buttons[currentKeySet][i]->draw(true);
        else buttons[currentKeySet][i]->draw(false);
    }
    LCDdisplay();
}

void KeyBoard::up()
{
    int x = hoverItem.x;
    int y = hoverItem.y;
    do
    {
        hoverItem.y--;
        if (hoverItem.y < 0) hoverItem.y = 10;
    } while (keyMap[currentKeySet][hoverItem.x][hoverItem.y] == keyMap[currentKeySet][x][y]);
    draw();
}

void KeyBoard::down()
{
    int x = hoverItem.x;
    int y = hoverItem.y;
    do
    {
        hoverItem.y++;
        if (hoverItem.y > 10) hoverItem.y = 0;
    } while (keyMap[currentKeySet][hoverItem.x][hoverItem.y] == keyMap[currentKeySet][x][y]);
    draw();
}

void KeyBoard::back()
{
    int x = hoverItem.x;
    int y = hoverItem.y;
    do
    {
        hoverItem.x++;
        if (hoverItem.x > 3) hoverItem.x = 0;
    } while (keyMap[currentKeySet][hoverItem.x][hoverItem.y] == keyMap[currentKeySet][x][y]);
    draw();
}

void KeyBoard::press()
{
    keyMap[currentKeySet][hoverItem.x][hoverItem.y]->click();
    draw();
}

string KeyBoard::popup(string title)
{
    if (title.compare("")) this->title = title;
    text = "";
    currentKeySet = 0;
    draw();
    visible = true;
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

    LCDclear();
    LCDdisplay();

    return text;
}

void KeyBoard::initPin()
{
    pinMode (BL, OUTPUT);
    digitalWrite(BL, HIGH);

    pinMode (BTN1, INPUT);
    pullUpDnControl (BTN1, PUD_UP) ;

    pinMode (BTN2, INPUT);
    pullUpDnControl (BTN2, PUD_UP) ;

    pinMode (BTN3, INPUT);
    pullUpDnControl (BTN3, PUD_UP) ;

    pinMode (BTN4, INPUT);
    pullUpDnControl (BTN4, PUD_UP) ;
}

void KeyBoard::del()
{
    if (text.size() > 0) text = text.substr(0, text.size() - 1);
}

void KeyBoard::upper()
{
    if (currentKeySet != 2)
    {
        if (currentKeySet == 0) currentKeySet = 1;
        else currentKeySet = 0;
    }
    draw();
}

void KeyBoard::symbol()
{
    if (currentKeySet != 2) currentKeySet = 2;
    else currentKeySet = 0;
    draw();
}

void KeyBoard::cancel()
{
    text = "";
    visible = false;
}

void KeyBoard::enter()
{
    visible = false;
}

void KeyBoard::genKeySet(int idx)
{
    string classKey1, classKey2, classKey3;

    if (idx == 0)
    {
        classKey1 = "QWERTYUIOP";
        classKey2 = "ASDFGHJKL";
        classKey3 = "ZXCVBNM";
    }
    else if (idx == 1)
    {
        classKey1 = "qwertyuiop";
        classKey2 = "asdfghjkl";
        classKey3 = "zxcvbnm";
    }
    else if (idx == 2)
    {
        classKey1 = "1234567890";
        classKey2 = "+-=<>/\\*:";
        classKey3 = "()_.@!?";
    }

    for (int i = 0; i < 10; i++)
    {
        Button *btn = new Button(classKey1.substr(i, 1), 10 + i * 7, 20);
        buttons[idx].push_back(btn);
        keyMap[idx][0][i + 1] = btn;
        btn->setAction([this, classKey1, i](){
            text += classKey1.substr(i, 1);
        });
    }

    for (int i = 0; i < 9; i++)
    {
        Button *btn = new Button(classKey2.substr(i, 1), 10 + i * 7, 27);
        buttons[idx].push_back(btn);
        keyMap[idx][1][i + 1] = btn;
        btn->setAction([this, classKey2, i](){
            text += classKey2.substr(i, 1);
        });
    }

    for (int i = 0; i < 8; i++)
    {
        Button *btn = new Button(classKey3.substr(i, 1), 17 + i * 7, 35);
        buttons[idx].push_back(btn);
        keyMap[idx][2][i + 2] = btn;
        btn->setAction([this, classKey3, i](){
            text += classKey3.substr(i, 1);
        });
    }

    Button *keyENTER = new Button("", 67, 36, ENTER, ENTER_HOVER);
    Button *keySPACE = new Button("", 18, 41, BACKSPACE, BACKSPACE_HOVER);
    Button *keyDEL = new Button("", 73, 28, DEL, DEL_HOVER);
    Button *keySYMBOL = new Button("", 2, 38, SYMBOL, SYMBOL_HOVER);
    Button *keyUPPER = new Button("", 2, 28, UPPER, UPPER_HOVER);
    Button *keyEXIT = new Button("", 2, 20, EXIT, EXIT_HOVER);

    keyENTER->setAction([this](){
        enter();
    });
    keySPACE->setAction([this](){
        text += " ";
    });
    keyDEL->setAction([this](){
        del();
    });
    keySYMBOL->setAction([this](){
        symbol();
    });
    keyUPPER->setAction([this](){
        upper();
    });
    keyEXIT->setAction([this](){
        cancel();
    });

    buttons[idx].push_back(keyENTER);
    buttons[idx].push_back(keySPACE);
    buttons[idx].push_back(keyDEL);
    buttons[idx].push_back(keySYMBOL);
    buttons[idx].push_back(keyUPPER);
    buttons[idx].push_back(keyEXIT);

    keyMap[idx][0][0] = keyEXIT;
    keyMap[idx][1][0] = keyUPPER;
    keyMap[idx][2][0] = keySYMBOL;
    keyMap[idx][2][1] = keySYMBOL;
    keyMap[idx][3][0] = keySYMBOL;
    keyMap[idx][3][1] = keySYMBOL;
    keyMap[idx][3][2] = keySPACE;
    keyMap[idx][3][3] = keySPACE;
    keyMap[idx][3][4] = keySPACE;
    keyMap[idx][3][5] = keySPACE;
    keyMap[idx][3][6] = keySPACE;
    keyMap[idx][3][7] = keySPACE;
    keyMap[idx][3][8] = keySPACE;
    keyMap[idx][1][10] = keyDEL;
    keyMap[idx][2][9] = keyENTER;
    keyMap[idx][2][10] = keyENTER;
    keyMap[idx][3][9] = keyENTER;
    keyMap[idx][3][10] = keyENTER;
}
