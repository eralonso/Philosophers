/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:45:10 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/07 14:01:41 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include	<limits.h>
# include	<stdio.h>
# include	<stdlib.h>
# include	<unistd.h>
# include	<pthread.h>
# include	<sys/time.h>
# include	<sys/types.h>
# include	<sys/wait.h>
# include	<sys/stat.h> 
# include	<fcntl.h>
# include	<signal.h>
# include	<semaphore.h>

//Define: Philo logs
# define HTK	(char *)"has taken a fork"
# define ISE	(char *)"is eating"
# define ISS	(char *)"is sleeping"
# define IST	(char *)"is thinking"
# define DEAD	(char *)"died"

//Define: Errors messages
# define ERR_ARGS_1		(char *)"\033[7;97m ❌ \033[0m Error: \
\'Invalid number of arguments\': ./philo number_of_philosophers time_to_die \
time_to_eat time_to_sleep <number_of_times_each_philosopher_must_eat>"
# define ERR_ARGS_2		(char *)"\033[7;97m ❌ \033[0m Error: \
\'Invalid arguments\': Arguments must be an integers"
# define ERR_SEM		(char *)"\033[7;97m ❌ \033[0m Error: \
\'sem_open()\'"
# define ERR_FORK		(char *)"\033[7;97m ❌ \033[0m Error: \
\'fork()\': Process "
# define ERR_PTHREAD	(char *)"\033[7;97m ❌ \033[0m Error: \
\'pthread_create()\': Thread "

//Typedef: Synonymous
typedef long long int	t_lli;

//Typedef: Structs
typedef struct s_philo	t_philo;
typedef struct s_table	t_table;
typedef struct s_times	t_times;

//Structs
struct s_times
{
	int	to_die;
	int	to_eat;
	int	to_sleep;
};

struct s_philo {
	int			n;
	int			times_eat;
	t_lli		last_eat;
};

struct s_table {
	sem_t		*print;
	sem_t		*life_check;
	sem_t		*forks;
	sem_t		*finish;
	sem_t		*dead;
	pid_t		*pid;
	pthread_t	th_dead;
	pthread_t	th_finish;
	int			end;
	t_philo		philo;
	t_times		time;
	int			tt_eat;
	int			n_philo;
	long long	time_start;
};

//Functions: Matrix
int				init_matrix(t_table *table);

//Functions: Matrix Utils
void			print_state(t_table *table, char *state);
void			do_sleep(t_table *table, t_lli time);
void			*set_dead(t_philo *philo, t_table *table);
long long int	get_time(void);
void			destroy_all(t_table *table);

//Functions: Checker
int				ft_check_args(int ac, char **args);

//Functions: Utils
t_lli			ft_atoll(char *str);
void			ft_putnbr_fd(long int num, int fd);
int				ft_free(void *f1, void *f2);
int				print_error(char *msg, int num, int ret);
int				ft_strlen(char *str);

#endif