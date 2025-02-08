
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <ncurses.h>

#include "client.h"
#include "lantern.h"
#include "constants.h"
#include "function_stack.h"
#include "vector.h"

#define UNUSED(x) (void)(x)
#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initClient(client_t* client) {
	initFunctionStack(&client->contextHandlers);
}

void dropClient(client_t* client) {
	dropFunctionStack(&client->contextHandlers);
}

void setupClientFileDescriptorSet(client_t* client, fd_set* fileDescriptorSet, int* maxFileDescriptor) {
	socket_t* clientSocket = &client->socket;
	FD_ZERO(fileDescriptorSet);
	
	//écoute du serveur via le socket client
	FD_SET(clientSocket->fileDescriptor, fileDescriptorSet);
	*maxFileDescriptor = clientSocket->fileDescriptor;
	
	//écoute du clavier local
	int stdinFileDescriptor = fileno(stdin);
	FD_SET(stdinFileDescriptor, fileDescriptorSet);
	if (stdinFileDescriptor > *maxFileDescriptor) { *maxFileDescriptor = stdinFileDescriptor; }
	
	return;
}

void handleClientSockets(client_t* client, fd_set* fileDescriptorSet){
	socket_t* clientSocket = &client->socket;

	if (FD_ISSET(clientSocket->fileDescriptor, fileDescriptorSet)) {
		char line[1024];
		int byteCount = recvData(clientSocket, line, 1024);
		if (byteCount == 0) {
			printf("(-) le serveur a mis fin à la connexion\n");
			client->running = 0;
			return;
		}
		printf("(received) '%s'\n", line);

		// for (unsigned n = 0; n < 1024; n++) {
		// 	printf("%c", line[n]);
		// }
		// printf("\n");
	}

	if (FD_ISSET(fileno(stdin), fileDescriptorSet)) {
		char line[512];
		fgets(line, sizeof(line), stdin);
		line[strlen(line) - 1] = '\0';
		printf("(sending) '%s'\n", line);
		char sent[1024];
		sprintf(sent, "COMMAND %s", line);
		sendData(clientSocket, sent);
	}
	
	return;
}

void awaitInput(client_t* client) {
	int maxFileDescriptor;
	struct timeval timeout = { 0, SERVER_TICK };
	setupClientFileDescriptorSet(client, &client->ioState, &maxFileDescriptor);
	select(maxFileDescriptor + 1, &client->ioState, NULL, NULL, &timeout);
}

int mainClient(int argc, const char* argv[]) {
	UNUSED(argc); UNUSED(argv);
	socket_t clientSocket;
	connectServer(&clientSocket, "127.0.0.1", SERVER_PORT);
	client_t client;
	initClient(&client);
	client.socket = clientSocket;
	client.running = 1;
	
	while (client.running) {
		awaitInput(&client);
		handleClientSockets(&client, &client.ioState);
	}

	closeSocket(&clientSocket);
	dropClient(&client);
	return 0;
}

void displayMenu(WINDOW* window, unsigned select, char** options, unsigned optionCount) {
    int x = 2, y = 1;
    box(window, 0, 0);

    for (unsigned n = 0; n < optionCount; n++) {
        if (n == select) { wattron(window, A_REVERSE); }
        mvwprintw(window, y + n, x, "%s", options[n]);
        wattroff(window, A_REVERSE);
    }
}

void* initialClientHandler(void* _) {
	client_t* client = _;

	char* options[] = { "(I)dentification", "(E)nregistrement" };
	unsigned optionCount = 2;
	unsigned select = 0;

	int startx = (COLS - 20) / 2;
    int starty = (LINES - 2) / 2;
	WINDOW* window = newwin(4, 20, starty, startx);
    keypad(window, TRUE);

	while (1) {
		displayMenu(window, select, options, optionCount);
		awaitInput(client);

		if (FD_ISSET(fileno(stdin), &client->ioState)) {
			int character = wgetch(window);
			switch (character) {
				case KEY_UP:
					if (select > 0) { select--; }
					break;

				case KEY_DOWN:
					if (select < optionCount - 1) { select++; }
					break;

				case KEY_ENTER:
					if (select == 0) {
						pushFunction(&client->contextHandlers, loginClienthandler);
						delwin(window);
						return NULL;
					}
					break;
			}
		}

		wrefresh(window);
	}

	return NULL;
}

void* loginClienthandler(void* _) {
	client_t* client = (client_t*)_;

	unsigned width = 60;
	int startx = (COLS - width) / 2;
	int starty = (LINES - 10) / 2;
	WINDOW* window = newwin(4, width, starty, startx);

	char login[MAX_INPUT] = { 0 };
	char password[MAX_INPUT] = { 0 };
	unsigned loginHead = 0;
	unsigned passwordHead = 0;
	unsigned editionMode = 0;

	box(window, 0, 0);
    mvwprintw(window, 0, 2, " Nom d'utilisateur ");
	mvwprintw(window, 1, 2, " Mot de passe ");
	
	while (1) {
		awaitInput(client);

		if (FD_ISSET(fileno(stdin), &client->ioState)) {
			int character = wgetch(window);
	
			if (character == KEY_ENTER) {
				login[loginHead] = '\0';
			} else if (character == KEY_BACKSPACE) {
				if (loginHead > 0) {
					loginHead--;
					mvwprintw(window, 1, loginHead + 2, " ");
					wmove(window, 1, loginHead + 2);
				}
			} else if (loginHead < MAX_INPUT && character >= 32 && character <= 126) {
				login[loginHead] = character;
				loginHead++;
				mvwprintw(window, 1, loginHead + 1, "%c", character);
			}
		}
	}

	wrefresh(window);
	return NULL;
}

void* registerClientHandler(void* _) { UNUSED(_); return NULL; }

void* gameworldClientHandler(void* _) { UNUSED(_); return NULL; }

void* moveClientHandler(void* _) { UNUSED(_); return NULL; }

void* interactClientHandler(void* _) { UNUSED(_); return NULL; }

void* buyItemClientHandler(void* _) { UNUSED(_); return NULL; }

void* buyChampionClienthandler(void* _) { UNUSED(_); return NULL; }

void* sellItemClienthandler(void* _) { UNUSED(_); return NULL; }

void* sellChampionClientHandler(void* _) { UNUSED(_); return NULL; }

int mainGUIClient(int argc, const char* argv[]) {
	UNUSED(argc); UNUSED(argv);
	socket_t clientSocket;
	connectServer(&clientSocket, "127.0.0.1", SERVER_PORT);
	client_t client;
	initClient(&client);
	client.socket = clientSocket;
	client.running = 1;
	pushFunction(&client.contextHandlers, initialClientHandler);
	// ncurses
	initscr(); clear(); noecho(); cbreak(); keypad(stdscr, TRUE);

	while (client.running) {
		function_t handler = peekFunction(&client.contextHandlers);
		handler((void*)&client);

		if (functionStackEmpty(&client.contextHandlers)) { client.running = 0; }
	}

	endwin();
	closeSocket(&clientSocket);
	dropClient(&client);
	return 0;
}
