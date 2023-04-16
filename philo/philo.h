/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:45:10 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/16 19:18:04 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include	<stdlib.h>
# include	<stdio.h>
# include	<unistd.h>
# include	<pthread.h>
# include	<sys/time.h>

typedef struct s_table	t_table;
typedef struct s_philo	t_philo;

struct s_table {
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				any_dead;
};

struct s_philo
{
	pthread_t		id;
	int				alive;
	int				n;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	l_fork;
};

#endif