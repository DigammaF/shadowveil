
#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

void* initialHandler(void* arg);

void* handleFlee(command_context_t* context);
void* handleAttack(command_context_t* context);

/** 
 * @brief Mets en action les commandes textuelles qui ne sont pas liées au combat.
*/
void* gameWorldHandler(void* arg);
void* adminHandler(void* arg);

void* debugEchoHandler(void* arg);

#endif
