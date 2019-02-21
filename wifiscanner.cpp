#include "wifiscanner.h"

WifiScanner::WifiScanner()
{
}

void split(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

vector<WifiAP> WifiScanner::scan()
{
    vector<WifiAP> apList;
    string output = exec("sudo iwlist wlan0 scanning | egrep 'Quality|ESSID'");
    vector<string> lines;
    split(output, '\n', lines);
    string ssid;
    double quality;
    for (int i = 0; i < lines.size(); i++)
    {
        if (i % 2 == 0)
        {
            string::size_type sz;
            quality = stod (trim(lines[i]).substr(8, 2),&sz);
        }
        else
        {
            ssid = trim(lines[i]);
            ssid = ssid.substr(7, ssid.size() - 8);
            WifiAP ap;
            ap.ssid = ssid;
            ap.quality = quality;
            apList.push_back(ap);
        }
    }
    return apList;
}

int WifiScanner::connect(string ssid, string pwd)
{
    string cmd = "nmcli device wifi connect '" + ssid + "' password '" + pwd + "'";
    string response = exec(cmd.c_str());
    if (response.substr(0, 5).compare("Error") == 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
