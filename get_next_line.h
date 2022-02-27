#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include	<unistd.h>
# include	<stdlib.h>

//# define	BUFFER_SIZE 42 // 삭제해야함

typedef struct s_gnl_list
{
	int		gnl_fd;
	char	buff[BUFFER_SIZE + 1]; // 이거 되나? 되겟지?
	char	*buff_cur;
	struct s_gnl_list *next;
} t_gnl_list ;

static t_gnl_list gnl_head;

char	*get_next_line(int fd);
int	gnl_read_fd(int fd, t_gnl_list	*now, char **str);
int	gnl_strjoin(char **str, t_gnl_list *now);
int	gnl_free_list(t_gnl_list *now);
size_t	gnl_strlen(const char *s);
t_gnl_list	*gnl_get_fdlist(int fd);
t_gnl_list	*gnl_find_fdlist(int fd);
void	*gnl_memset(void *b, int c, size_t len);




#endif