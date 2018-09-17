#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <winspool.h>
#include <string>
#include "ToolFunctions.h"
#pragma comment(lib, "Winspool.lib")
using std::wstring;
using namespace std;

char* WcharToChar(const wchar_t* wp)
{
	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

struct PrinterInfo{
	std::wstring name;
	bool connected;
	std::wstring local_port;
	size_t id;
	std::wstring model;
	int type;
};

BOOL StringToWString(const std::string &str, std::wstring &wstr)
{
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');
	
	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
	
	if (nResult == 0)
	{
		return FALSE;
	}
	
	return TRUE;
}

/*************************查询打印机驱动信息********************************/
//int main()
//{
//	std::hash<std::wstring> hash;
//	DWORD printers_size = 0;
//	DWORD printers_size_bytes = 0, dummyBytes = 0;
//	DWORD Level = 2;
//	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS; // https://msdn.microsoft.com/en-us/library/cc244669.aspx
//	// First try to retrieve the number of printers
//	BOOL bError = EnumPrintersW(flags, NULL, 2, NULL, 0, &printers_size_bytes, &printers_size);
//	// allocate the required memmory
//
//	PRINTER_INFO_2W* pInfos = (PRINTER_INFO_2W*)malloc(printers_size_bytes);
//	bError = EnumPrintersW(flags, NULL, 2, (LPBYTE)(pInfos), printers_size_bytes, &dummyBytes, &printers_size);
//	if (!bError)
//	{
//		free(pInfos);
//		return 0;
//	}
//	// v8::Local<v8::Array> result = V8_VALUE_NEW_V_0_11_10(Array, printers_size);
//	// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd162845(v=vs.85).aspx
//	PRINTER_INFO_2W *printer = pInfos;
//
//	for (DWORD i = 0; i < printers_size; ++i, ++printer)
//	{
//		struct PrinterInfo printerInfo;
//		//printerInfo.id = id++;
//		printerInfo.name = printer->pPrinterName;
//		printerInfo.connected = !(printer->Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE);
//		printerInfo.local_port = printer->pPortName;
//		printerInfo.model = printer->pDriverName;
//		size_t id = hash(printerInfo.model + printerInfo.name);
//		printerInfo.id = id;
//
//		size_t pos = printerInfo.local_port.find_first_of(L':');
//		if (pos != std::wstring::npos) {
//			printerInfo.local_port = printerInfo.local_port.substr(0, pos);
//		}
//
//		std::wstring portName = printerInfo.local_port;
//		transform(portName.begin(), portName.end(), portName.begin(), ::toupper);
//
//		if (portName.find(L"NUL") == 0 ||
//			portName.find(L"PORTPROMPT") == 0 ||
//			portName.find(L"FILE") == 0 ||
//			portName.find(L"SHRFAX") == 0 ||
//			portName.find(L"PDF-XChange") == 0 ||
//			portName.find(L"Microsoft Document") == 0 ||
//			portName.find(L"FOXIT_READER") == 0 ||
//			portName.find(L"XPSPORT") == 0){
//			printerInfo.type = 0;
//			continue;
//		}
//		printerInfo.type = 1;
//		cout << "-----------------------------------------" << endl;
//		cout << "打印机id:" << id << endl;
//		cout << "打印机名称:" << WcharToChar(printer->pPrinterName) << endl;
//		cout << "驱动名称:" << WcharToChar(printer->pDriverName) << endl;
//		cout << "端口名称:" << WcharToChar(printer->pPortName) << endl;
//		cout << "纸张宽度:" << printer->pDevMode->dmPaperWidth << endl;
//		cout << "打印机分辨率:" << printer->pDevMode->dmPrintQuality << endl;
//	}
//	free(pInfos);
//	return 0;
//}


/*************************查询打印机驱动队列信息********************************/
//int main(){
//	std::hash<std::wstring> hash;
//	DWORD printers_size = 0;
//	DWORD printers_size_bytes = 0, dummyBytes = 0;
//	DWORD Level = 2;
//	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
//
//	//第一次调用，获取打印机驱动数量和内存大小
//	BOOL bError = EnumPrintersW(flags, NULL, 2, NULL, 0, &printers_size_bytes, &printers_size);
//
//	PRINTER_INFO_2W* pInfos = (PRINTER_INFO_2W*)malloc(printers_size_bytes);
//
//	//第二次调用，查询打印机驱动列表
//	bError = EnumPrintersW(flags, NULL, 2, (LPBYTE)(pInfos), printers_size_bytes, &dummyBytes, &printers_size);
//	if (!bError)
//	{
//		free(pInfos);
//		return 0;
//	}
//	PRINTER_INFO_2W *printer = pInfos;
//
//	string printerName = "80mmSPRT251";
//	/*wstring printerNameW;
//	StringToWString(printerName, printerNameW);*/
//
//	for (DWORD i = 0; i < printers_size; ++i, ++printer)
//	{
//		if (wcscmp(printer->pPrinterName, s2w(printerName, printerName.length())) == 0)
//		{
//			HANDLE hdcprint;
//			if (OpenPrinter((LPWSTR)(LPCWSTR)(printerName.c_str()), &hdcprint, NULL) != 0)
//			{
//				DWORD job_size = 0;
//				DWORD job_size_bytes = 0, dummyBytes = 0;
//
//				BOOL bError = EnumJobs(hdcprint, 0, 0xFFFFFFFF, 1, 0, 0, &job_size_bytes, &job_size);
//
//				_JOB_INFO_1W* jobInfos = (_JOB_INFO_1W*)malloc(job_size_bytes);
//
//				bError = EnumJobs(hdcprint, 0, 0xFFFFFFFF, 1, (LPBYTE)(jobInfos), job_size_bytes, &dummyBytes, &job_size);
//				if (!bError)
//				{
//					free(jobInfos);
//					return 0;
//				}
//
//				_JOB_INFO_1W *jobs = jobInfos;
//				for (DWORD i = 0; i < job_size; ++i, ++jobs)
//				{
//					cout << "第" << i << "个打印任务：" << endl;
//					cout << "任务id为：" << jobs->JobId << endl;
//					cout << "打印机名称为：" << WcharToChar(jobs->pPrinterName) << endl;
//					cout << "创建打印任务机器的名称为：" << WcharToChar(jobs->pMachineName) << endl;
//					cout << "创建用户名为：" << WcharToChar(jobs->pUserName) << endl;
//					cout << "pStstus值为：" << jobs->pStatus << endl;
//					cout << "任务状态为：" << jobs->Status << endl;
//					cout << endl;
//				}
//				ClosePrinter(hdcprint);
//				free(jobInfos);
//			}
//			else
//			{
//				cout << "打开打印机失败！" << endl;
//				cout << "Last Error Code is:" << GetLastError() << endl;
//			}
//		}
//
//	}
//	free(pInfos);
//}