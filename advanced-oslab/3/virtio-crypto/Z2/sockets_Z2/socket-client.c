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
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
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


int iv_n_key(int server_descriptor) {
	
	int i;
	ssize_t n;	

	fprintf(stdout, "Receiving initialization vector ...\n");

	/* Read initialization vector */
	n = read(server_descriptor, data.iv, sizeof(data.iv));
	if (n < 0) {
		perror("read");
		exit(1);
	}
	if (n <= 0) return 1;

	/* Print initialization vector */
	fprintf(stdout, "Initialization vector received :\n");

	for (i = 0; i < BLOCK_SIZE; i++) {
		printf("%x", data.iv[i]);
	}
	printf("\n");	
	
	fprintf(stdout, "Receiving symmetric key ...\n");

	/* Read key */
	n = read(server_descriptor, data.key, sizeof(data.key));
	if (n < 0) {
		perror("read");
		exit(1);
	}
	if (n <= 0) return 1;

	/* Print key */
	fprintf(stdout, "Symmetric key received :\n");	
	for (i = 0; i < KEY_SIZE; i++) {
		printf("%x", data.key[i]);
	}
	printf("\n");	
	
	fprintf(stdout, "All done.\n");	

	return 0;

}


int encrypt_n_send(int crypto_fd, int server_descriptor) {
	
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
	if (insist_write(server_descriptor, data.encrypted, sizeof(data.encrypted)) != sizeof(data.encrypted)) {
		perror("write");
		exit(1);
	}

	return 0;
}


int decrypt_data(int crypto_fd) {
	
	int i;

	cryp.ses = sess.ses;			
	cryp.src = data.encrypted;
	cryp.dst = data.decrypted;
	cryp.op = COP_DECRYPT;

	printf("Encrypted Data from Server is:\n");
	for (i = 0; i < DATA_SIZE; i++) {
		printf("%x", data.encrypted[i]);
	}
	printf("\n");

	/* Decrypt data */
	if (ioctl(crypto_fd, CIOCCRYPT, &cryp)) {
		perror("ioctl(CIOCCRYPT)");
		return 1;
	}

	fprintf(stdout, "Remote says:\n");
	fprintf(stdout, "%s",data.decrypted);		

	return 0;
}

int main(int argc, char *argv[]) {

	int sd, port;
	int i=0;
	int cfd;
	int count=0;
	char c;
	ssize_t n;
	char *hostname;
	struct hostent *hp;
	struct sockaddr_in sa;

	/* Open dev crypto */
	cfd = open("/dev/crypto", O_RDWR);
	if (cfd < 0) {
		perror("open(/dev/crypto)");
		return 1;
	}
	
	memset(&sess, 0, sizeof(sess));
	memset(&cryp, 0, sizeof(cryp));

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

	/* Initialization vector and symmetric key */	
	if(iv_n_key(sd)) goto out;
	
	sess.cipher = CRYPTO_AES_CBC;
	sess.keylen = KEY_SIZE;
	sess.key = data.key;

	if (ioctl(cfd, CIOCGSESSION, &sess)) {
		perror("ioctl(CIOCGSESSION)");
		return 1;
	}
	
	while(1) {
		
		i=0;
		memset(data.in,0,sizeof(data.in));		
		fprintf(stdout,"Say Something to Server ::\n");
		/* Be careful with buffer overruns, ensure NUL-termination */
	 	
		/* Say something... */
		while((count = fscanf(stdin,"%c",&c))!=-1 && c!='\n' && i<=DATA_SIZE-2) { 
			data.in[i]=c;
			i++;
		}	
		if(count==-1) goto out;	

		data.in[i] = '\0';

		/* Encrypt what you've said */
		
		if(encrypt_n_send(cfd,sd)) return 1;
		
		/*
	 	* Let the remote know we're not going to write anything else.
	 	* Try removing the shutdown() call and see what happens.
	 	*/
	
		/* Read answer and write it to standard output */
		for (;;) {

			n = read(sd, data.encrypted, sizeof(data.encrypted));
			if (n < 0) {
				perror("read");
				exit(1);
			}
			if (n <= 0){
				goto out;			
			}
			
			/*
			 * Decrypt data.encrypted to data.decrypted
			 */
			memset(data.decrypted,0,sizeof(data.decrypted));		
			
			if(decrypt_data(cfd)) return 1;		

			/*
			if (insist_write(1,data.decrypted, sizeof(data.decrypted)) != sizeof(data.decrypted)) {
				perror("write");
				exit(1);
			}
			*/
			break;
		}
		fprintf(stdout,"\n");

	}

out:	if (shutdown(sd, SHUT_WR) < 0) {
		perror("shutdown");
		exit(1);
	}

	if (ioctl(cfd, CIOCFSESSION, &sess.ses)) {
		perror("ioctl(CIOCFSESSION)");
		return 1;
	}
	fprintf(stdout, "\nDone.\n");

	return 0;

}
