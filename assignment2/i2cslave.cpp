#include <pigpio.h>
#include <iostream>
#include <atomic>
#include <signal.h>
#include <cstring>
#include "CRC8.h"
#include <cstdlib>

// g++ i2cslave.cpp CRC8.cpp -o slave -lpigpio -lpthread -lrt

#define BUFFLEN 12

std::atomic<bool> Quit = ATOMIC_VAR_INIT(false);

void quit(int sig){
    Quit = true; //will require handler quit too
}

int main(int argc, char** argv)
{
    CRC8 checker;
    int receiveCounter = 0;
    int validCounter = 0;
    int count = 500;

    if(argc > 1){
	count = atoi(argv[1]);
    }
    std::vector<unsigned char> data;
    signal(SIGINT, quit);
    bsc_xfer_t xfer;

    gpioInitialise();

    xfer.control = (0x0A<<16) | 0x305;

    int status = bscXfer(&xfer);
    if (status >= 0)
    {
        xfer.rxCnt = 0;

        while(!Quit)
        {
            status = bscXfer(&xfer);
            if(status)
            {
                if (xfer.rxCnt > 0)
             	{
//                  std::cout << std::endl << xfer.rxBuf << std:: endl;
		    receiveCounter++;

                    data.assign(xfer.rxBuf, xfer.rxBuf + BUFFLEN - 1);

		    checker.calculate(&data);

		    if((char)xfer.rxBuf[BUFFLEN - 1] == (char)data.back()){
//			std::cout << "Checksum valid" << std::endl;
			validCounter++;
		   }else{
//			std::cout << "Checksum invalid" << std::endl;
		   }
		    memset( xfer.rxBuf, '\0', sizeof(char)*BSC_FIFO_SIZE );
                }
            }
         	if(receiveCounter == count){
			break;
		}
	}

	std::cout << "Received data objects: " << receiveCounter << std::endl;
	std::cout << "Valid data objects:    " << validCounter << std::endl;
	std::cout << "Invalid data objects:  " << receiveCounter - validCounter << std:: endl;
    }
    else
    {
        std::cout<<"\nInit status error.\n"<<std::flush;
    }

    return status;
}
