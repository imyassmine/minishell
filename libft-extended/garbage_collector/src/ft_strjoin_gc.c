#include "gc.h"

char	*ft_strjoin_gc(const char *s1, const char *s2, t_gc *gc)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*s;

	if (!s1 && !s2)
		return (ft_strdup_gc("", gc));
	if (!s1)
		return (ft_strdup_gc(s2, gc));
	if (!s2)
		return (ft_strdup_gc(s1, gc));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s = gc_malloc(sizeof(char) * (s1_len + s2_len + 1), gc);
	ft_strlcpy(s, s1, s1_len + 1);
	ft_strlcat(s + s1_len, s2, s2_len + 1);
	return (s);
}
