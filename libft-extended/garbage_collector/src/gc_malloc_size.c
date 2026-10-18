#include "gc.h"

size_t	gc_malloc_size(void *ptr, t_gc *gc)
{
	t_gc_node	*current;

	current = gc->head;
	while (current)
	{
		if (current->ptr == ptr)
			return (current->size);
		current = current->next;
	}
	return (0);
}
