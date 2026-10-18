#include "libft.h"

size_t	ft_tabstrlen(char **tabstr)
{
	size_t	i;

	i = 0;
	while (tabstr && tabstr[i])
		i++;
	return (i);
}
