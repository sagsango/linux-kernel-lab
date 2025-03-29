#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFF_SIZE] = {0};
	char *hello = "Hello from server";
	int ret = 0;

	/* [S1: point 1]
	 * socket systems call with arguments:
	 * @1: domain = AF_INET, IPv4 Internet protocols
	 * @2: type = SOCK_STREAM, TCP connection
         * @3: protocol = 0, for SOCK_STREAM
	 */
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	/* [S2: point 1]
	 * setsockopt systems call for getting & seting the options on socket with the following args:
         * @1: fd = server_fd, which is from previously created socket
         * @2: level = SOL_SOCKET, socket API level
         * @3: option = SO_REUSEADDR | SO_REUSEPORT, so multiple sockets can be created with same addr and port
         *          and message will be distributed between them
         * @4: &opt = if we want to set the options or disable them, opt = 1, so set them
         * 
	 */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		       &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	/* [S3: point 1]
	 * sin_family = AF_INET, which is ipv4 addr
         * s_addr = INADDR_ANY = 0.0.0.0; to accept the connection at any interface
         * sin_port = htons( PORT); assign the port, but we have to convert the port number to network representation
         *            because on machine it can be big-endian or small-endian. 
	 */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	/* [S4: point 1]
	 * bind systems call to assign a socket an address with the following args:
         * @1: server_fd: sockert file discripter on which we want to assign the address
         * @2: &address: address of the sockaddr struct
         * @3: sizeof(address): size of the sockaddr_in
	 */
	if (bind(server_fd, (struct sockaddr *)&address,
		 sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/* [S5: point 1]
	 * listen systems call makrs socket in listening state so that it can accpect in incomming connections, agrs:
	 * @1: socket file discripter
         * @2: max no of pening connections in the queue, after that connections may get rejectec.
	 */
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* [S6: point 1]
	 * accept systems call to accept the incoming the connection, args:
         * @1: server fd, which want to accept the connection
         * @2: buffer address, where peer's address will be written after accepting the connection
         * @3: Length of the bufer
	 */
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
				 (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("Press any key to continue...\n");
	getchar();

	/* [S7: point 1]
	 * read systems call with args:
         * @1: fd on which we want to do read (fd can be file, socket, pipe, shm or messeging queue)
         * @2: buffer address where we want to write the content
         * @3: size of the buffer
	 */
	ret = read(new_socket, buffer, 1024);
	printf("Read %d bytes of message from a client: %s\n", ret, buffer);

	/* [S8: point 1]
	 * systems call to send the data on the client fd, which args:
	 * @1: client's FD
         * @2: pointer to the message
         * @3: size of the messges
         * @4: sending option, this can be nonblocking etc (here we have blocking one)
	 */
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	return 0;
}
