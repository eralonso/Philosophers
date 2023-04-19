/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:45:10 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/19 12:40:42 by eralonso         ###   ########.fr       */
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

//Synonymous
typedef pthread_mutex_t	t_mutex;

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

#endif