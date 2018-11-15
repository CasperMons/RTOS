#include <stdio.h>
#include <pigpio.h>
#include <string>
#include <pthread.h>
#include "CRC8.h"
#include <iostream>
#include <unistd.h>

int main() {

	CRC8 checker;

	int sleepTime;
	std::cout << "Enter usleep value: ";
	std::cin >> sleepTime;

	gpioInitialise();
	std::cout << "This is the master speaking...\n" << std::endl;

	int handle = i2cOpen(1, 0x0A, 0);
	char* strData = "HotBox Piet";
	std::vector<unsigned char> vecData;

	vecData.assign(strData, strData + 11);

	checker.calculate(&vecData);

	for (int i = 0; i < vecData.size(); i++) {
		printf("0x%x ", vecData.at(i));
	}

	printf("\n");

	int i = 0;
	while (1) {
		i++;
		i2cWriteDevice(handle, reinterpret_cast<char*>(vecData.data()), vecData.size());
		if (i % 1000 == 0) {
			printf("%d\n", i);
		}
		usleep(sleepTime);
	}
	return 0;
}
