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
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <crypto/cryptodev.h>


#include "socket-common.h"

struct cry{
	unsigned char 	in[DATA_SIZE],
			encrypted[DATA_SIZE],
			decrypted[DATA_SIZE],
			iv[BLOCK_SIZE],
			key[KEY_SIZE];
};
struct User{
	char name[100];
	char address[100];
	int sd_link;
	int init;
	struct cry data;
	struct session_op sess;
	struct crypt_op cryp;
};
struct User * peer;

void * safemalloc(size_t s){
	void *cd;
	cd = malloc(s);
	if(cd == NULL){
		perror("memory");
		exit(1);	
	}
	return cd;	
}
ssize_t insist_read(int fd, void *buf, size_t cnt)
{
        ssize_t ret;
        size_t orig_cnt = cnt;

        while (cnt > 0) {
                ret = read(fd, buf, cnt);
                if (ret < 0)
                        return ret;
                buf += ret;
                cnt -= ret;
        }

        return orig_cnt;
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

static int fill_urandom_buf(unsigned char *buf, size_t cnt)
{
        int crypto_fd;
        int ret = -1;

        crypto_fd = open("/dev/urandom", O_RDONLY);
        if (crypto_fd < 0)
                return crypto_fd;

        ret = insist_read(crypto_fd, buf, cnt);
        close(crypto_fd);

        return ret;
}

int iv_n_key_server(int new_sd) {
	int i;
	/* Create and print initialization vector */
	fprintf(stdout, "Creating initialization vector ...\n");
	if (fill_urandom_buf(peer[new_sd].data.iv, BLOCK_SIZE) < 0) {
		perror("getting data from /dev/urandom\n");
		return 1;
	}
	fprintf(stdout, "Initialization vector created :\n");
	for (i = 0; i < BLOCK_SIZE; i++) {
		printf("%x", peer[new_sd].data.iv[i]);
	}
	printf("\n");

	/* Send initialization vector to client */		
	if (insist_write(new_sd,peer[new_sd].data.iv,sizeof(peer[new_sd].data.iv)) != sizeof(peer[new_sd].data.iv)) {
		perror("write to remote peer failed");
		return 2;
	}

	/* Create and print symmetric key */
	fprintf(stdout, "Creating symmetric key ...\n");	
	if (fill_urandom_buf(peer[new_sd].data.key, KEY_SIZE) < 0) {
		perror("getting data from /dev/urandom\n");
		return 1;
	}
	fprintf(stdout, "Symmetric key created :\n");	
	for (i = 0; i < KEY_SIZE; i++) {
		printf("%x", peer[new_sd].data.key[i]);
	}
	printf("\n");	

	/* Send symmetric key to client */	
	if (insist_write(new_sd,peer[new_sd].data.key,sizeof(peer[new_sd].data.key)) != sizeof(peer[new_sd].data.key)) {
		perror("write to remote peer failed");
		return 2;
	}

	return 0;
}
int encrypt_n_send_to_client(int crypto_fd, int new_sd){

	int i;	
	peer[new_sd].cryp.ses = peer[new_sd].sess.ses;
	peer[new_sd].cryp.len = sizeof(peer[new_sd].data.in);
	peer[new_sd].cryp.src = peer[new_sd].data.in;
	peer[new_sd].cryp.dst = peer[new_sd].data.encrypted;
	peer[new_sd].cryp.iv = peer[new_sd].data.iv;
	peer[new_sd].cryp.op = COP_ENCRYPT;
	
	/*
	fprintf(stdout, "Your PlainText Answer is:\n%s\n",peer[new_sd].data.in);
	*/
	/* Encrypt data */
	if (ioctl(crypto_fd, CIOCCRYPT, &peer[new_sd].cryp)) {
		perror("ioctl(CIOCCRYPT)");
		return 1;
	}
	/*
	fprintf(stdout,"Your CipherText Answer is:\n");
	for (i = 0; i < sizeof(peer[new_sd].data.encrypted); i++) {
		printf("%x", peer[new_sd].data.encrypted[i]);
	}
	printf("\n");
	*/
	/* Send data */
	if (insist_write(new_sd,peer[new_sd].data.encrypted,sizeof(peer[new_sd].data.encrypted)) != sizeof(peer[new_sd].data.encrypted)) {
		perror("write to remote peer failed");
		return 2;
	}

	return 0;
}
int decrypt_data(int sd, int crypto_fd){
	int i;
	peer[sd].cryp.ses = peer[sd].sess.ses;
	peer[sd].cryp.len = sizeof(peer[sd].data.in);
	peer[sd].cryp.src = peer[sd].data.in;			
	peer[sd].cryp.src = peer[sd].data.encrypted;
	peer[sd].cryp.dst = peer[sd].data.decrypted;
	peer[sd].cryp.iv = peer[sd].data.iv;	
	peer[sd].cryp.op = COP_DECRYPT;

	/*
	printf("Encrypted Data from Client is:\n");
	for (i = 0; i < DATA_SIZE; i++) {
		printf("%x", peer[sd].data.encrypted[i]);
	}
	printf("\n");
	*/
	/* Decrypt data */
	if (ioctl(crypto_fd, CIOCCRYPT, &peer[sd].cryp)) {
		perror("ioctl(CIOCCRYPT)");
		return 1;
	}
	return 0;
}
int getname(int sd, int crypto_fd){
	ssize_t n;
	int retval;
	struct timeval tv;
	
	fd_set master;    // master file descriptor list
        fd_set read_fds;  // temp file descriptor list for select()	
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);
	FD_SET(sd, &master);
	tv.tv_sec = WAIT_SEC;
        tv.tv_usec = 0;
	sprintf(peer[sd].data.in,"Who are you?\n");
	encrypt_n_send_to_client(crypto_fd,sd);
	read_fds = master; // copy it
	fprintf(stdout,"Waiting for a selection... In ... getname()\n");
	retval = select(sd+1, &read_fds, NULL, NULL, &tv);
	if(retval == -1){
 	        perror("select");
		return -1;
	}
	else{
		if(retval){
			n = read(sd,peer[sd].data.encrypted, sizeof(peer[sd].data.encrypted));
			if (n <= 0) {
				if (n < 0){
					perror("read from remote peer failed");
				}
				else{
					fprintf(stderr, "Peer went away\n");
				}	
				return -1;	
			}
			fprintf(stdout,"A selection has been made..\n");
			decrypt_data(sd,crypto_fd);
			sprintf(peer[sd].name,"%s",peer[sd].data.decrypted);	
			return 1;
		}	
		else{
			printf("Peer with sd : %d on address %s didn't answer get_name question within %d seconds.\n",peer[sd].sd_link,peer[sd].address,WAIT_SEC);	
			return 0;		
		}		
	}
}
int main(void)
{
	fd_set master;    // master file descriptor list
        fd_set read_fds;  // temp file descriptor list for select()	
	int fdmax;        // maximum file descriptor number
	//char A[MESSAGE_SIZE+200];
	char addrstr[INET_ADDRSTRLEN];
	int sd, newsd;
	int cfd;
	peer = (struct User *)safemalloc((PEER_LEN)*sizeof(struct User));	

	int i,j;
	int x;
	ssize_t n;
	socklen_t len;
	struct sockaddr_in sa;
	
	/* Make sure a broken connection doesn't kill us */
	signal(SIGPIPE, SIG_IGN);
	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);

	/* Open dev crypto */
	cfd = open("/dev/crypto", O_RDWR);
	if (cfd < 0) {
		perror("open(/dev/crypto)");
		return 1;
	}


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
			//printf("\n\nIf case 0! i = %d \n\n",i);
			if (FD_ISSET(i, &read_fds)) { // we got one!!
                		if (i == sd){
				   //printf("\n\nIf case 1! \n\n");
				    len = sizeof(struct sockaddr_in);
                		    // handle new connections
				    fprintf(stderr, "Waiting for an incoming connection...\n");
				    if ((newsd = accept(sd, (struct sockaddr *)&sa, &len)) < 0) {
					perror("accept");
					exit(1);
				    }
				    else{
					printf("\n\nIf case 2! \n\n");
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
				    	fprintf(stdout, "Incoming connection from %s:%d\n",addrstr, ntohs(sa.sin_port));
					sprintf(peer[newsd].address,"%s:%d",addrstr,ntohs(sa.sin_port));
					x=iv_n_key_server(newsd);		
					if (x==1) return 1;			
					else if (x==2) break;			
					/* Initialise Crypto */
					peer[newsd].sess.cipher = CRYPTO_AES_CBC;
					peer[newsd].sess.keylen = KEY_SIZE;
					peer[newsd].sess.key = peer[newsd].data.key;
		
					if (ioctl(cfd, CIOCGSESSION, &peer[newsd].sess)){
						perror("ioctl(CIOCGSESSION)");
						return 1;
					}
				    	/* Accept an incoming connection */
					sprintf(peer[newsd].data.in,"Welcome on board!\n");				
					x=encrypt_n_send_to_client(cfd,newsd);
					/* Ask his name */
					if (x==1) return 1;	
					else if (x==2) break;
					peer[newsd].init = getname(newsd,cfd);
						if(peer[newsd].init==-1){
							goto exit;
						}
					}				
				}else{
					//printf("\n\nIf case 3! \n\n");
					if(peer[i].init==0){
						peer[i].init = getname(i,cfd);
						if(peer[i].init==-1){
							//printf("\n\nIf case 3 exit! \n\n");		
							goto exit;
						}
						continue;
					}
					//printf("\n\nIf case 3 .. 011! \n\n");
					n = read(i, peer[i].data.encrypted, sizeof(peer[i].data.encrypted));
					//printf("\n\nIf case 3 .. 1! \n\n");
					if (n <= 0) {
						if (n < 0){
						perror("read from remote peer failed");
						}
						else{
							fprintf(stderr, "Peer went away\n");
							//printf("\n\nIf case 3 .. 2! \n\n");
						}				
exit:						if (close(i) < 0) perror("close");
						FD_CLR(i, &master);
						if (ioctl(cfd, CIOCFSESSION, &peer[i].sess.ses)) {
							perror("ioctl(CIOCFSESSION)");
							return 1;
						}
						peer[i].init=-1;
					}else{
						//printf("\n\nIf case 4! \n\n");
						if(decrypt_data(i,cfd)) return 1;
						sprintf(peer[i].data.in,"User: %s ::: with sd %d ::: with ip address : %s \nsays: %s\n",peer[i].name,peer[i].sd_link,peer[i].address,peer[i].data.decrypted);
						for(j = 0; j <= fdmax; j++) {
                            			// send to everyone!
                            				if (FD_ISSET(j, &master)){
                                			// except the listener and ourselves
                                				if (j != sd && j != i) {
									//printf("\n\nIf case 5! \n\n");									
									sprintf(peer[j].data.in,"%s",peer[i].data.in);
									printf("Peer with sd = %d sending to peer with sd = %d, from cfd = %d the text %s",i,j,cfd,peer[i].data.in);				
					                		x=encrypt_n_send_to_client(cfd,j);
									if (x==1) return 1;			
									else if (x==2) break;
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

