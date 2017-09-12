#pragma once

#include "comport.h"
#include <vector>
#include <functional>

class Comm
{
public:
	Comm(COMPort &com) : com(com), sendPacketID(0), recvPacketID(0) {}

	void always_read(std::function<void(const std::vector<unsigned char> &)> callback);
	void write(const std::vector<unsigned char> &data);

protected:
	COMPort &com;
	size_t recvPacketID;
	size_t sendPacketID;
};