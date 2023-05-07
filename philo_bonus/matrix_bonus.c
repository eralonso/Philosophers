/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 12:36:44 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/07 17:32:37 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

static void	routine_eating(t_table *table);
static void	child_proc(t_table *table, int n);
static void	rt_dead(t_table *table);
static void	rt_finish(t_table *table);

int	init_matrix(t_table *table)
{
	int	i;

	i = -1;
	pthread_create(&table->th_finish, NULL, (void *)rt_finish, table);
	pthread_create(&table->th_dead, NULL, (void *)rt_dead, table);
	table->time_start = get_time();
	while (++i < table->n_philo)
	{
		table->pid[i] = fork();
		if (table->pid[i] < 0)
			return (print_error(ERR_FORK, i, 0));
		if (table->pid[i] == 0)
			child_proc(table, i);
	}
	sem_post(table->finish);
	sem_post(table->dead);
	pthread_join(table->th_dead, NULL);
	pthread_join(table->th_finish, NULL);
	destroy_all(table);
	return (1);
}

static void	routine_eating(t_table *table)
{
	if (table->philo.times_eat && ((table->time.to_eat * 2 >= \
	table->time.to_die) || (table->time.to_eat * 3 >= table->time.to_die \
	&& table->n_philo % 2)))
		do_sleep(table, table->time.to_die + 1);
	sem_wait(table->life_check);
	sem_wait(table->forks);
	print_state(table, HTK);
	if (table->n_philo == 1)
		do_sleep(table, table->time.to_die + 1);
	sem_wait(table->forks);
	print_state(table, HTK);
	sem_post(table->life_check);
	table->philo.last_eat = get_time();
	print_state(table, ISE);
	do_sleep(table, table->time.to_eat);
	table->philo.times_eat++;
	sem_post(table->forks);
	sem_post(table->forks);
	if (table->philo.times_eat == table->tt_eat)
		sem_post(table->finish);
	while (table->philo.times_eat == table->tt_eat)
		;
}

static void	child_proc(t_table *table, int n)
{
	table->philo.n = n + 1;
	table->philo.last_eat = table->time_start;
	while (42)
	{
		routine_eating(table);
		print_state(table, ISS);
		do_sleep(table, table->time.to_sleep);
		print_state(table, IST);
	}
}

static void	rt_dead(t_table *table)
{
	int	i;

	sem_wait(table->dead);
	sem_wait(table->dead);
	if (table->end)
		return ;
	table->end = 1;
	i = -1;
	while (++i < table->n_philo)
	{
		sem_post(table->finish);
		kill(table->pid[i], SIGTERM);
	}
}

static void	rt_finish(t_table *table)
{
	int	i;

	sem_wait(table->finish);
	i = -1;
	while (++i < table->n_philo)
		sem_wait(table->finish);
	if (table->end)
		return ;
	table->end = 1;
	i = -1;
	while (++i < table->n_philo)
		kill(table->pid[i], SIGTERM);
	sem_post(table->dead);
}
