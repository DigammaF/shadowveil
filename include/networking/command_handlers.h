
#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

void* initialHandler(void* arg);
void* gameWorldHandler(void* arg);
void* adminHandler(void* arg);

/**  
 * 
 * 	\brief		Mettre à jour la position du pawn associé au joueur,
 *              et envoyer un message en conséquence.
 *  \param		context: le contexte actuel
 *  \param		destinationKey: clé correspondant à la destination dans pawn->place->links
 * 
 */
void handleMove(command_context_t* context, int destinationKey){

void* debugEchoHandler(void* arg);

#endif
