#include <stdio.h>
#include <pigpio.h>
#include <string>
#include <pthread.h>
#include "CRC8.h"
#include <iostream>
#include <unistd.h>

int main(){

	CRC8 checker;

	int sleeper;
	std::cout << "Enter usleep value: ";
	std::cin >> sleeper;

	gpioInitialise();
	std::cout << "This is Sinterklaas speaking...\n" << std::endl;

	int handle = i2cOpen(1, 0x0A, 0);
	char* ding = "HotBox Piet";
	std::vector<unsigned char> stringding;

	stringding.assign(ding, ding+11);

	checker.calculate(&stringding);

	for(int i = 0; i < stringding.size(); i++){
		printf("0x%x ", stringding.at(i));
	}

	printf("\n");

//	for (int i = 0; i < 10; i++){
	int i = 0;
	while(1){
		i++;
		i2cWriteDevice(handle, reinterpret_cast<char*>(stringding.data()), stringding.size() );
		if(i % 1000 == 0){
			printf("%d\n", i);
		}
		usleep(sleeper);
	}
	return 0;
}
