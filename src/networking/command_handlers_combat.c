#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#include "command_handlers_combat.h"
#include "server.h"

void* handleFlee(command_context_t* context){
    //TODO

    if (TODO){ //si fin du match
        pushFunction(&user->commandHandlers, gameWorldHandler); //retour en mode normal
    }
    
}

void* handleAttack(command_context_t* context){
    //TODO

    if (TODO ){ //si fin du match
        pushFunction(&user->commandHandlers, gameWorldHandler); //retour en mode normal
    }
}


void* combatCommandHandler(command_context_t* context){

    if (context->count == 3){// nom commande + 1 autre argument, eg FLEE <int>
        if (strcmp(context->args[1], "FLEE") == 0){
            handleFlee(context);
            return NULL;
        }
    }

    if (context->count == 5){   // nom commande + 3 autres arguments, eg ATTACK <int> <int> <int>
        if (strcmp(context->args[1], "ATTACK") == 0) {
            handleAttack(context);
            return NULL;
        }
    }
    

    //si aucun cas ci-dessus n'a été activé
    printf("(!) unable to parse command\n");
	return NULL;
}