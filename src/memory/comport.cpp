#include "comport.h"

COMPort::COMPort() : hComm(INVALID_HANDLE_VALUE) {}
COMPort::~COMPort()
{
	if (hComm != INVALID_HANDLE_VALUE)
		CloseHandle(hComm);
}

bool COMPort::open(const std::string &name)
{
	hComm = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
		return false;
	DCB dcb;
	//dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(hComm, &dcb))
		return false;
	BuildCommDCBA("baud=230400 parity=E data=8 stop=1", &dcb);
	/*dcb.BaudRate = 9600;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.fParity = TRUE;
	dcb.Parity = PARITY_ODD;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;*/
	if (!SetCommState(hComm, &dcb))
		return false;
	//SetCommMask(hComm, EV_RXCHAR);
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts(hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
	SetCommTimeouts(hComm, &CommTimeOuts);
	return true;
}

void COMPort::write(const unsigned char *buf, size_t length)
{
	DWORD dwWrite;
	WriteFile(hComm, buf, length, &dwWrite, NULL);
}

size_t COMPort::read(unsigned char *buf, size_t length)
{
	DWORD dwRead;
	if (!ReadFile(hComm, buf, length, &dwRead, NULL))
		return 0;
	return dwRead;
}