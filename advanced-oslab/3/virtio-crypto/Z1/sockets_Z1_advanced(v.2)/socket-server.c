/*
 * socket-server.c
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


struct User{
char name[100];
char address[100];
int sd_link;
int init;
};

void * safemalloc(size_t s){
void *cd;
cd = malloc(s);
if(cd == NULL){
	perror("memory");
	exit(1);	
}
return cd;	
}
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

int read_msg(char buf[MESSAGE_SIZE],int newsd){
ssize_t n;
char A[MESSAGE_SIZE];
memset(A,0,sizeof(A));
n = read(newsd, A, MESSAGE_SIZE*sizeof(char));
//printf("READ MSG ::::: A == %s\n",A);
strcpy(buf,A);
return n;
}
int send_msg(int f,int newsd,char A[MESSAGE_SIZE]){
/* if f equals 0 read from stdin */
char ans[MESSAGE_SIZE];
char c;
int i;
if(f == 0){
	printf("Reply ::\n");	
	memset(ans,0,sizeof(ans));
	i=0;
	while(fscanf(stdin,"%c",&c)!=0 && c!='\n' && i<=MESSAGE_SIZE-2){
			ans[i]=c;
			i++;
	}	
	ans[i] = '\0';
}
else{
	strcpy(ans,A);
}
fprintf(stdout, "Your Answer is:\n%s\n",ans);
if (insist_write(newsd,ans,strlen(ans)) != strlen(ans)) {
		perror("write to remote peer failed");
		return -1;
}
return 0;
}


int getname(struct User *peer){
	ssize_t n;
	int retval;
	struct timeval tv;
	char A[100];
	
	fd_set master;    // master file descriptor list
        fd_set read_fds;  // temp file descriptor list for select()	
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	FD_SET(peer->sd_link, &master);
	tv.tv_sec = WAIT_SEC;
        tv.tv_usec = 0;
	if (insist_write(peer->sd_link, strdup("Who are you?\n"), strlen(strdup("Who are you?\n"))) == -1) {
        	perror("send");
        }
	read_fds = master; // copy it
	fprintf(stdout,"Waiting for a selection... In ... getname()\n");
	retval = select(peer->sd_link+1, &read_fds, NULL, NULL, &tv);
	if(retval == -1){
 	        perror("select");
		return -1;
	}
	else{
		if(retval){
			n = read_msg(A,peer->sd_link);
			if (n <= 0) {
				if (n < 0){
					perror("read from remote peer failed");
				}
				else{
					fprintf(stderr, "Peer went away\n");
				}	
				return -1;	
			}
			sprintf(peer->name,"%s",A);
			return 1;
		}	
		else{
			printf("Peer with sd : %d on address %s didn't answer get_name question within %d seconds.\n",peer->sd_link,peer->address,WAIT_SEC);	
			return 0;		
		}		
	}
}

int main(void)
{
	fd_set master;    // master file descriptor list
        fd_set read_fds;  // temp file descriptor list for select()	
	int fdmax;        // maximum file descriptor number
	char buf[MESSAGE_SIZE];
	struct User * peer;
	char A[MESSAGE_SIZE+200];
	char addrstr[INET_ADDRSTRLEN];
	int sd, newsd;
	
	peer = (struct User *)safemalloc((PEER_LEN)*sizeof(struct User));	

	int i,j;
	ssize_t n;
	socklen_t len;
	struct sockaddr_in sa;
	
	/* Make sure a broken connection doesn't kill us */
	signal(SIGPIPE, SIG_IGN);
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);

	/* Create TCP/IP socket, used as main chat channel */
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	fprintf(stderr, "Created TCP socket\n");
  
	/* Bind to a well-known port */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(TCP_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("bind");
		exit(1);
	}
	fprintf(stderr, "Bound TCP socket to port %d\n", TCP_PORT);

	/* Listen for incoming connections */
	if (listen(sd, TCP_BACKLOG) < 0) {
		perror("listen");
		exit(1);
	}
	// add the listener to the master set
    	FD_SET(sd, &master);

	// keep track of the biggest file descriptor
   	fdmax = sd; // so far, it's this one
	/* Loop forever, accept()ing connections */
	for (;;) {
		read_fds = master; // copy it
		fprintf(stdout,"Waiting for a selection...\n");
	        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
	            perror("select");
	            exit(4);
	        }
		fprintf(stdout,"A selection has been made!\n");
		for(i = 0; i <= fdmax; i++){
			if (FD_ISSET(i, &read_fds)) { // we got one!!
                	if (i == sd) {
			    len = sizeof(struct sockaddr_in);
                	    // handle new connections
			    fprintf(stderr, "Waiting for an incoming connection...\n");
			    if ((newsd = accept(sd, (struct sockaddr *)&sa, &len)) < 0) {
				perror("accept");
				exit(1);
			    }
			    else{
                        	FD_SET(newsd, &master); // add to master set
                    		if (newsd > fdmax) {    // keep track of the max
                        		fdmax = newsd;
                    	    	}	
				peer[newsd].init=0;
				peer[newsd].sd_link=newsd;
			   	//Print everything is good;
			  	if(!inet_ntop(AF_INET, &sa.sin_addr, addrstr, sizeof(addrstr))) {
					perror("could not format IP address");
					exit(1);
			    	}
			    	fprintf(stderr, "Incoming connection from %s:%d\n",addrstr, ntohs(sa.sin_port));
				memset(&peer[i].address, 0, sizeof(peer[i].address));
				sprintf(peer[newsd].address,"%s:%d",addrstr,ntohs(sa.sin_port));
			    	/* Accept an incoming connection */
				sprintf(A,"Welcome on board!\n");				
				if (insist_write(newsd, A, strlen(A)) == -1) {
                         	        perror("send");
    	                        }		
				/* Ask his name */
				memset(&peer[i].name, 0, sizeof(peer[i].name));				
				peer[newsd].init = getname(&peer[newsd]);
				if(peer[i].init==-1){
					goto exit;
				}
				}				
			}else{
				if(peer[i].init==0){
					peer[i].init = getname(&peer[i]);
					if(peer[i].init==-1){
						goto exit;
					}
					continue;
				}
				n = read_msg(buf,i);
				if (n <= 0) {
					if (n < 0){
						perror("read from remote peer failed");
					}
					else{
						fprintf(stderr, "Peer went away\n");
					}				
exit:					if (close(i) < 0) perror("close");
					FD_CLR(i, &master);
					memset(&peer[i].name, 0, sizeof(peer[i].name));
					memset(&peer[i].address, 0, sizeof(peer[i].address));
					peer[i].init=-1;
				}else{			
				//toupper_buf(buf, n);
					sprintf(A,"User: %s with sd %d ::: with ip address : %s \nsays: %s\n",peer[i].name,peer[i].sd_link,peer[i].address,buf);
					for(j = 0; j <= fdmax; j++) {
                            		// send to everyone!
                            			if (FD_ISSET(j, &master)) {
                                		// except the listener and ourselves
                                			if (j != sd && j != i) {
                                			    	if (insist_write(j, A, strlen(A)) == -1){
                         				               perror("send");
    	                                			}
                                			}
                            			}
                        		}
				}
			  }	// END handle data from client
		  } // END got new incoming connection
	     } // END looping through file descriptors	
			/* Make sure we don't leak open files */
	}// END for(;;)--and you thought it would never end!

	/* This will never happen */
	return 1;
}

















	/*
	fprintf(stdout, "Client from %s:%d said : %s\n",addrstr, ntohs(sa.sin_port),buf);
				if( send_msg(0,newsd,NULL) == -1){
					break;			
				}
	*/	

