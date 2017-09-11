/*
 * socket-common.h
 *
 * Simple TCP/IP communication using sockets
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 */

#ifndef _SOCKET_COMMON_H
#define _SOCKET_COMMON_H

/* Compile-time options */
#define TCP_PORT    35001
#define TCP_BACKLOG 5
#define PEER_LEN 2000000
#define WAIT_SEC 13

/* Encryption Constants */
#define DATA_SIZE       256
#define BLOCK_SIZE      16
#define KEY_SIZE	16  /* AES128 */
#define MESSAGE "Oresti ap'to volo."

#define MESSAGE_SIZE 300

#define HELLO_THERE "Hello there!"

#endif /* _SOCKET_COMMON_H */

