
// main.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[]) {
	struct	sockaddr_un soc;
	int 	id, status, sockfd;
	char	*lanName;
	
	// Check to make sure there are at least 2 arguements
	if(argc < 3) {
		printf("Usage: 3700client <id> <LAN> [<LAN> [<LAN ...]]\n");
		return 0;
	}
	
	sscanf(argv[1], "%x", &(id));	// Get the bridge ID

	// Get the LAN name, with '\0' prepended 
	lanName = (char *) malloc(strlen(argv[2] + 2));
	lanName[0] = 0;
#if 0
	lanName[0] = '0';
#endif
	strcpy(lanName + 1, argv[2]);

	printf("Bridge %04x starting up\n", id);

	// First create a socket
	sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(sockfd == -1) {
		char * error_buffer = malloc(256);
		sprintf(error_buffer,
			"Error: failed to create socket.");
		perror(error_buffer);
		return 0;
	}

	soc.sun_family = AF_UNIX;
	// Can't use str_cpy because null character
	soc.sun_path[0] = lanName[0];
	strcpy(&(soc.sun_path[1]), &(lanName[1]));

	status = connect(sockfd, (struct sockaddr *) &soc, sizeof(soc));
	if(status == -1) {
		char * error_buffer = malloc(256);
		sprintf(error_buffer,
			"Error: failed to connect to server at '\\0%s' ",
			soc.sun_path + 1);
		perror(error_buffer);
		return 0;
	}

	
	return 0;
}
