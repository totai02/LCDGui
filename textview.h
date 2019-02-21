#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <iostream>
#include <fstream>
#include <vector>
#include <wiringPi.h>
#include "PCD8544.h"

#include "define.h"

using namespace std;

extern int milisecond();

class TextView
{
public:
    TextView(string path);

    void open();

    void draw();

    void up();
    void down();
    void back();
    void enter();

    void exit();
private:
    bool readNewLine();

    int pageX;
    int pageY;
    bool visible;
    ifstream file;
    vector<string> lines;
};

#endif // TEXTVIEW_H
