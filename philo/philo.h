/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:45:10 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/02 19:27:53 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include	<stdlib.h>
# include	<limits.h>
# include	<stdio.h>
# include	<unistd.h>
# include	<pthread.h>
# include	<sys/time.h>

//Philo logs
# define HTK	(char *)"has taken a fork"
# define ISE	(char *)"is eating"
# define ISS	(char *)"is sleeping"
# define IST	(char *)"is thinking"
# define DEAD	(char *)"died"

//Errors messages
# define ERR_ARGS_1		(char *)"\033[7;97m ❌ \033[0m Error: \
\'Invalid number of arguments\': ./philo number_of_philosophers time_to_die \
time_to_eat time_to_sleep <number_of_times_each_philosopher_must_eat>"
# define ERR_ARGS_2		(char *)"\033[7;97m ❌ \033[0m Error: \
\'Invalid arguments\': Arguments must be an integers"
# define ERR_PTHREAD	(char *)"\033[7;97m ❌ \033[0m Error: \
\'pthread_create()\': Thread "

//Synonymous
typedef pthread_mutex_t	t_mutex;
typedef long long int	t_lli;

//Struct
typedef struct s_table	t_table;
typedef struct s_philo	t_philo;
typedef struct s_times	t_times;

struct s_times
{
	int	to_die;
	int	to_eat;
	int	to_sleep;
};

struct s_table {
	t_mutex		print;
	t_mutex		init;
	t_mutex		life_check;
	t_mutex		*forks;
	t_philo		*philos;
	t_times		time;
	int			any_dead;
	int			tt_eat;
	int			n_philo;
	long long	time_start;
	char		**av;
};

struct s_philo
{
	pthread_t	id;
	int			alive;
	int			n;
	int			times_eat;
	t_mutex		*r_fork;
	t_mutex		*l_fork;
	long long	last_eat;
	t_table		*table;
};

void			print_state(t_philo *philo, char *state);
void			do_sleep(long long time);
void			*set_dead(t_philo *philo, t_table *table);
long long int	get_time(void);
t_lli			ft_atoll(char *str);
int				print_error(char *msg, int num, int ret);
int				ft_check_args(int ac, char **args);

#endif