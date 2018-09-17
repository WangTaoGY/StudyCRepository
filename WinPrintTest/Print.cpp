#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <winspool.h>
#include <string>
#include "ToolFunctions.h"
#include "Struct.h"
#include "Print.h"
#pragma comment(lib, "Winspool.lib")
using std::wstring;
using namespace std;

/*************************��ѯ��ӡ��������Ϣ********************************/
int getPrinterDriverInfo()
{
	std::hash<std::wstring> hash;
	DWORD printers_size = 0;
	DWORD printers_size_bytes = 0, dummyBytes = 0;
	DWORD Level = 2;
	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;     // https://msdn.microsoft.com/en-us/library/cc244669.aspx
	// First try to retrieve the number of printers
	BOOL bError = EnumPrintersW(flags, NULL, 2, NULL, 0, &printers_size_bytes, &printers_size);
	// allocate the required memmory

	PRINTER_INFO_2W* pInfos = (PRINTER_INFO_2W*)malloc(printers_size_bytes);
	bError = EnumPrintersW(flags, NULL, 2, (LPBYTE)(pInfos), printers_size_bytes, &dummyBytes, &printers_size);
	if (!bError)
	{
		free(pInfos);
		return 0;
	}
	// v8::Local<v8::Array> result = V8_VALUE_NEW_V_0_11_10(Array, printers_size);
	// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd162845(v=vs.85).aspx
	PRINTER_INFO_2W *printer = pInfos;

	for (DWORD i = 0; i < printers_size; ++i, ++printer)
	{
		struct PrinterInfo printerInfo;
		//printerInfo.id = id++;
		printerInfo.name = printer->pPrinterName;
		printerInfo.connected = !(printer->Attributes & PRINTER_ATTRIBUTE_WORK_OFFLINE);
		printerInfo.local_port = printer->pPortName;
		printerInfo.model = printer->pDriverName;
		size_t id = hash(printerInfo.model + printerInfo.name);
		printerInfo.id = id;

		size_t pos = printerInfo.local_port.find_first_of(L':');
		if (pos != std::wstring::npos) {
			printerInfo.local_port = printerInfo.local_port.substr(0, pos);
		}

		std::wstring portName = printerInfo.local_port;
		transform(portName.begin(), portName.end(), portName.begin(), ::toupper);

		if (portName.find(L"NUL") == 0 ||
			portName.find(L"PORTPROMPT") == 0 ||
			portName.find(L"FILE") == 0 ||
			portName.find(L"SHRFAX") == 0 ||
			portName.find(L"PDF-XChange") == 0 ||
			portName.find(L"Microsoft Document") == 0 ||
			portName.find(L"FOXIT_READER") == 0 ||
			portName.find(L"XPSPORT") == 0){
			printerInfo.type = 0;
			continue;
		}
		printerInfo.type = 1;
		cout << "-----------------------------------------" << endl;
		cout << "��ӡ��id:" << id << endl;
		cout << "��ӡ������:" << WcharToChar(printer->pPrinterName) << endl;
		cout << "��������:" << WcharToChar(printer->pDriverName) << endl;
		cout << "�˿�����:" << WcharToChar(printer->pPortName) << endl;
		cout << "ֽ�ſ��:" << printer->pDevMode->dmPaperWidth << endl;
		cout << "��ӡ���ֱ���:" << printer->pDevMode->dmPrintQuality << endl;
	}
	free(pInfos);
	return 0;
}


/*************************��ѯ��ӡ������������Ϣ********************************/
int getDriverTaskList()
{
	std::hash<std::wstring> hash;
	DWORD printers_size = 0;
	DWORD printers_size_bytes = 0, dummyBytes = 0;
	DWORD Level = 2;
	DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;

	//��һ�ε��ã���ȡ��ӡ�������������ڴ��С
	BOOL bError = EnumPrintersW(flags, NULL, 2, NULL, 0, &printers_size_bytes, &printers_size);

	PRINTER_INFO_2W* pInfos = (PRINTER_INFO_2W*)malloc(printers_size_bytes);

	//�ڶ��ε��ã���ѯ��ӡ�������б�
	bError = EnumPrintersW(flags, NULL, 2, (LPBYTE)(pInfos), printers_size_bytes, &dummyBytes, &printers_size);
	if (!bError)
	{
		free(pInfos);
		return 0;
	}
	PRINTER_INFO_2W *printer = pInfos;

	string printerName = "80mmSPRT251";
	/*wstring printerNameW;
	StringToWString(printerName, printerNameW);*/

	for (DWORD i = 0; i < printers_size; ++i, ++printer)
	{
		if (wcscmp(printer->pPrinterName, s2w(printerName, printerName.length())) == 0)
		{
			HANDLE hdcprint;
			if (OpenPrinter((LPWSTR)(LPCWSTR)(printerName.c_str()), &hdcprint, NULL) != 0)
			{
				DWORD job_size = 0;
				DWORD job_size_bytes = 0, dummyBytes = 0;

				BOOL bError = EnumJobs(hdcprint, 0, 0xFFFFFFFF, 1, 0, 0, &job_size_bytes, &job_size);

				_JOB_INFO_1W* jobInfos = (_JOB_INFO_1W*)malloc(job_size_bytes);

				bError = EnumJobs(hdcprint, 0, 0xFFFFFFFF, 1, (LPBYTE)(jobInfos), job_size_bytes, &dummyBytes, &job_size);
				if (!bError)
				{
					free(jobInfos);
					return 0;
				}

				_JOB_INFO_1W *jobs = jobInfos;
				for (DWORD i = 0; i < job_size; ++i, ++jobs)
				{
					cout << "��" << i << "����ӡ����" << endl;
					cout << "����idΪ��" << jobs->JobId << endl;
					cout << "��ӡ������Ϊ��" << WcharToChar(jobs->pPrinterName) << endl;
					cout << "������ӡ�������������Ϊ��" << WcharToChar(jobs->pMachineName) << endl;
					cout << "�����û���Ϊ��" << WcharToChar(jobs->pUserName) << endl;
					cout << "pStstusֵΪ��" << jobs->pStatus << endl;
					cout << "����״̬Ϊ��" << jobs->Status << endl;
					cout << endl;
				}
				ClosePrinter(hdcprint);
				free(jobInfos);
			}
			else
			{
				cout << "�򿪴�ӡ��ʧ�ܣ�" << endl;
				cout << "Last Error Code is:" << GetLastError() << endl;
			}
		}

	}
	free(pInfos);
}