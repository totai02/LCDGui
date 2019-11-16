#include <wiringPi.h>
#include <PCD8544.h>
#include <experimental/filesystem>
#include <sys/time.h>
#include <algorithm>
#include <vector>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_SIZE 32

namespace fs = std::experimental::filesystem;
using namespace std;

#include "menubar.h"
#include "button.h"
#include "wifiscanner.h"
#include "textview.h"

MenuBar menuBar("Main Menu");
WifiScanner scanner;
bool light = true;
int contrast = 45;

void initPin();
void showKeyboard();
void runSmartLibrary();
void changeLight(MenuItem *setting3);
void showLogo();
int createFileList(MenuItem *menuItem, string path);
int milisecond();
void getIP(MenuItem *setting4);
void shutdownOptions();
void startFruitPicker(int angle, int duration, int ballColor, int offsetX, int offsetY, int v1, int v2, int stadium);
extern string exec(const char* cmd);
void runProgram(string filename, string path, string param = "");

bool runProgramMode = false;
string prog_name;

int main()
{
    cout << "LCDGui is running..." << endl;
    cout << "Press [CTRL+C] to exit." << endl;

    wiringPiSetup();
    LCDInit();
    LCDsetContrast(contrast);
    initPin();

    LCDclear();
    LCDshowLogo();
    LCDdisplay();
    delay(2000);

    MenuItem prog("Programs");
    MenuItem file("File Explorer");
    MenuItem sys("System");
    MenuItem power("Power");

    MenuItem prog1("Smart Library");
    MenuItem prog2("Show Keyboard");

    MenuItem setting1("LCD Contrast");
    MenuItem setting2("Wi-Fi");
    MenuItem setting3("Light: ON");
    MenuItem setting4("Get IP");

    prog.addItem(&prog1);
    prog.addItem(&prog2);

    sys.addItem(&setting1);
    sys.addItem(&setting2);
    sys.addItem(&setting4);
    sys.addItem(&setting3);

    menuBar.addItem(&prog);
    menuBar.addItem(&file);
    menuBar.addItem(&sys);
    menuBar.addItem(&power);

    prog1.setAction(runSmartLibrary);
    prog2.setAction(showKeyboard);
    power.setAction(shutdownOptions);

    setting1.setAction([](){
        contrast = menuBar.showDialog("Contrast", contrast, [](int value){
            LCDsetContrast(value);
        });
        LCDsetContrast(contrast);
    });
    setting2.setAction([&setting2](){
        menuBar.waitScreen();
        vector<WifiAP> apList = scanner.scan();
        for (unsigned int i = 0; i < apList.size(); i++)
        {
            int image;
            if (apList[i].quality > 50) image = WIFI1;
            else if (apList[i].quality > 20) image = WIFI2;
            else image = WIFI3;

            string ssid = apList[i].ssid;
            ImageMenuItem *item = new ImageMenuItem(apList[i].ssid, image);
            item->setAction([ssid](){
                string password = menuBar.keyboard();
                if (password.compare("") != 0)
                {
                    menuBar.waitScreen();
                    int error = scanner.connect(ssid, password);
                    if (!error) menuBar.showDialog("Wi-Fi", "Connect Success");
                    else menuBar.showDialog("Wi-Fi", "Connect Failed");
                }
            });
            setting2.addItem(item);
        }
        menuBar.enter();
    });
    setting2.setExitAction([&setting2](){
       setting2.clearItem();
    });
    setting3.setAction(bind(changeLight, &setting3));
    setting4.setAction(bind(getIP, &setting4));
    setting4.setExitAction([&setting4](){
        setting4.clearItem();
    });

    createFileList(&file, "/home/pi");
    changeLight(&setting3);

    int timer = milisecond();
    int deltaTime = 0;
    while (true)
    {
        if (!runProgramMode)
        {
            if(digitalRead (BTN1) == LOW)
            {
                menuBar.back();
                delay(250);
            }
            if(digitalRead (BTN2) == LOW)
            {
                menuBar.up();
                delay(250);
            }
            if(digitalRead (BTN3) == LOW)
            {
                menuBar.down();
                delay(250);
            }
            if(digitalRead (BTN4) == LOW)
            {
                menuBar.enter();
                delay(250);
            }

            if (!runProgramMode)
            {
                deltaTime = milisecond() - timer;
                if (deltaTime >= 500)
                {
                    deltaTime = 0;
                    timer = milisecond();
                    menuBar.update();
                }
            }
        }
        else
        {
            if (digitalRead (BTN1) == LOW)
            {
                int start = milisecond();
                while ((digitalRead (BTN1) == LOW) && (milisecond() - start < 2000))
                {
                    delay(50);
                }
                if (milisecond() - start >= 2000)
                {
                    system(("sudo killall " + prog_name).c_str());
                    menuBar.update();
                    runProgramMode = false;
                    while (digitalRead (BTN1) == LOW) delay(50);
                    delay(250);
                }
            }
        }
        delay(50);
    }
    return 0;
}

