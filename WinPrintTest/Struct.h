#include <string>

struct PrinterInfo{
	std::wstring name;
	bool connected;
	std::wstring local_port;
	size_t id;
	std::wstring model;
	int type;
};