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

/* Encrypt context */
struct session_op sess;
struct crypt_op cryp;
struct {
	unsigned char 	in[DATA_SIZE],
			encrypted[DATA_SIZE],
			decrypted[DATA_SIZE],
			iv[BLOCK_SIZE],
			key[KEY_SIZE];
} data;

struct sockaddr_in sa;
char addrstr[INET_ADDRSTRLEN];


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
	if (fill_urandom_buf(data.iv, BLOCK_SIZE) < 0) {
		perror("getting data from /dev/urandom\n");
		return 1;
	}
	fprintf(stdout, "Initialization vector created :\n");
	for (i = 0; i < BLOCK_SIZE; i++) {
		printf("%x", data.iv[i]);
	}
	printf("\n");

	/* Send initialization vector to client */		
	if (insist_write(new_sd,data.iv,sizeof(data.iv)) != sizeof(data.iv)) {
		perror("write to remote peer failed");
		return 2;
	}

	/* Create and print symmetric key */
	fprintf(stdout, "Creating symmetric key ...\n");	
	if (fill_urandom_buf(data.key, KEY_SIZE) < 0) {
		perror("getting data from /dev/urandom\n");
		return 1;
	}
	fprintf(stdout, "Symmetric key created :\n");	
	for (i = 0; i < KEY_SIZE; i++) {
		printf("%x", data.key[i]);
	}
	printf("\n");	

	/* Send symmetric key to client */	
	if (insist_write(new_sd,data.key,sizeof(data.key)) != sizeof(data.key)) {
		perror("write to remote peer failed");
		return 2;
	}

	return 0;
}

int encrypt_n_send_to_client(int crypto_fd, int new_sd){

	int i;	

	cryp.ses = sess.ses;
	cryp.len = sizeof(data.in);
	cryp.src = data.in;
	cryp.dst = data.encrypted;
	cryp.iv = data.iv;
	cryp.op = COP_ENCRYPT;
	
	fprintf(stdout, "Your PlainText Answer is:\n%s\n",data.in);
	
	/* Encrypt data */
	if (ioctl(crypto_fd, CIOCCRYPT, &cryp)) {
		perror("ioctl(CIOCCRYPT)");
		return 1;
	}
	
	fprintf(stdout,"Your CipherText Answer is:\n");
	for (i = 0; i < sizeof(data.encrypted); i++) {
		printf("%x", data.encrypted[i]);
	}
	printf("\n");

	/* Send data */
	if (insist_write(new_sd,data.encrypted,sizeof(data.encrypted)) != sizeof(data.encrypted)) {
		perror("write to remote peer failed");
		return 2;
	}

	return 0;
}


int decrypt_data(int crypto_fd){

	int i;

	cryp.ses = sess.ses;
	cryp.len = sizeof(data.encrypted);
	cryp.src = data.encrypted;
	cryp.dst = data.decrypted;
	cryp.iv = data.iv;
	cryp.op = COP_DECRYPT;

	printf("Encrypted Data from Client is:\n");
	for (i = 0; i < DATA_SIZE; i++) {
		printf("%x", data.encrypted[i]);
	}
	printf("\n");

	/* Decrypt data */
	if (ioctl(crypto_fd, CIOCCRYPT, &cryp)) {
		perror("ioctl(CIOCCRYPT)");
		return 1;
	}

	fflush(stdout);
	fprintf(stdout, "Man from %s:%d said : ",addrstr, ntohs(sa.sin_port));
	for (i = 0; i < sizeof(MESSAGE); i++) {
		printf("%c", data.decrypted[i]);
	}
	printf("\n");

	return 0;
}


int main(void) {

	char c;
	int sd, newsd;
	ssize_t n;
	int i=0;
	int count=0;
	int cfd;
	socklen_t len;
	
	/* Make sure a broken connection doesn't kill us */
	signal(SIGPIPE, SIG_IGN);

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
	

	/* Loop forever, accepting connections */
	for (;;) {
		fprintf(stderr, "Waiting for an incoming connection...\n");

		/* Accept an incoming connection */
		len = sizeof(struct sockaddr_in);
		if ((newsd = accept(sd, (struct sockaddr *)&sa, &len)) < 0) {
			perror("accept");
			exit(1);
		}
		if (!inet_ntop(AF_INET, &sa.sin_addr, addrstr, sizeof(addrstr))) {
			perror("could not format IP address");
			exit(1);
		}
		fprintf(stderr, "Incoming connection from %s:%d\n",addrstr, ntohs(sa.sin_port));

		/* Generate and send to client initialization vector and symmetric key*/		
		i=iv_n_key_server(newsd);
		
		if (i==1) return 1;			
		else if (i==2) break;		

		memset(&sess, 0, sizeof(sess));
		memset(&cryp, 0, sizeof(cryp));

		/* Initialise Crypto */
		sess.cipher = CRYPTO_AES_CBC;
		sess.keylen = KEY_SIZE;
		sess.key = data.key;
	
		if (ioctl(cfd, CIOCGSESSION, &sess)) {
			perror("ioctl(CIOCGSESSION)");
			return 1;
		}

		/* We break out of the loop when the remote peer goes away */
		for (;;) {			
			
			memset(data.encrypted, 0,sizeof(data.encrypted));
			n = read(newsd, data.encrypted, sizeof(data.encrypted));
			/* Client send his encrypted input */			
			
			if (n < 0) {
				perror("read from remote peer failed");
				break;
			}
			else if (n == 0) { 
				fprintf(stderr, "Peer went away\n");
				break;
			}
			
			/*
			 * Decrypt data.encrypted to data.decrypted
			 */
			memset(data.decrypted, 0,sizeof(data.decrypted));
			
			if(decrypt_data(cfd)) return 1;
		
			/* Answer to Client */
			i=0;
			memset(data.in,0,sizeof(data.in));

			fprintf(stdout, "Reply ::\n");
			while((count = fscanf(stdin,"%c",&c))!=-1 && c!='\n' && i<=DATA_SIZE-2) { 
				data.in[i]=c;
				i++;
			}
			data.in[i] = '\0';	
			if(count==-1){
				goto label;
				//Don't want to speak to him anymore shut him down
			}			

			/* Encrypt your answer */
			memset(data.encrypted,0,sizeof(data.encrypted));
			
			i=encrypt_n_send_to_client(cfd,newsd);

			if (i==1) return 1;			
			else if (i==2) break;
			
		}

		/* Make sure we don't leak open files */
label:		if (close(newsd) < 0){
			perror("close");
		}
		if (ioctl(cfd, CIOCFSESSION, &sess.ses)) {
			perror("ioctl(CIOCFSESSION)");
			return 1;
		}
	}
		
	/* This will never happen */
	return 1;
}

