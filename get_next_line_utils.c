/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiwolee <jiwolee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 23:37:18 by jiwolee           #+#    #+#             */
/*   Updated: 2022/03/02 23:37:39 by jiwolee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"get_next_line.h"

t_gnl_list	*gnl_get_fdlist(int fd, t_gnl_list *head)
{
	t_gnl_list	*list;

	list = NULL;
	list = gnl_find_fdlist(fd, head);
	if (list != NULL)
		return (list);
	list = (t_gnl_list *)malloc(sizeof(t_gnl_list));
	if (list)
	{
		list = gnl_memset(list, 0, sizeof(t_gnl_list));
		list->gnl_fd = fd;
		list->buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (list->buff == NULL)
		{
			free(list);
			return (NULL);
		}	
		gnl_memset(list->buff, 0, BUFFER_SIZE + 1);
		while (head->next != NULL)
			head = head->next;
		head->next = list;
	}
	return (list);
}

t_gnl_list	*gnl_find_fdlist(int fd, t_gnl_list *head)
{
	t_gnl_list	*tmp;

	tmp = head;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->gnl_fd == fd)
			return (tmp);
	}
	return (NULL);
}

int	gnl_free_list(t_gnl_list *list, t_gnl_list *head)
{
	t_gnl_list	*tmp;

	tmp = head;
	while (tmp->next != NULL)
	{
		if (tmp->next->gnl_fd == list->gnl_fd)
		{
			free(list->buff);
			tmp->next = list->next;
			free(list);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

void	*gnl_memset(void *b, int c, size_t len)
{
	unsigned char	*str;
	size_t			i;

	str = b;
	i = 0;
	while (i < len)
		str[i++] = (unsigned char)c;
	return (str);
}
