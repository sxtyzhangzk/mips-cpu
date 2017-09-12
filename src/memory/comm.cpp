#include "comm.h"
#include <cassert>
#include <bitset>
#include <iostream>

void Comm::write(const std::vector<unsigned char> &data)
{
	assert(data.size() <= 9);
	sendPacketID++;
	unsigned char buf[50];
	buf[0] = (0b100 << 5) | (sendPacketID & 0b11111);
	buf[1] = 0b10100000;
	buf[2] = (0b110 << 5) | (data.size() & 0b11111);
	
	std::bitset<72> bits = 0;
	for (size_t i = 0; i < data.size(); i++)
		for (size_t j = 0; j < 8; j++)
			bits[i * 8 + j] = data[i] & (1 << j) ? 1 : 0;

	size_t i = 3;
	for (size_t sent = 0; sent < data.size() * 8; sent += 7, i++)
	{
		buf[i] = 0;
		for (size_t j = 0; j < 7; j++)
			buf[i] |= bits[sent + j] << j;
	}
	buf[i] = (0b111 << 5) | (sendPacketID & 0b11111);

	com.write(buf, i+1);
}

void Comm::always_read(std::function<void(const std::vector<unsigned char> &)> callback)
{
	enum states { IDLE, CHANNEL, LENGTH, DATA, END };
	states state = IDLE;
	
	size_t length_bit;
	size_t packet_id;
	size_t recv_bit;
	std::bitset<72> bits;

	auto read_packet = [&](unsigned char packet)
	{
		switch (state)
		{
		case IDLE:
			if (packet >> 5 == 0b100)
			{
				packet_id = packet & 0b11111;
				if (packet_id != ((recvPacketID + 1) & 0b11111))
					std::cerr << "Lose Packet!" << std::endl;
				recv_bit = 0;
				state = CHANNEL;
			}
			else
				std::cerr << "Corrupted Packet @IDLE" << std::endl;
			break;

		case CHANNEL:
			if (packet >> 5 == 0b101)
				state = LENGTH;
			else
			{
				state = IDLE;
				std::cerr << "Corrupted Packet @Channel" << std::endl;
			}
			break;

		case LENGTH:
			if (packet >> 5 == 0b110)
			{
				length_bit = (packet & 0b11111) * 8;
				state = DATA;
				bits.reset();
			}
			else
			{
				state = IDLE;
				std::cerr << "Corrupted Packet @Length" << std::endl;
			}
			break;

		case DATA:
			if (packet >> 7 == 0b0)
			{
				for (size_t i = 0; i < 7 && recv_bit < length_bit; i++, recv_bit++)
					bits[recv_bit] = packet & (1 << i) ? 1 : 0;
				if (recv_bit == length_bit)
					state = END;
			}
			else
			{
				state = IDLE;
				std::cerr << "Corrupted Packet @Data." << recv_bit / 7 << std::endl;
			}
			break;

		case END:
			if (packet >> 5 == 0b111)
			{
				if (packet_id == (packet & 0b11111))
				{
					recvPacketID = packet_id;
					std::vector<unsigned char> result;
					size_t write_bit = 0;
					for (size_t write_bit = 0; write_bit < recv_bit; write_bit += 8)
					{
						char tmp = 0;
						for (size_t j = 0; j < 8; j++)
							tmp |= bits[write_bit + j] << j;
						result.push_back(tmp);
					}
					callback(result);
				}
			}
			else
			{
				std::cerr << "Corrupted Packet @End" << std::endl;
			}
			state = IDLE;
		}
	};

	while (true)
	{
		unsigned char tmp;
		size_t len = com.read(&tmp, 1);
		if (len == 0)
			break;
		//std::cerr << "Read " << std::hex << (int)tmp << std::endl;
		//for (size_t i = 0; i < len; i++)
			//read_packet(tmp[i]);
		read_packet(tmp);
	}
}