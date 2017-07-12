/*
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

#include <pspkernel.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pspnet_apctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <malloc.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_resolver.h>
#include <psphttp.h>

#include "pgeNet.h"

/*typedef struct _url
{
	char host[256];
	char page[256];
	char getString[256];
	char hostString[256];
} _url;*/

int pgeNetInit(void)
{	
	int result;
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	
	if(result < 0)
		return result;
		
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);
	
	if(result < 0)
		return result;
		
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);
	
	if(result < 0)
		return result;

	result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);
	
	if(result < 0)
		return result;
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);
	
	if(result < 0)
		return result;
			
	result = sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
	
	if(result < 0)
		return result;
			
	result = sceNetInetInit();
	
	if(result < 0)
		return result;
			
	result = sceNetApctlInit(0x10000, 48);
	
	if(result < 0)
		return result;
			
	result = sceNetResolverInit();
	
	if(result < 0)
		return result;

	return(1);
}

int pgeNetShutdown(void)
{
	int result;
	
	result = sceNetApctlTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetResolverTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetInetTerm();
	
	if(result < 0)
		return result;
		
	result = sceNetTerm();
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);
	
	if(result < 0)
		return result;
		
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);
	
	if(result < 0)
		return result;

	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
	
	if(result < 0)
		return result;
	
	result = sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
	
	if(result < 0)
		return result;
	
	return 1;
}

int pgeNetGetFile(const char *url, const char *filepath)
{
	int template, connection, request, ret, status, dataend, fd, byteswritten;
	SceULong64 contentsize;
	unsigned char readbuffer[8192];
	
	ret = sceHttpInit(20000);
	
	if(ret < 0)
		return 0;

	template = sceHttpCreateTemplate("PGE-agent/0.0.1 libhttp/1.0.0", 1, 1);
	
	if(template < 0)
		return 0;

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	
	if(ret < 0)
		return 0;

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	
	if(ret < 0)
		return 0;

	ret = sceHttpSetSendTimeOut(template, 60000000);
	
	if(ret < 0)
		return 0;

	connection = sceHttpCreateConnectionWithURL(template, url, 0);
	
	if(connection < 0)
		return 0;

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);
	
	if(request < 0)
		return 0;

	ret = sceHttpSendRequest(request, 0, 0);
	
	if(ret < 0)
		return 0;

	ret = sceHttpGetStatusCode(request, &status);
	
	if(ret < 0)
		return 0;
		
	if(status != 200)
		return 0;

	ret = sceHttpGetContentLength(request, &contentsize);
	
	if(ret < 0)
		return 0;

	dataend = 0;
	byteswritten = 0;
	
	fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);
	
	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);
		
		if(ret < 0)
		{
			sceIoWrite(fd, filepath, 4);
			sceIoClose(fd);
			return 0;
		}
		
		if(ret == 0)
			dataend = 1;
			
		if(ret > 0)
		{
			byteswritten += ret;
			sceIoWrite(fd, readbuffer, ret);
			//printf("Got and wrote %d bytes...\n", ret);
		}
	}
	
	sceIoClose(fd);
	
	//printf("File saved (size = %d bytes)... Exiting!\n", bytes_written);
	
	sceHttpDeleteRequest(request);
	
	sceHttpDeleteConnection(connection);
	
	sceHttpDeleteTemplate(template);
	
	sceHttpEnd();
	
	return 1;
}

int pgeNetPostForm(const char *url, char *data, char *response, unsigned int responsesize)
{
	int template, connection, request, ret, status;
	
	ret = sceHttpInit(20000);
	
	if(ret < 0)
		return 0;

	template = sceHttpCreateTemplate("PGE-agent/0.0.1 libhttp/1.0.0", 1, 1);
	
	if(template < 0)
	{	
		sceHttpEnd();
		return 0;
	}

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	
	if(ret < 0)
	{	
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	
	if(ret < 0)
	{	
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}

	ret = sceHttpSetSendTimeOut(template, 60000000);
	
	if(ret < 0)
	{	
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}

	connection = sceHttpCreateConnectionWithURL(template, url, 0);
	
	if(connection < 0)
	{
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}
	
	sceHttpEnableKeepAlive(connection);

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_POST, (char*)url, strlen(data));
	
	if(request < 0)
	{
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}
		
	sceHttpAddExtraHeader(request, "Content-Type", "application/x-www-form-urlencoded", 0);

	ret = sceHttpSendRequest(request, data, strlen(data));
	
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}
	
	ret = sceHttpGetStatusCode(request, &status);
	
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}
	
	if(status != 200)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);	
		sceHttpEnd();
		return 0;
	}

	if(response != NULL && responsesize > 0)
	{
		ret = sceHttpReadData(request, response, responsesize);
	
		if(ret < 0)
		{
			sceHttpDeleteRequest(request);
			sceHttpDeleteConnection(connection);
			sceHttpDeleteTemplate(template);	
			sceHttpEnd();
			return 0;
		}
	}
		
	sceHttpDeleteRequest(request);
	
	sceHttpDeleteConnection(connection);
	
	sceHttpDeleteTemplate(template);
	
	sceHttpEnd();
	
	return 1;
}

int pgeNetSwitchStatus(void)
{
	return sceWlanGetSwitchState();
}

void pgeNetDisconnect(void)
{
	sceNetApctlDisconnect();
	pspSdkInetTerm();
}

int pgeNetIsConnected(void)
{
	char ipaddr[32];

	if(sceNetApctlGetInfo(8, ipaddr) != 0)
		return(0);
           
	return(1);
}

