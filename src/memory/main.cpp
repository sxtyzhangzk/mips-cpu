#include "comm.h"
#include <cstdio>

int main()
{
	COMPort com;
	if (!com.open("com4"))
	{
		fprintf(stderr, "Failed to open serial port\n");
		return 1;
	}
	Comm comm(com);

	const unsigned int MEMORY_SIZE = 8192;
	unsigned char *memory = new unsigned char[MEMORY_SIZE];
	memset(memory, 0, MEMORY_SIZE);

	FILE *fram = fopen(R"(D:\Project\Vivado\mips_cpu\testcase\mips.bin)", "rb");
	fread(memory, sizeof(unsigned char), MEMORY_SIZE, fram);
	//for(int i=0; i<1000; i++)
	//	comm.write({ 2, 3, 3, 3 });
	/*comm.always_read([&](const std::vector<unsigned char> &data)
	{
		unsigned int num = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
		num++;
		comm.write({ unsigned char(num), unsigned char(num >> 8), 
			unsigned char(num >> 16), unsigned char(num >> 24) });
	});*/
	comm.always_read([&](const std::vector<unsigned char> &data)
	{
		if (data.size() == 5 && data[4] == 0)
		{
			unsigned int addr = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
			if (addr >= MEMORY_SIZE)
				printf("Read Addr 0x%08x out of range!\n", addr);
			else
			{
				printf("Read Addr 0x%08x: %02x%02x%02x%02x\n", addr, (int)memory[addr + 3], (int)memory[addr + 2], (int)memory[addr + 1], memory[addr]);
				comm.write({ memory[addr], memory[addr + 1], memory[addr + 2], memory[addr + 3] });
			}
		}
		else if (data.size() == 9)
		{
			unsigned int addr = data[4] | data[5] << 8 | data[6] << 16 | data[7] << 24;
			unsigned int wdata = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
			printf("Write to Addr 0x%08x: %08x, Mask = %d\n", addr, wdata, data[8] & 0b1111);
			if (addr >= MEMORY_SIZE)
			{
				printf("Write addr out of range!");
			}
			else
			{
				if (data[8] & 0b0001)
					memory[addr] = data[0];
				if (data[8] & 0b0010)
					memory[addr + 1] = data[1];
				if (data[8] & 0b0100)
					memory[addr + 2] = data[2];
				if (data[8] & 0b1000)
					memory[addr + 3] = data[3];
			}
		}
	});
	return 0;
}