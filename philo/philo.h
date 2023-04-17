/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:45:10 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/17 19:41:05 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include	<stdlib.h>
# include	<stdio.h>
# include	<unistd.h>
# include	<pthread.h>
# include	<sys/time.h>

# define HTK	(char *)"has taken a fork"
# define ISE	(char *)"is eating"
# define ISS	(char *)"is sleeping"
# define IST	(char *)"is thinking"
# define DEAD	(char *)"died"

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
	pthread_mutex_t	print;
	pthread_mutex_t	init;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	t_times			time;
	int				any_dead;
	int				tt_eat;
	long long		time_start;
};

struct s_philo
{
	pthread_t		id;
	int				alive;
	int				n;
	int				times_eat;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	l_fork;
	t_table			*table;
};

#endif