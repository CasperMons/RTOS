#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>

#define PIN 7 // GPIO pin 4
#define COUNTER 50000

int count = 0;
clock_t startTime;

void pinHandler(void){
	if(count == 1){
		startTime = clock();
	}
//	printf("Count %u \n", count);
	count++;
}

int main(int argc, char **argv){
	printf("Starting setup...\n");
	printf("Pin %u will be read %u times\n", PIN, COUNTER);

	if(wiringPiSetup() == -1){
		return -1;
	}

	pinMode(PIN, INPUT);
	printf("Setup wiringPi pin %u to input\n", PIN);

	wiringPiISR(PIN, INT_EDGE_RISING, &pinHandler);

	while(1){
		if(count > COUNTER){
			break;
		}
	}

	clock_t endTime = clock(); - startTime;
	int sec = endTime / CLOCKS_PER_SEC;

	printf("Done, WiringPi pin %u is read %u times\n", PIN, count);
	printf("Execution time: %d seconds\n", sec);

	return 0;
}
