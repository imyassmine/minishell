#include "gc.h"

void	*gc_malloc(size_t size, t_gc *gc)
{
	void		*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("Memory allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, size);
	return (ptr);
}
