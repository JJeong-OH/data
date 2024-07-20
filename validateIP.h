#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool isNumber(const string& str)
{
    return !str.empty() && (str.find_first_not_of("0123456789") == string::npos);
}

vector<string> split(const string& str, char delim)
{
    auto i = 0;
    vector<string> list;

    auto pos = str.find(delim);

    while (pos != string::npos) {
        list.push_back(str.substr(i, pos - i));
        i = (int)++pos;
        pos = str.find(delim, pos);
    }

    list.push_back(str.substr(i, str.length()));

    return list;
}

bool validateIP(string ip)
{
    vector<string> list = split(ip, '.');

    if (list.size() != 4) {
        return false;
    }

    for (string str : list) {
        if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0) {
            return false;
        }
    }
    return true;
}