#ifndef GC_H
# define GC_H

/* ================================ INCLUDES ================================ */

# include <stdio.h>
# include <stdbool.h>
# include "libft.h"

/* ================================ STRUCTURES =============================== */

typedef struct s_gc_node
{
	void				*ptr;
	size_t				size;
	struct s_gc_node	*next;
	bool				locked;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}	t_gc;

/* =========================== FUNCTION PROTOTYPES ========================== */

void	gc_init(t_gc *gc);
void	gc_add(t_gc *gc, void *ptr, size_t size);
void	gc_remove(t_gc *gc, void *ptr);
void	gc_free(void *ptr, t_gc *gc);
void	gc_cleanup(t_gc *gc);
void	gc_cleanup_except_locked(t_gc *gc);
void	*gc_malloc(size_t size, t_gc *gc);
void	*gc_calloc(size_t nmemb, size_t size, t_gc *gc);
size_t	gc_malloc_size(void *ptr, t_gc *gc);
void	*gc_realloc(void *ptr, size_t new_size, t_gc *gc);
char	*ft_substr_gc(char *s, unsigned int start, size_t len, t_gc *gc);
char	*ft_strjoin_gc(char const *s1, char const *s2, t_gc *gc);
char	*ft_strdup_gc(const char *s1, t_gc *gc);
char	*ft_itoa_gc(int n, t_gc *gc);
char	**ft_split_gc(char *s, char c, t_gc *gc);

#endif
