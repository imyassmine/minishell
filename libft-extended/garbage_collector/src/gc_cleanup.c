#include "gc.h"

void	gc_cleanup(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	current = gc->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
}
