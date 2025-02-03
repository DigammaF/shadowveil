
#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#include "place.h"

/**
 * 
 *  Attention, la quantité de mémoire requise pour stocker le monde est fonction cubique de cette constante.
 * 	Une constante trop grande va provoquer un seg fault.
 * 
 * 	L'analyse du fichier dump correspondant donne:
 * 
 * 		Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x0000561e6c08569a in mainServer (argc=<error reading variable: Cannot access memory at address 0x7ff5b5e5c53c>,
    argv=<error reading variable: Cannot access memory at address 0x7ff5b5e5c530>) at src/networking/server.c:224
224     int mainServer(int argc, const char** argv) {
(gdb) info locals
server = <error reading variable server (value of type `server_t' requires 34358689880 bytes, which is more than max-value-size)>
 * 
 */
#define WORLD_SIZE 600

typedef struct {
    place_t* places[WORLD_SIZE][WORLD_SIZE]; // possède la valeur
} world_t;

void initWorld(world_t* world);
void dropWorld(world_t* world);

void generateWorld(world_t* world);

#endif
