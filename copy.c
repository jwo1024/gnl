#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>

/*
#include	<fcntl.h>
#include	<stdio.h>

int	main(void)
{
	//int	a;
	char	*s;

//	a = open("tst.txt", O_RDONLY);
	while (1)
	{
		s = get_next_line(1);
		printf("result @%s@\n", s);
		if (s == NULL)
			break;
	}

//	s = get_next_line(0);
//	printf("result @%s@\n", s);
	return (0);
}
*/

#define	BUFFER_SIZE 42
// EOF값은 stdio.h 에 define 되어 있음, 이걸 써도 되는 건가??
// EOF 입력은 CTRL + D

typedef struct s_gnl_list
{
	int		gnl_fd;  // fd 0 표준입력, 1 표준출력, 2 표준에러
	// fd 번호; 최대 rlim_fd_max // getconf OPEN_MAX fd의 최댓값을 알 수 있는 명령어 // mac m1 -> 2561 // int ? size_t ? 
	char	buff[BUFFER_SIZE + 1]; // 이거 되나? 되겟지?
	char	*buff_cur;
	t_gnl_list *next;
} t_gnl_list ;

static t_gnl_list gnl_head;

char	*get_next_line(int fd)
{
	t_gnl_list *now;
	char	*str;

	now = gnl_get_fdlist(fd);

	while (now && now->buff_cur[0 - 1] != '\n') // index 가능한가???????? 0 - 1 ?? 
	{
		str = gnl_read_fd(fd, now);

	}

	return (str);

	// now = 해당 fd의 버퍼 존재확인
	//		있으면 해당 버퍼 반환
	//		없다면 head 뒤에 추가하여 만 해당 list 주소 반환
	//
	// 1) buff 에 있는지 확인 - 없다면 '2' 번으로 있다면 '3' 번으로
	// 2) now->buff = read(fd); fd 읽어서 buff에 저장
	//		맨 처음 읽은것이 EOF 혹은, fd 읽는 것에 실패시, NULL 리턴
	// 3) 기존이 읽은 index == buff_cur, 혹은 처음부터 ~ \n, EOF 혹은 끝까지 len 세기,
	//		그 구간 malloc 해서 기존 str 에 붙여 저장. str 이 없었다면 새로 할당하여 저장.
	//		+ 어디까지 읽었는지 혹은 끝까지 읽었는지 혹은 EOF로 끝났는지 저장
	// 4) 끝까지 일반 문자열일 경우 - '2' 번으로가 반복
	//
	// \n 이었을 경우 - 어디까지 읽었는지 저장 , 만약 \n 이 버퍼 끝이라면 NULL 로 설정
	// EOF 이었을 경우 - fd해당 연결리스트 할당 해제 및 삭제 + 다른 리스트 연결 시켜두기
	//		앞의 리스트, 뒤의 리스트 합쳐서 저장,
	//
	// str 반환
}



///////////////////////////////
char	*gnl_read_fd(int fd, t_gnl_list	*now)
{
	char	*str;
	int		i;

	if (now->buff_cur == NULL || now->buff_cur[0] == 0)
	{
		// read 함수는 성공시 0보다 큰수를, eof를 만났을 때는 0을, 에러시 -1 반환 // ssize_t로 반환?
		i = read(fd,now->buff, BUFFER_SIZE);
		if (i == -1) //read 실패
			return (NULL); // + 메모리 해제해야함.. !!!!!!!!!!!! new ego 적대적인 고난과 슬픔 널 더 팝인 진화 시켜 u wiwi fallow watch me workit worit out
		else if (i == 0)
			; // eof 를 만났을때 처리
		now->buff_cur = now->buff;// 읽어서 buffer 에 저장 // 기존에 데이터가 있다면 위에 씌워서 되나? 된다고 가정하고 써보자 x 안된댜;
	}
	gnl_strjoin(str, now); // 무조건 buff_cur 이 유효한것 가리키고 있을때
	if (i == 0) 
		gnl_free_list(now);// 메모리 헤제//

	return (str); //다시 돌아가는거 어떻게 하지?
}

int	gnl_strjoin(char *str, t_gnl_list *now)
{
	int		len;
	int		i;
	char	*tmp;

	len = 0;
	while (now->buff_cur[len] != 0 && now->buff_cur[len] != '\n')
		len++;
	tmp = str;
	str = (char *)malloc(sizeof(char) * (gnl_strlen(str) + len));
	if (str)
	{
		i = 0;
		while (i < gnl_strlen(tmp))
		{
			str[i] = tmp[i];
			i++;
		}
		while (i < gnl_strlen(tmp) + len)
		{
			str[i] = now->buff_cur++;
			i++;
		}
		str[i] == 0;
		return (1);
	}
	//if (now->buff[BUFFER_SIZE] == now->buff_cur) + eof 일때
	// now->buff = NULL; // ????????? 0으로 초기화하고 싶음 memset 을 사용?
	return (0); //?? 실패시 에러 뜻하는 0을 반환 한다고 쳤을때 어차피 상위 함수에서 바로 str을 내보내는데 상관이 있나? 굳이 ? 
}

int	gnl_free_list(t_gnl_list *now)
{
	t_gnl_list	*tmp;

	tmp = &gnl_head;
	while (tmp->next != NULL)
	{
		if (tmp->next->gnl_fd == now->gnl_fd)
		{
			tmp->next = now->next;
			free(now);
			return (1); //굳이 반환값이 필요한가?
		}
		tmp = tmp->next;
	}
	return (0);
}
//////////////////////////////





////////////////////////////
t_gnl_list	*gnl_get_fdlist(int fd)
{
	t_gnl_list	*rtn;

	rtn = gnl_find_fdlist(fd);
	if (rtn == NULL)
	{
		rtn = (t_gnl_list *)malloc(sizeof(t_gnl_list)); // 배열이 아니니까 + 1 안해도 괜찮것지 대신 memset 으로 
		if (rtn)
			rtn = gnl_memset(rtn, 0, sizeof(t_gnl_list));
	}
	return (rtn);
}

t_gnl_list	*gnl_find_fdlist(fd)
{
	t_gnl_list	*tmp;

	tmp = &gnl_head;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
		if (tmp->gnl_fd == fd)
			return (tmp);
	}
	return (NULL);
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
////////////////////////////