#pragma once

#ifndef CRC8_H
#define CRC8_H

#include <vector>
#include <cstdint>

class CRC8
{
public:
	CRC8();
	~CRC8();

	void calculate(std::vector<unsigned char> *data);
};

#endif // !CRC8_H
