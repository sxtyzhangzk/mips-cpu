#pragma once

#include <string>

#include <Windows.h>

class COMPort
{
public:
	COMPort();
	~COMPort();

	bool open(const std::string &name);
	void write(const unsigned char *buf, size_t length);
	size_t read(unsigned char *buf, size_t length);

protected:
	HANDLE hComm;
};