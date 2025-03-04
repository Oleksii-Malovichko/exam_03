#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

void	ft_putnbr(int n, int *result)
{
	int r;

	if (n < 0)
	{
		if (write(1, "-", 1) == -1)
		{
			*result = -1;
			return ;
		}
		n = -n;
		(*result)++;
	}
	if (n >= 10)
	{
		ft_putnbr(n / 10, result);
	}
	r = (n % 10) + '0';
	if (write(1, &r, 1) == -1)
	{
		*result = -1;
		return ;
	}
	(*result)++;
}

void	ft_putstr(char *str, int *result)
{
	while (*str)
	{
		if (write(1, str, 1) == -1)
		{
			*result = -1;
			return ;
		}
		(*result)++;
		str++;
	}
}

void	ft_puthex(unsigned int n, int *result)
{
	char	*buffer = "0123456789abcdef";

	if (n > 16)
	{
		ft_puthex(n / 16, result);
	}
	if (write(1, &buffer[n % 16], 1) == -1)
	{
		*result = -1;
		return ;
	}
	(*result)++;
}

void	go_to_cycle(const char *str, char ch, va_list args, int *result)
{
	int		result1;
	char	*result2;
	unsigned int		result3;

	if (ch == 'd')
	{
		result1 = va_arg(args, int);
		ft_putnbr(result1, result);
	}
	if (ch == 's')
	{
		result2 = va_arg(args, char *);
		ft_putstr(result2, result);
	}
	if (ch == 'x')
	{
		result3 = va_arg(args, unsigned int);
		ft_puthex(result3, result);
	}
}

char	compare_with_right_sym(const char *str, char *sym, va_list args, int *result)
{
	int	i;

	i = 0;
	while (sym[i])
	{
		if (sym[i] == *str)
			return (sym[i]);
		i++;
	}
	return (0);
}

void	check_other_sym(const char *str, char *sym, va_list args, int *result)
{
	char ch;

	ch = compare_with_right_sym(str, sym, args, result);
	if (ch)
	{
		go_to_cycle(str, ch, args, result);
	}
	else
	{
		if (write(1, str - 1, 2) == -1)
		{
			*result = -1;
			return ;
		}
		(*result)+=2;
	}
}

int	check_percent(const char *str, char *sym, va_list args, int *result)
{
	if (*str == '%')
	{
		if (write(1, "%", 1) == -1)
			return (-1);
		(*result)++;
	}
	else
	{
		check_other_sym(str, sym, args, result);
		if (*result == -1)
			return (-1);
	}
}

int	start_process(const char *str, char *sym, va_list args)
{
	int	result;

	result = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			if (check_percent(str, sym, args, &result) == -1)
				return (-1);
		}
		else
		{
			if (write(1, str, 1) == -1)
				return (-1);
			result++;
		}
		str++;
	}
	return (result);
}

int	ft_printf(const char *str, ...)
{
	int		result;
	char	*sym = "sdx";
	va_list args;

	va_start(args, str);
	result = start_process(str, sym, args);
	va_end(args);
	return (result);
}

int main()
{
	printf("%d\n",ft_printf("hello %d and %s and hex %x\n", -225, "hello", -233));
	printf("%d\n", printf("hello %d and %s and hex %x\n", -225, "hello", -233));
}
// hello % and %r