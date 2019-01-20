// LockWire
// Written by: Kyle O'Rourke
// 1/19/2019
// CruzHacks UCSC

// Description:
// Electronic bike lock running on a DragonBoard 410c
// System uses an Accelerometer and GPS running on a
// Sparkfun 9-dof Razor IMU board. This software reads
// the incomming serial data from the Razor IMU board
// and determines if a theft has occured.

#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
using namespace std;

#define LINE_BUF_SIZE 128 // Used in the pipe.

// Arduino serial variables:
// Data format: "Loc: 36.997051, -122.052368"
const char * portname = "/dev/ttyUSB0";
unsigned char buf[1000];
int bufLen = 600;
float xCord, yCord; // Extracted location data.

int set_interface_attributes(int fd, int speed){
	struct termios tty;

	if(tcgetattr(fd, &tty) < 0){
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}

	cfsetospeed(&tty, (speed_t)speed);
	cfsetispeed(&tty, (speed_t)speed);

	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;
	
	
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;

	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;

	if(tcsetattr(fd, TCSANOW, &tty) != 0){
		printf("ERROR: from tcsetattr: %s\n", strerror(errno));
		return -1;
	}
	
	return 0;
}

void set_mincount(int fd, int mcount){
	struct termios tty;

	if(tcgetattr(fd, &tty) < 0){
		printf("Error: tcgetattr: %s\n", strerror(errno));
		return;
	}

	tty.c_cc[VMIN] = mcount ? 1 : 0;
	tty.c_cc[VTIME] = 5;

	if(tcsetattr(fd, TCSANOW, &tty) < 0){
	 	printf("Error: tcsetattr: %s\n", strerror(errno));
	}
}

int main(void){

	// Open file descriptor:
	int fd;
	int wlen;

	fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd < 0){
		printf("Failed to open serial port!\n");
		return -1;
	}
	printf("fd opened as %i\n", fd);
	
	set_interface_attributes(fd, B4800);
	//set_mincount(fd, 23);

	// Main loop to read data:
	int stringCount = 0;
	unsigned char builtBuffer[100];
	char line[LINE_BUF_SIZE];
	FILE *pipe;

	while(1){
		int rdlen = read(fd, buf, sizeof(buf)-1);
		if(rdlen > 0){
			unsigned char *p;
			//printf("Read %d:", rdlen); // Prints the incoming len.
			for(p = buf; rdlen > 0; p++){
				builtBuffer[stringCount] = *p;
				stringCount++;
				rdlen--;
				if(*p == ';'){
					stringCount--;
					string location = "";
					for(int i = 0; i < stringCount; i++){
						if(builtBuffer[i] >= 45 && builtBuffer[i] <= 57){
							location += builtBuffer[i];
						}
						//printf("%c", builtBuffer[i]);	
					}

					// builtBuffer[] - Holds the new coordinate data.
					// stringCount   - Is the length of the new data.
					string upload = "sudo curl -X POST 35.236.123.213:3000/location/" + location + "\n";	
					system(upload.c_str());
					cout << upload;
					stringCount = 0; // Now that we are done with the buffer, reset it.
				}
			}
		}
		else if(rdlen < 0){
			printf("Error from read: %d: %s\n", rdlen, strerror(errno));
		}
		else{
			printf("TIMOUT FROM READ\n");
		}
	}


	// Get acceleration values:
	// Future feature.
	
	// Print ID to terminal:
	cout << "END: LockWire\n";
	return 0; // END PROGRAM.
}


