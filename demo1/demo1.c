/*
	demo-01: create a TCP/IP socket

	usage:	demo-01 
*/

#include <stdio.h>	/* defines printf */
#include <stdlib.h>	/* defines exit and other sys calls */
#include <sys/socket.h>

int main(int argc, char **argv)
{
	int fd;

	/* create a tcp/ip socket */
	/* request the Internet address protocol */
	/* and a reliable 2-way byte stream (TCP/IP) */

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}

	printf("created socket: descriptor=%d\n", fd);
	return 0;
}