int pgeNetGetLocalIp(char *buffer)
{
	if(sceNetApctlGetInfo(8, buffer) != 0)
		return(0);
		
	return(1);
}

int pgeNetResolveHost(char *hostname, char *resolved)
{
	struct hostent *host;

	if(!(host = gethostbyname(hostname)))
		return 0;
	
	sprintf(resolved, inet_ntoa(*((struct in_addr *)host->h_addr)));

	return 1;
}

pgeSocket pgeNetSocketCreate(void)
{
	pgeSocket sock;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(sock < 0)
		return -1;
	
	return sock;
}

pgeSocket pgeNetSocketAccept(pgeSocket socket)
{
	pgeSocket sock;
	
	struct sockaddr_in client;

	socklen_t size;

	sock = accept(socket, (struct sockaddr *) &client, &size);
	
	if(sock < 0)
		return -1;
	
	return sock;
}

int pgeNetSocketBind(pgeSocket socket, unsigned short port)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int error;

	error = bind(socket, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	if(error < 0)
		return 0;
	
	return 1;
}

int pgeNetSocketListen(pgeSocket socket, unsigned int maxconn)
{
	int error;
	
	error = listen(socket, maxconn);
	
	if(error < 0)
		return 0;
	
	return 1;
}

int pgeNetSocketConnect(pgeSocket socket, char *ip, unsigned short port)
{	
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_aton(ip, &(addr.sin_addr));
	memset(&(addr.sin_zero), '\0', 8);

	if(connect(socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return 0;
	
	return 1;
}

int pgeNetSocketSend(pgeSocket socket, const void *data, int length)
{
	int bytessent = send(socket, data, length, 0);
	
	return bytessent;
}

int pgeNetSocketReceive(pgeSocket socket, void *data, int length)
{
	int bytesrecv = recv(socket, data, length, 0);

	return bytesrecv;
}

void pgeNetSocketClose(pgeSocket socket)
{
	close(socket);
}

void pgeNetSocketSetClear(pgeSocketSet *set)
{
	FD_ZERO(set);
}

void pgeNetSocketSetAdd(pgeSocket socket, pgeSocketSet *set)
{
	FD_SET(socket, set);
}

void pgeNetSocketSetRemove(pgeSocket socket, pgeSocketSet *set)
{
	FD_CLR(socket, set);
}

int pgeNetSocketSetIsMember(pgeSocket socket, pgeSocketSet *set)
{
	if(FD_ISSET(socket, set))
		return 1;
	
	return 0 ;
}

int pgeNetSocketSetSelect(unsigned int maxsockets, pgeSocketSet *set)
{
	// TODO: Add argument manipulation of timeout etc.
	
	int numsockets;

	struct timeval tv;
	
	tv.tv_sec = 0;
	tv.tv_usec = 1000;

	numsockets = select(maxsockets, set, NULL, NULL, &tv);

	if(numsockets < 0)
		return -1;
	
	return numsockets;
}

/*static _url _parseUrl(char *theUrl)
{
	// Bit messy....
	_url parsedUrl;
	char *host;
	char *page;
	char *check;
	char urlBuffer[256];

	memset(&parsedUrl, 0, sizeof(parsedUrl));
	
	// Check for http://
	check = strstr(theUrl, "http://");

	if(check)
	{
		check += 7;
		strcpy(theUrl, check);
	}
	
	strcpy(urlBuffer, theUrl);

	// Get first '/'
	host = strtok(urlBuffer, "/");
	strcpy(parsedUrl.host, host);

	page = strstr(theUrl, "/");
	strcpy(parsedUrl.page, page);

	// For debugging
	//pgeLogPrint("theUrl: %s\r\n", theUrl);
	//pgeLogPrint("Host: %s\r\n", parsedUrl.host);
	//pgeLogPrint("Page: %s\r\n", parsedUrl.page);

	return(parsedUrl);
}

int pgeNetGetUrl(char *url, char *response)
{
	char buffer[1024];
	
	_url parsedUrl;
	
	parsedUrl = _parseUrl(url);
	
	sprintf(parsedUrl.getString, "GET %s HTTP/1.0\r\n", parsedUrl.page);
	sprintf(parsedUrl.hostString, "host: %s\r\n\r\n", parsedUrl.host);
        
	pgeSocket mySocket = pgeNetSocketCreate();

	char ip[12];
	
	if(pgeNetResolveHost(parsedUrl.host, ip) == 0)
		return(0);

	pgeNetSocketConnect(mySocket, ip, 80);

	pgeNetSocketSend(mySocket, parsedUrl.getString, strlen(parsedUrl.getString));
	pgeNetSocketSend(mySocket, parsedUrl.hostString, strlen(parsedUrl.hostString));

	int bytesReceived;
        
	char header[1024];

	memset(header, 0, sizeof(header));
	memset(buffer, 0, sizeof(buffer));

	int headerFinished = 0;

	char *headerEnd;

	while(pgeRunning())
	{
		bytesReceived = pgeNetSocketReceive(mySocket, buffer);

		if(strlen(buffer) > 0)
		{
			if(headerFinished)
			{
				strcpy(response, headerEnd);
				break;
			}
			else
			{
				strcat(header, buffer);
				headerEnd = strstr(header, "\r\n\r\n");
				
				if(headerEnd)
				{
					headerEnd += 4;
					headerFinished = 1;
					strcpy(response, headerEnd);
					break;
				}
			}
		}
	}
	
	return 1;
}


int pgeNetPhpPost()
{
	//
	return(1);
}*/
