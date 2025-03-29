#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[BUFF_SIZE] = {0};
	int ret = 0;

	/* [C1: point 1]
         * socket systems call with arguments:
         * @1: domain = AF_INET, IPv4 Internet protocols
         * @2: type = SOCK_STREAM, TCP connection
         * @3: protocol = 0, for SOCK_STREAM
	 */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	/* [C2: point 1]
	 * setting the memoery to '0' (char) so whole struct contains the '0' in every byte
         * then assigning the addr family to ipv6 by AF_INET
         * then assigning the port by converting the host bytes to network bytes
	 */
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	/* [C3: point 1]
	 * inet_pton convert the string formate of the host ipv4 addr to network ip addr 
	 */
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	/* [C4: point 1]
	 * connect syscall to send the connection request to the peers, with args:
         * @1: socket fd trough which we want to make connection
         * @2: address of the buffer contating the peer's address
         * @3: size of the address
	 */
	if (connect(sock, (struct sockaddr *)&serv_addr,
		    sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("Press any key to continue...\n");
	getchar();

	/* [C5: point 1]
	 * systems call to send the data on the client fd, which args:
         * @1: client's FD
         * @2: pointer to the message
         * @3: size of the messges
         * @4: sending option, this can be nonblocking etc (here we have blocking one)
	 */
	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	/* [C6: point 1]
	 * read systems call with args:
         * @1: fd on which we want to do read (fd can be file, socket, pipe, shm or messeging queue)
         * @2: buffer address where we want to write the content
         * @3: size of the buffer
	 */
	ret = read(sock, buffer, 1024);
	printf("Read %d bytes of message from a server: %s\n", ret, buffer);

	return 0;
}
