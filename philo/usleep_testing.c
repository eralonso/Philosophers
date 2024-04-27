#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define THREAD_NB 200

long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	do_sleep(long time)
{
	long	f_time;

	f_time = time + get_time();
	while (get_time() < f_time)
		usleep(100);
	//usleep(time * 1000);
}

void	do_count(long time)
{
	long	s;
	long	e;

	
	s = get_time();
	do_sleep(time);
	e = get_time();
	if (time != e - s)
		printf("time: %ld ms\n", e - s);
}

void	*thread_routine(int *time)
{
	int	i;

	i = 0;
	while (i < 100)
	{
		do_count(*time);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	int			time;
	int			i;
	pthread_t	id[THREAD_NB];

	time = 60;
	i = 0;
	while (i < THREAD_NB)
	{
		pthread_create(id + i, NULL, (void *)thread_routine, &time);
		i++;
	}
	i = 0;
	while (i < THREAD_NB)
	{
		pthread_join(id[i], NULL);
		i++;
	}
	return (0);
}
