#include "libft.h"

int	ft_isspace(char c)
{
	return ((c >= '\t' && c <= '\r') || c == ' ');
}
