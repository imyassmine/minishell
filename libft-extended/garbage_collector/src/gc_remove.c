#include "gc.h"

void	gc_remove(t_gc *gc, void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	current = gc->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				gc->head = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
