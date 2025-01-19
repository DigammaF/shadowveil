
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

#include "session.h"

/**
 *
 * 	\fn			int createSocket(const int mode, socket_t* socketObject);
 * 	\brief		Créée une socket en mode SOCK_STREAM ou SOCK_DGRAM, remplissant les champs .fileDescriptor et .mode
 *  \param		mode: SOCK_STREAM ou SOCK_DGRAM
 *  \param		socketObject: une socket vierge
 * 	\result		indicateur de réussite POSIX
 * 
 */
int createSocket(const int mode, socket_t* socketObject) {
	if (mode != SOCK_STREAM && mode != SOCK_DGRAM) {
		errno = EPFNOSUPPORT;
		return -1;
	}
	socketObject->fileDescriptor = socket(AF_INET, mode, 0);
	if (socketObject->fileDescriptor == -1) { return -1; }
	socketObject->mode = mode;
	return 0;
}

/**
 * 
 * 	\fn 		int closeSocket(socket_t* socket);
 * 	\brief		Ferme une socket
 * 	\param		socket: socket en mode STREAM
 * 	\result		indicateur de réussite POSIX
 * 
 */
int closeSocket(socket_t* socket) {
	return close(socket->fileDescriptor);
}

/**
 * 
 * 	\fn			int bindLocal(socket_t* socket, const char* address, const short port);
 * 	\brief		Associe la socket à une addresse, remplissant le champ .local
 *  \param		socket: socket en mode STREAM
 * 	\param		address: adresse IP
 * 	\param		port: port
 * 	\result		indicateur de réussite POSIX
 * 
 */
int bindLocal(socket_t* socket, const char* address, const short port) {
	struct sockaddr_in addressObject;
	memset(&addressObject, 0, sizeof(addressObject));
	addressObject.sin_family = AF_INET;
	addressObject.sin_port = htons(port);
	if (1 != inet_pton(AF_INET, address, &addressObject.sin_addr)) { return -1; }
	int status = bind(
		socket->fileDescriptor,
		(struct sockaddr*)&addressObject, sizeof(addressObject)
	);
	if (status == -1) { return -1; }
	socket->local = addressObject;
	return 0;
}

/**
 * 
 * 	\fn			int listenSocket(socket_t* socket, const int maxClientCount);
 * 	\brief		Attends que des clients soient prêt à être acceptés
 *  \param		socket: socket en mode STREAM
 * 	\param		maxClientCount: nombre maximal de client dans la file d'attente
 * 	\result		indicateur de réussite POSIX
 * 
 */
int listenSocket(socket_t* socket, const int maxClientCount) {
	return listen(socket->fileDescriptor, maxClientCount);
}

/**
 * 
 * 	\fn			int acceptRemote(socket_t* socket);
 * 	\brief		Accepte un client. Les informations relatives à cette nouvelle connexion sont renseignées dans newSocket
 *  \param		socket: socket en mode STREAM
 *  \param		newSocket: la socket client en mode STREAM
 * 	\result		indicateur de réussite POSIX
 * 
 */
int acceptRemote(socket_t* socket, socket_t* newSocket) {
	struct sockaddr_in remoteAddress;
	socklen_t clientAddressLength = sizeof(remoteAddress);
	int remoteFileDescriptor = accept(
		socket->fileDescriptor, (struct sockaddr*)&remoteAddress, &clientAddressLength
	);
	if (remoteFileDescriptor == -1) { return -1; }
	createSocket(SOCK_STREAM, newSocket);
	newSocket->fileDescriptor = remoteFileDescriptor;
	newSocket->local = socket->local;
	newSocket->remote = remoteAddress;
	return 0;
}

/**
 * 
 * 	\fn			int connectToRemote(socket_t* socket, const char* address, const short port);
 * 	\brief		Se connecte à un serveur, remplissant le champ .remote
 *  \param		socket: socket en mode STREAM
 * 	\param		address: adresse IP
 * 	\param		port: port
 * 	\result		indicateur de réussite POSIX
 * 
 */
int connectToRemote(socket_t* socket, const char* address, const short port) {
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	inet_pton(AF_INET, address, &serverAddress.sin_addr);
	socklen_t serverAddressLength = sizeof(serverAddress);
	int status = connect(
		socket->fileDescriptor,
		(struct sockaddr*)&serverAddress, serverAddressLength
	);
	if (status == -1) { return -1; }
	socket->remote = serverAddress;
	return 0;
}

/**
 * 
 * 	\fn			int setRemote(socket_t* socket, const char* address, const short port);
 * 	\brief		Indique l'addresse de destination de la socket.
 *  \param		socket: socket en mode DGRAM
 * 	\param		address: adresse IP
 * 	\param		port: port
 * 	\result		indicateur de réussite POSIX
 * 
 */
int setRemote(socket_t* socket, const char* address, const short port) {
	struct sockaddr_in addressObject;
	memset(&addressObject, 0, sizeof(addressObject));
	addressObject.sin_family = AF_INET;
	addressObject.sin_port = htons(port);
	if (1 != inet_pton(AF_INET, address, &addressObject.sin_addr)) { return -1; }
	socket->remote = addressObject;
	return 0;
}
