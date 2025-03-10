#include "get_next_line.h"

int find_new_line(char buffer[])
{
	int i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i + 1);
		i++;
	}
	return (-1);
}

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

char *copy_str(char buffer[], int till)
{
	int i = 0;
	if (!buffer || till < 0)
		return (NULL);
	char *result = malloc(till + 1);
	if (!result)
		return (NULL);
	while (i < till)
	{
		result[i] = buffer[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char *realloc_result(char **old_result, char buffer[], int end)
{
	char *new_result;
	int i = 0;
	int j = 0;

	if (!old_result || !buffer || end < 0)
		return (NULL);
	new_result = malloc(ft_strlen(*old_result) + end + 1);
	if (!new_result)
		return (NULL);
	while ((*old_result)[i])
	{
		new_result[i] = (*old_result)[i];
		i++;
	}
	while (j < end)
	{
		new_result[i] = buffer[j];
		i++;
		j++;
	}
	new_result[i] = '\0';
	free(*old_result);
	return (new_result);
}

char *gen_str(char **result, char buffer[], int till)
{
	if (!*result)
	{
		*result = copy_str(buffer, till);
		if (!*result)
			return NULL;
	}
	else
	{
		*result = realloc_result(result, buffer, till);
		if (!*result)
			return NULL;
	}
}

void	deal_with_remainder(char **result, char buffer[], char **remainder, int new_line)
{
	int i = 0;

	*result = gen_str(result, buffer, new_line);
	if (!*result)
		return ;
	*remainder = malloc(ft_strlen(buffer) - new_line + 1);
	if (!*remainder)
		return ;
	while (buffer[new_line])
	{
		(*remainder)[i] = buffer[new_line];
		i++;
		new_line++;
	}
	(*remainder)[i] = '\0';
}

int get_remainder(char **result, char **remainder)
{
	if (!*remainder)
		return (0);
	int new_line = find_new_line(*remainder);
	int i;
	if (new_line == -1) // не найдена \n
	{
		*result = copy_str(*remainder, ft_strlen(*remainder));
		if (!*result)
			return -1;
		free(*remainder);
		*remainder = NULL;
		return (0);
	}
	*result = copy_str(*remainder, new_line);
	if (!*result)
		return -1;
	char *new_remainder = malloc(ft_strlen(*remainder) - new_line + 1);
	if (!new_remainder)
		return -1;
	i = 0;
	while ((*remainder)[new_line])
	{
		new_remainder[i] = (*remainder)[new_line];
		i++;
		new_line++;
	}
	new_remainder[i] = '\0';
	free(*remainder);
	*remainder = new_remainder;
	return (1);
}

char *get_next_line(int fd)
{
	if (fd < 0)
		return (NULL);
	ssize_t bytes_read;
	char buffer[BUFFER_SIZE + 1];
	static char *remainder = NULL;
	char *result = NULL;

	int status = get_remainder(&result, &remainder);
	if (status)
		return result;
	if (status == -1)
		return (NULL);
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(result), free(remainder), NULL);
		buffer[bytes_read] = '\0';
		if (bytes_read == 0)
		{
			if (remainder)
			{
				free(remainder);
				remainder = NULL;
			}
			if (result)
				return result;
			return NULL;
		}
		if (find_new_line(buffer) != -1)
			return (deal_with_remainder(&result, buffer, &remainder, find_new_line(buffer)), result);
		result = gen_str(&result, buffer, ft_strlen(buffer));
	}
}

int main()
{
	int fd = open("file.txt", O_RDONLY);
	char *str;

	str = get_next_line(fd);
	while (str)
	{
		printf("%s", str);
		str = get_next_line(fd);
	}
}