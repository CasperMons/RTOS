#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <pthread.h>

#define PIN_IN 7 // GPIO pin 4
#define COUNTER 500
#define PIN_OUT 5 // GPIO pin 24

/*
	Compile this program with gcc pin_in_out.c -o pininout -lwiringPi -lpthread
*/

int readCount = 0; // Count how many times interrupt has been fired.
int writeCount = 0; // Count how many times the OUTPUT pin is toggled

double timeDiff = 0; // Difference in time every pin pulse and its handle

clock_t startTime; // Start time of the clock when a pulse is generated

pthread_t tid1; // Thread for toggling pulses

void* gpioToggle(){
	// Toggle the output pin and add 1 to writing count
	while(readCount < COUNTER){
		startTime = clock(); // Reset clock timer to now
		digitalWrite(PIN_OUT, HIGH);
		delay(0); // Need to call delay to prevent interrupt skipping pulses
		digitalWrite(PIN_OUT, LOW);
		delay(0);
		writeCount++;
	}
}

// Interrupt Handler for pin 
void pinHandler(void){
	startTime = clock() - startTime; // Compute difference in time
	double currTimeDiff = ((double)startTime)/CLOCKS_PER_SEC; // Get time in seconds
	timeDiff = timeDiff + currTimeDiff; // Add the new difference to the previous difference to get avg later on
	readCount++; // Add one to the reading count
}

int main(int argc, char **argv){
	printf("Starting setup...\n");
	printf("Pin %u will be read %u times\n", PIN_IN, COUNTER);

	if(wiringPiSetup() == -1){
		return -1;
	}
	// Setup WiringPi pins
	pinMode(PIN_IN, INPUT);
	pinMode(PIN_OUT, OUTPUT);

	printf("Setup wiringPi pin %u to input\n", PIN_IN);
	printf("Setup wiringPi pin %u to output\n", PIN_OUT);

	printf("Setup read handler on pin %u\n", PIN_IN);

	// Configure interrupt handler
	wiringPiISR(PIN_IN, INT_EDGE_RISING, &pinHandler);

	printf("Starting gpioToggle thread...\n");
	pthread_create(&tid1, NULL, gpioToggle, NULL);
	printf("Waiting until thread is finished...\n");
	pthread_join(tid1, NULL);

	printf("Done!\n");

	printf("WiringPi pin %u is read       %u times\n", PIN_IN, readCount);
	printf("WiringPi pin %u is written to %u times\n", PIN_OUT, writeCount);

	double avgLat = timeDiff / (double)readCount;
	printf("Average latency in seconds: %f milliseconds\n", avgLat*1000);
	printf("Skipped pulses: %u\n", writeCount - readCount);
	return 0;
}
