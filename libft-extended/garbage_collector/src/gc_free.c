#include "gc.h"

void	gc_free(void *ptr, t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	if (!ptr || !gc || !gc->head)
		return ;
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
			free(current->ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
