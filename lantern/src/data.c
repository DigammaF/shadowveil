
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include "data.h"

/**
 * 
 * 	\fn			int sendData(socket_t* socket, const char* data);
 * 	\brief		Envoie des données à .remote
 *  \param		socket: la socket à utiliser
 *  \param		data: chaîne de charactères terminée par un zéro
 * 	\result		nombre d'octets envoyés et indicateur de réussite POSIX
 * 
 */
int sendData(socket_t* socket, const char* data) {
	if (socket->mode == SOCK_STREAM) {
		ssize_t bytesSent = send(socket->fileDescriptor, data, strlen(data) + 1, 0);
		if (bytesSent < 0) { return -1; }
		return (int)bytesSent;
	} else if (socket->mode == SOCK_DGRAM) {
		ssize_t bytesSent = sendto(socket->fileDescriptor, data, strlen(data) + 1, 0,
			(struct sockaddr*)&(socket->remote), sizeof(socket->remote)
		);
		if (bytesSent < 0) { return -1; }
		return (int)bytesSent;
	} else {
		errno = EPFNOSUPPORT;
		return -1;
	}
}

/**
 * 
 * 	\fn			int recvData(socket_t* socket, char* data);
 * 	\brief		Reçoit des données depuis .remote; en mode SOCK_DGRAM écrit .remote
 *  \param		socket: la socket à utiliser
 *  \param		data: chaîne de charactères terminée par un zéro
 *  \param		maxDataLength: la longueur maximale des données reçues
 * 	\result		nombre d'octets recus et indicateur de réussite POSIX
 * 
 */
int recvData(socket_t* socket, char* data, size_t maxDataLength) {
	if (socket->mode == SOCK_STREAM) {
		ssize_t bytesReceived = recv(socket->fileDescriptor, data, maxDataLength - 1, 0);
		if (bytesReceived < 0) { return -1; }
		data[bytesReceived] = '\0';
		return (int)bytesReceived;
	} else if (socket->mode == SOCK_DGRAM) {
		struct sockaddr_in client_address;
		socklen_t client_address_length = sizeof(client_address);
		ssize_t bytesReceived = recvfrom(
			socket->fileDescriptor, data, maxDataLength - 1, 0,
			(struct sockaddr*)&client_address, &client_address_length
		);
		if (bytesReceived < 0) { return -1; }
		socket->remote = client_address;
		data[bytesReceived] = '\0';
		return (int)bytesReceived;
	} else {
		errno = EPFNOSUPPORT;
		return -1;
	}
	
}
