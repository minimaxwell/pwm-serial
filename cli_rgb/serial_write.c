/**
 * Lecture des données sur le port serie.
 * credit : https://github.com/cheydrick/Canonical-Arduino-Read/blob/master/canonicalarduinoread.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#define DEBUG 1


int serial_open(char *path, int speed) {

	int fd;

	struct termios toptions;
	fd = open(path, O_RDWR | O_NOCTTY);

	if (fd < 0)
		printf("Error opening %s\n", path);

	tcgetattr(fd, &toptions);
	cfsetispeed(&toptions, speed);
	cfsetospeed(&toptions, speed);
	cfmakeraw(&toptions);
	tcsetattr(fd, TCSANOW, &toptions);

	return fd;
}

int main(int argc, char *argv[])
{
	int fd;

	if ( argc != 6  ) {
		printf("usage : ./serial_write path led r g b\n");
		return 1;
	}

	/* open serial port */
	fd = serial_open(argv[1], B57600);

	uint8_t data[4];

	data[3] = (uint8_t)atoi(argv[2]);
	data[0] = (uint8_t)atoi(argv[3]);
	data[1] = (uint8_t)atoi(argv[4]);
	data[2] = (uint8_t)atoi(argv[5]);

	write(fd, data, 4);

	close(fd);

	return 0;
}
