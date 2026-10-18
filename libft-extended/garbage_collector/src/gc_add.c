#include "gc.h"

void	gc_add(t_gc *gc, void *ptr, size_t size)
{
	t_gc_node	*new_node;

	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		perror("Failed to add a new node");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->next = gc->head;
	new_node->locked = false;
	gc->head = new_node;
}
