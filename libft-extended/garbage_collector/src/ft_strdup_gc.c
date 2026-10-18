#include "gc.h"

char	*ft_strdup_gc(const char *s1, t_gc *gc)
{
	size_t		i;
	char		*str;

	if (!s1)
		return (ft_strdup_gc("", gc));
	i = -1;
	str = gc_malloc(sizeof(char) * (ft_strlen(s1) + 1), gc);
	while (s1[++i])
		str[i] = s1[i];
	str[i] = '\0';
	return (str);
}
