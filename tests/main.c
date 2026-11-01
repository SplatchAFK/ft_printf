#include "../ft_printf.h"

int	main(int ac, char **av)
{
	int n;

	(void) ac;
	n = ft_atoi((const char *)av[1]);
	ft_printf("voici un test : %d", n);
	return (0);
}
