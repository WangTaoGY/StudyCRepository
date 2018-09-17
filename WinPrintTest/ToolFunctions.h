#include <string> 
using namespace std;

int hexCharToInt(char c);

void hexstringToBytes(string s, char* ret);

wchar_t* s2w(string &content, int length);

void containCh(string &content);

bool isIPAddress(const char *s);

char* WcharToChar(const wchar_t* wp);

BOOL StringToWString(const std::string &str, std::wstring &wstr);