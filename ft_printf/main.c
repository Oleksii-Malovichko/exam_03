#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

int	ft_printf(const char *format, ...)
{
	int	len;
	va_list arguments;

	len = 0;
	va_start(arguments, format);
	while (format[len])
	{
		
		len++;
	}
	va_end(arguments);
}

int main()
{
	ft_printf("hello\n", "hello");
}