void runSmartLibrary()
{
    runProgram("Smart Library", "/home/pi/build-SmartLibrary-Desktop-Debug/SmartLibrary", "");
}

void runProgram(string filename, string path, string param)
{
    menuBar.showDialog("Program ", filename + " is running.");
    string cmd = "lxterminal -e sudo " + path + " " + param + " &";
    system(cmd.c_str());
    prog_name = filename;
    runProgramMode = true;
}

void startFruitPicker(int angle, int duration, int ballColor, int offsetX, int offsetY, int v1, int v2, int stadium)
{
    string param = to_string(angle) + " " + to_string(duration) + " " + to_string(ballColor) + " " + to_string(offsetX) + " " + to_string(offsetY) + " " + to_string(v1) + " " + to_string(v2) + " " + to_string(stadium);
    string cmd = "sudo /home/pi/build-FruitPicker-Desktop-Debug/FruitPicker " + param;

    menuBar.showDialog("Program", "FruitPicker is running.");
    exec(cmd.c_str());
    menuBar.update();
}

void getIP(MenuItem *setting4)
{
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            MenuItem *item = new MenuItem(string(ifa->ifa_name), string(addressBuffer));
            setting4->addItem(item);
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    menuBar.enter();
}

void initPin()
{
    pinMode (BTN1, INPUT);
    pullUpDnControl (BTN1, PUD_UP) ;

    pinMode (BTN2, INPUT);
    pullUpDnControl (BTN2, PUD_UP) ;

    pinMode (BTN3, INPUT);
    pullUpDnControl (BTN3, PUD_UP) ;

    pinMode (BTN4, INPUT);
    pullUpDnControl (BTN4, PUD_UP) ;

    pinMode (BL, OUTPUT);
}

void showKeyboard()
{
    string text = menuBar.keyboard();
    if (text.compare("") != 0)
    {
        LCDsetTinyFont(false);
        LCDdrawstring_P(0, 0, text.c_str());
        LCDdisplay();
        delay(2000);
    }
}

void changeLight(MenuItem *setting3)
{
    light = !light;
    if (!light) setting3->setTitle("Light: On");
    else setting3->setTitle("Light: OFF");
    menuBar.update();
    digitalWrite(BL, light);
}

void showLogo()
{
    LCDclear();
    LCDshowLogo();
    LCDdisplay();
    delay(2000);
    menuBar.update();
}

bool sortFunc(fs::directory_entry obj1, fs::directory_entry obj2)
{
    string str1 = obj1.path().filename().string();
    string str2 = obj2.path().filename().string();

    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);

    return str1.compare(str2) < 0;
}

int createFileList(MenuItem *menuItem, string path)
{
    vector<fs::directory_entry> files;
    vector<fs::directory_entry> folders;
    for (const auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().filename().string()[0] != '.')
        {
            if (fs::is_directory(entry.status()))
            {
                folders.push_back(entry);
            }
            if (fs::is_regular_file(entry.status()))
            {
                files.push_back(entry);
            }
        }
    }
    sort(folders.begin(), folders.end(), sortFunc);
    sort(files.begin(), files.end(), sortFunc);

    for (const auto & entry : folders)
    {
        ImageMenuItem *folder = new ImageMenuItem(entry.path().filename().string(), FOLDER);
        string path = entry.path().string();
        folder->setAction([folder, path](){
            int numOfEntry = createFileList(folder, path);
            if (numOfEntry != 0) menuBar.enter();
        });
        menuItem->addItem(folder);
    }

    for (const auto & entry : files)
    {
        ImageMenuItem *file = new ImageMenuItem(entry.path().filename().string(), FILE_IMG);
        file->setAction([entry](){
            int btn = menuBar.showDialog("Open file", "Choose action", OPENFILE_DIALOG);
            if (btn == RUN)
            {
                runProgram(entry.path().filename().string(), entry.path().string());
            }
            else if (btn == OPEN)
            {
                TextView textView(entry.path().string());
                textView.open();
                menuBar.update();
                while (digitalRead(BTN1) == LOW);
                delay(250);
            }
        });
        menuItem->addItem(file);
    }
    return files.size() + folders.size();
}

int milisecond()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void shutdownOptions()
{
    int btn = menuBar.showDialog("Power", "Shutdown Options", POWER_DIALOG);
    if (btn == SHUTDOWN)
    {
        menuBar.showDialog("Power", "Shutting down...", 0, 10);
        system("sudo shutdown now");
    }
    else if (btn == RESTART)
    {
        menuBar.showDialog("Power", "Restarting .....", 0, 10);
        system("reboot");
    }
    else if (btn == CANCEL)
    {
        digitalWrite(BL, LOW);
        LCDclear();
        LCDdisplay();
        exit(0);
    }
}
