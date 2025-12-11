
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO

        ssize_t bytes = read(server, &req, sizeof(struct message));
        if (bytes != sizeof(struct message)) {
            continue;
        }

		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		// close target FIFO after writing the message

        char target_fifo[50];
        strcpy(target_fifo, req.target);
        int target_fd = open(target_fifo, O_WRONLY);
        if (target_fd == -1) {
            perror("open target");
            continue;
        }
        write(target_fd, &req, sizeof(struct message));
        close(target_fd);
	}
	close(server);
	close(dummyfd);
	return 0;
}
