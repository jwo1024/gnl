#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include	<unistd.h>
# include	<stdlib.h>

typedef struct s_gnl_list
{
	int					gnl_fd;
	char				*buff;
	char				*buff_cur;
	struct s_gnl_list	*next;
}	t_gnl_list;

char		*get_next_line(int fd);
int			gnl_read_fd(int fd, t_gnl_list	*now, char **str);
int			gnl_read_buffer(char **str, t_gnl_list *now);
int			gnl_free_list(t_gnl_list *now, t_gnl_list *gnl_head);
size_t		gnl_strlen(const char *s);
t_gnl_list	*gnl_get_fdlist(int fd, t_gnl_list *gnl_head);
t_gnl_list	*gnl_find_fdlist(int fd, t_gnl_list *gnl_head);
void		*gnl_memset(void *b, int c, size_t len);
int			gnl_strjoin(char **str, size_t str_len, t_gnl_list *now);

#endif