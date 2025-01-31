
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "function_stack.h"

#define CHECKM(status, message) { if ((status) == NULL) { perror(message); exit(EXIT_FAILURE); } }

void initFunctionStack(function_stack_t* stack) {
	stack->node = NULL;
}

void dropFunctionStack(function_stack_t* stack) {
	while (!functionStackEmpty(stack)) { popFunction(stack); }
}

int isFunctionStackEmpty(function_stack_t* stack) {
	return stack->node == NULL;
}

void pushFunction(function_stack_t* stack, function_t value) {
	function_stack_node_t* node = malloc(sizeof(function_stack_node_t));
	CHECKM(node, "node");
	node->value = value;
	node->previous = (struct function_stack_node_t*)stack->node;
	stack->node = node;
}

function_t popFunction(function_stack_t* stack) {
	function_stack_node_t* node = stack->node;
	stack->node = (function_stack_node_t*)node->previous;
	function_t value = node->value;
	free(node);
	return value;
}

function_t peekFunction(const function_stack_t* stack) {
	return stack->node->value;
}
