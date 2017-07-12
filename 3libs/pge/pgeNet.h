/*
 * pgeNet.h: Header for net/wifi
 *
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 * Phoenix Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Phoenix Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Phoenix Game Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PGENET_H__
#define __PGENET_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeNet Net/Wifi Library
 *  @{
 */
 
#include <psptypes.h>
#include <psputility.h>
#include <netinet/in.h>

#ifdef __PSP__
	#include <sys/fd_set.h>
#endif

#define PGE_SOCKET_TCP SOCK_STREAM	/**< Stream socket */
#define PGE_SOCKET_UDP SOCK_DGRAM	/**< Datagram socket */

#define PGE_LOCAL_IP 0		/**< Local IP ie. 192.168.x.x */
#define PGE_REAL_IP  1		/**< 'Real' IP */

#define PGE_MAX_CLIENTS 256	/**< Maximum clients that can connect to a server */

typedef int pgeSocket;

typedef fd_set pgeSocketSet;

/**
 * Initialise the wifi
 *
 * @returns 1 on success else an error code.
 */
int pgeNetInit(void);

/**
 * Shutdown the wifi
 *
 * @returns 1 on success else an error code.
 */
int pgeNetShutdown(void);

/**
 * Download a file
 *
 * @param url - The URL of the file
 *
 * @param filepath - The path to save the downloaded file
 *
 * @returns 1 on success else an error code.
 */
int pgeNetGetFile(const char *url, const char *filepath);

/**
 * Post a form
 *
 * @param url - The URL to post to
 *
 * @param data - The data to post
 *
 * @param response - The buffer where the response from the server will be stored
 *
 * @param responsesize - Size of the response buffer
 *
 * @returns 1 on success else an error code.
 */
int pgeNetPostForm(const char *url, char *data, char *response, unsigned int responsesize);

/**
 * Get the status of the wlan switch
 *
 * @returns 1 if switch is up
 */
int pgeNetSwitchStatus(void);

/**
 * Disconnect from an access point
 */
void pgeNetDisconnect(void);

/**
 * Check if connected to an access point
 *
 * @returns 1 on valid connection
 */
int pgeNetIsConnected(void);

/**
 * Get Local IP address
 *
 * @param buffer - Pointer to a buffer which will store the IP address
 *
 * @returns 1 on success else an error code.
 */
int pgeNetGetLocalIp(char *buffer);

/**
 * Resolve a hostname
 *
 * @param hostname - The hostname to resolve
 *
 * @param resolved - Pointer to a buffer that will store the resolved hostname
 *
 * @returns 1 on success else an error code.
 */
int pgeNetResolveHost(char *hostname, char *resolved);

/**
 * Create a socket
 *
 * @returns A ::pgeSocket.
 */
pgeSocket pgeNetSocketCreate(void);

/**
 * Accept a connection
 *
 * @param socket - The socket.
 *
 * @returns A ::pgeSocket for the incoming connection.
 */
pgeSocket pgeNetSocketAccept(pgeSocket socket);

/**
 * Bind a socket to a port
 *
 * @param socket - The socket.
 *
 * @param port - The port to bind to.
 *
 * @returns 1 on success, 0 on error.
 */
int pgeNetSocketBind(pgeSocket socket, unsigned short port);

/**
 * Listen for connections
 *
 * @param socket - The socket.
 *
 * @param maxconn - The maximum amount of connections to listen for.
 *
 * @returns 1 on success, 0 on error.
 */
int pgeNetSocketListen(pgeSocket socket, unsigned int maxconn);

/**
 * Connect to an IP and port
 *
 * @param socket - The socket.
 *
 * @param ip - The IP to connect to.
 *
 * @param port - The port to connect to.
 *
 * @returns 1 on success, 0 on error.
 */
int pgeNetSocketConnect(pgeSocket socket, char *ip, unsigned short port);

/**
 * Send data
 *
 * @param socket - The socket.
 *
 * @param data - The data to send.
 *
 * @param length - The size of the data (in bytes).
 *
 * @returns Number of bytes sent, -1 on error.
 */
int pgeNetSocketSend(pgeSocket socket, const void *data, int length);

/**
 * Receive data
 *
 * @param socket - The socket.
 *
 * @param data - Pointer to a buffer where the received data will be stored.
 *
 * @param length - The size of the buffer (in bytes).
 *
 * @returns Number of bytes received, -1 on error.
 */
int pgeNetSocketReceive(pgeSocket socket, void *data, int length);

/**
 * Close a socket
 *
 * @param socket - The socket.
 */
void pgeNetSocketClose(pgeSocket socket);

/**
 * Clear a socket set
 *
 * @param set - The socket set to clear.
 */
void pgeNetSocketSetClear(pgeSocketSet *set);

/**
 * Add a socket to a socket set
 *
 * @param socket - The socket to add.
 *
 * @param set - The socket set to add the socket to.
 */
void pgeNetSocketSetAdd(pgeSocket socket, pgeSocketSet *set);

/**
 * Remove a socket from a socket set
 *
 * @param socket - The socket to remove.
 *
 * @param set - The socket set to remove the socket from.
 */
void pgeNetSocketSetRemove(pgeSocket socket, pgeSocketSet *set);

/**
 * Check if a socket is a member of a socket set
 *
 * @param socket - The socket to check.
 *
 * @param set - The socket set to check the socket is a member of.
 *
 * @returns 1 if the socket is a member of the set, otherwise 0.
 */
int pgeNetSocketSetIsMember(pgeSocket socket, pgeSocketSet *set);

/**
 * Check if a socket is ready for reading
 *
 * @param maxsockets - Maximum number of sockets to check.
 *
 * @param set - The socket set to check.
 *
 * @returns Number of sockets, -1 on error.
 */
int pgeNetSocketSetSelect(unsigned int maxsockets, pgeSocketSet *set);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGENET_H__
