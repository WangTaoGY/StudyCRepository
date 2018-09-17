#include <windows.h>  
#include <iostream>  
#include <string> 
#include <winnt.h>
#include <stdio.h>
#include <stdlib.h>
#include <winspool.h>
#include "ToolFunctions.h"

using namespace std;


/**
*  功能描述: 小票打印函数
*  @param char* jsonText
*  @param Printer printer
*  @param string &ResponseMessage
*  @return bool
*/

int hexCharToInt(char c)
{
	if (c >= '0' && c <= '9') return (c - '0');
	if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
	return 0;
}

//16进制字符串转成char[]
void hexstringToBytes(string s, char* ret)
{
	int sz = s.length();

	for (int i = 0; i <sz; i += 2) {
		ret[i / 2] = (char)((hexCharToInt(s.at(i)) << 4)
			| hexCharToInt(s.at(i + 1)));
	}

	return;
}


/**
*  功能描述: 将string 转换为wchar_t*
*  @param string
*  @param int
*  @return wchar_t*
*/
wchar_t* s2w(string &content, int length)
{
	const char *str = content.c_str();
	length = strlen(str) + 1;
	wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(t, 0, length*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
	return t;
}

//void main()
//{
//	char picChar[10];
//	picChar[0] = 0x1c;
//	picChar[1] = 0x21;
//	picChar[2] = 0x00;
//	picChar[3] = 0x1b;
//
//	int size = 0;
//	for (size_t i = 0; i < 10; i++)
//	{
//		if (picChar[i] == null)
//		{ 
//			break;
//		}
//		size++;
//
//	}
//	cout << size << endl;
//
//}

void containCh(string &content){
	int i = 0, nLen = content.length();

	//char szText[] = const_cast<char>(content.data());

	boolean containCh = false;

	for (; i < nLen; i++)
	{
		if (!(content[i] >= 0 && content[i] <= 127)) //是全角字符？ 
		{
			containCh = true;
		}
	}

	if (containCh)
	{
		cout << content << "包含中文字符！" << endl;
	}
	else
	{
		cout << content << "不包含中文字符！" << endl;
	}
}



bool isIPAddress(const char *s)
{
	const char *pChar;
	bool rv = true;
	int tmp1, tmp2, tmp3, tmp4, i;


	while (1)
	{
		i = sscanf_s(s, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);


		if (i != 4)
		{
			rv = false;
			break;
		}


		if ((tmp1 > 255) || (tmp2 > 255) || (tmp3 > 255) || (tmp4 > 255))
		{
			rv = false;
			break;
		}


		for (pChar = s; *pChar != 0; pChar++)
		{
			if ((*pChar != '.')
				&& ((*pChar < '0') || (*pChar > '9')))
			{
				rv = false;
				break;
			}
		}
		break;
	}


	return rv;
}



