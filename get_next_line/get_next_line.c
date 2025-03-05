#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

# ifndef BUFFER_SIZE
#	define BUFFER_SIZE 5
# endif

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

int	find_new_line(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

void	deal_remainder(int start, char buffer[], char **remainder)
{
	int i = 0;

	*remainder = malloc(ft_strlen(buffer) - start + 1);
	if (!*remainder)
		return ;
	while (start < ft_strlen(buffer))
	{
		(*remainder)[i] = buffer[start];
		i++;
		start++;
	}
	(*remainder)[i] = '\0';
}

int	copy_str(char **result, char buffer[], char **remainder)
{
	int end = find_new_line(buffer);

	*result = malloc(end + 1);
	if (!*result)
		return (-1);
	int i = 0;
	while (i < end)
	{
		(*result)[i] = buffer[i];
		i++;
	}
	(*result)[i] = '\0';
	deal_remainder(end, buffer, remainder);
	return (1);
}

int	overwrite_buffer(char **result, char **remainder, char buffer[])
{
	if (!*result)
	{
		return (copy_str(result, buffer, remainder));
	}
	char *new_result = NULL;
	int i = 0;
	int j = 0;

	new_result = malloc(ft_strlen(*result) + find_new_line(buffer) + 1);
	if (!new_result)
		return (-1);
	while (i < ft_strlen(*result))
	{
		new_result[i] = (*result)[i];
		i++;
	}
	while (j < find_new_line(buffer))
	{
		new_result[i] = buffer[j];
		i++;
		j++;
	}
	new_result[i] = '\0';
	free(*result);
	*result = new_result;
	deal_remainder(find_new_line(buffer), buffer, remainder);
}

int	get_data(char **result, char **remainder, char buffer[])
{
	if (find_new_line(buffer))
		return (overwrite_buffer(result, remainder, buffer));
	
}

char *get_next_line(int fd)
{
	ssize_t bytes_read;
	static char *remainder = NULL;
	char *result = NULL;
	char buffer[BUFFER_SIZE];
	int status;
	printf("remainder: %s\n", remainder);
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		if (bytes_read == 0)
			return (free(remainder), result);
		status = get_data(&result, &remainder, buffer);
		if (status == -1)
			return (NULL);
		if (status)
			return (result);
	}
}

int main()
{
	int fd = open("/home/alex/42_learning/exam_03/get_next_line/file.txt", O_RDONLY);
	char *str = get_next_line(fd);
	printf("%s", str);
	str = get_next_line(fd);
	// printf("%s", str);
	// while (str)
	// {
	// 	printf("%s", str);
	// 	str = get_next_line(fd);
	// }
	return (0);
}