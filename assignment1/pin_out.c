#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>

#define PIN 5 // GPIO pin 24
//#define COUNTER 50000000

int main (int argc, char **argv)
{
  printf ("Starting setup...\n") ;

  printf("Setting pin %u high and low\n", PIN);

  if (wiringPiSetup () == -1)
    return 1 ;

  pinMode (PIN, OUTPUT);

  printf("Setup pin %u to output\n", PIN);
  printf("Sending pulses to pin %u...\n", PIN);
//  int count = 0;

  while(1)
  {
    digitalWrite (PIN, HIGH) ;
//    delay(0);
    digitalWrite (PIN, LOW) ;
//    delay(0);
//    count++;
  }

  printf("Done\n");

  return 0 ;
}
