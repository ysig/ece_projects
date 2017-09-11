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

#define MESSAGE_SIZE 300

#define HELLO_THERE "Hello there!"

#endif /* _SOCKET_COMMON_H */

