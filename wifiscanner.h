#ifndef WIFISCANNER_H
#define WIFISCANNER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

struct WifiAP
{
    string ssid;
    double quality;
};

class WifiScanner
{
public:
    WifiScanner();
    vector<WifiAP> scan();
    int connect(string ssid, string pwd);
};

#endif // WIFISCANNER_H
