/*
 * socket-client.c
 * Simple TCP/IP communication using sockets
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 */

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "socket-common.h"

/* Insist until all of the data has been written */
ssize_t insist_write(int fd, const void *buf, size_t cnt)
{
	ssize_t ret;
	size_t orig_cnt = cnt;
	
	while (cnt > 0) {
	        ret = write(fd, buf, cnt);
	        if (ret < 0)
	                return ret;
	        buf += ret;
	        cnt -= ret;
	}

	return orig_cnt;
}

int main(int argc, char *argv[])
{

	fd_set master;    // master file descriptor list
        fd_set read_fds;  // temp file descriptor list for select()	
	int sd, port;
	int i=0;
	int fdmax;
	int count;
	char c;
	ssize_t n;
	char buf[MESSAGE_SIZE];
	char *hostname;
	struct hostent *hp;
	struct sockaddr_in sa;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
		exit(1);
	}
	hostname = argv[1];
	port = atoi(argv[2]); /* Needs better error checking */

	/* Create TCP/IP socket, used as main chat channel */
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	fprintf(stderr, "Created TCP socket\n");
	
	/* Look up remote hostname on DNS */
	if ( !(hp = gethostbyname(hostname))) {
		printf("DNS lookup failed for host %s\n", hostname);
		exit(1);
	}

	/* Connect to remote TCP port */
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	memcpy(&sa.sin_addr.s_addr, hp->h_addr, sizeof(struct in_addr));
	fprintf(stderr, "Connecting to remote host... "); fflush(stderr);
	if (connect(sd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("connect");
		exit(1);
	}
	fprintf(stderr, "Connected.\n");
	FD_SET(sd, &master);
	FD_SET(0, &master);	
	fdmax = sd;
	for(;;){	
		read_fds = master; // copy it
		fprintf(stdout,"Waiting for a selection...\n");
	        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
	            perror("select");
	            exit(4);
	        }
		fprintf(stdout,"A selection has been made!\n");
		if (FD_ISSET(sd, &read_fds)){
			/*
	 		* Let the remote know we're not going to write anything else.
	 		* Try removing the shutdown() call and see what happens.
	 		*/
		
			/* Read answer and write it to standard output */
			n = read(sd, buf, sizeof(buf));
			if (n < 0) {
				perror("read");
				exit(1);
			}
			if (n <= 0) continue;
			fprintf(stdout, "Server Replies:\n");		
			if (insist_write(1, buf, n) != n) {
				perror("write");
				exit(1);
			}
			fprintf(stdout,"\n");
		}
		if (FD_ISSET(0, &read_fds)){
			memset(buf,0,sizeof(buf));		
			i=0;
			fprintf(stdout,"Say Something to Server ::\n");
			/* Be careful with buffer overruns, ensure NUL-termination */
	 		//clearerr(stdin);
			while((count = fscanf(stdin,"%c",&c))!=-1 && c!='\n' && i<=MESSAGE_SIZE-2){
				//printf("ktr ::: %c\n",c);
				buf[i]=c;
				i++;
			}	
			if(count==-1){
				goto out;
			}	
			buf[i] = '\0';
			/* Say something... */		
			if (insist_write(sd, buf, strlen(buf)) != strlen(buf)) {
				perror("write");
				exit(1);
			}
		}
	}
out:	
	if (shutdown(sd, SHUT_WR) < 0) {
			perror("shutdown");
			exit(1);
	}
	fprintf(stdout, "\nDone.\n");
	return 0;
}
