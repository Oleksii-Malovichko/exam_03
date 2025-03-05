#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gen_str(char **str1, char **str2)
{
	int	i = 0;
	int	j = 0;
	char	*new_str;

	while ((*str1)[i])
		i++;
	
}

int main()
{
	char *s1 = strdup("Hello, ");
	char *s2 = strdup("world\n");
	gen_str(&s1, &s2);
	printf("%s", s1);
}