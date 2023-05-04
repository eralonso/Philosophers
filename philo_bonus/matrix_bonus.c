/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 12:36:44 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/04 18:57:56 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

static int	routine_eating(t_philo *philo);
static void	*routine(t_philo *philo);
static void	child_proc(t_philo *philo, t_table *table);
static void	main_loop(t_table *table);

int	init_matrix(t_table *table)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&table->init);
	table->time_start = get_time();
	while (++i < table->n_philo)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid < 0)
			return (print_error(ERR_FORK, i, 0));
		if (table->philos[i].pid == 0)
			child_proc(&table->philos[i], table);
	}
	pthread_mutex_unlock(&table->init);
	i = -1;
	while (++i < table->n_philo)
		table->philos[i].last_eat = table->time_start;
	main_loop(table);
	destroy_all(table);
	return (1);
}

static int	routine_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	print_state(philo, HTK);
	if (philo->r_fork == philo->l_fork)
	{
		pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	pthread_mutex_lock(philo->l_fork);
	print_state(philo, HTK);
	pthread_mutex_lock(&philo->table->life_check);
	print_state(philo, ISE);
	philo->times_eat++;
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->table->life_check);
	do_sleep(philo->table->time.to_eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	if (philo->table->tt_eat != -1 && \
	philo->times_eat == philo->table->tt_eat)
		return (0);
	return (1);
}

static void	*routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->init);
	pthread_mutex_unlock(&philo->table->init);
	if (!(philo->n % 2))
		do_sleep(philo->table->time.to_eat);
	while (!philo->table->any_dead && !(philo->table->tt_eat != -1 \
		&& philo->times_eat == philo->table->tt_eat))
	{
		print_state(philo, IST);
		if (routine_eating(philo) == 0)
			break ;
		print_state(philo, ISS);
		do_sleep(philo->table->time.to_sleep);
	}
	return (NULL);
}

static void	child_proc(t_philo *philo, t_table *table)
{
	if (pthread_create(&philo->id, NULL, \
	(void *)routine, philo))
		exit (print_error(ERR_PTHREAD, philo->n, 1));
	while (!table->any_dead)
	{
		if (philo->times_eat == table->tt_eat)
			break ;
		if (get_time() - philo->last_eat >= table->time.to_die)
			set_dead(philo, table);
	}
	exit (0);
}

static check_dead()
{
	
}

static void	main_loop(t_table *table)
{
	int	i;
	int	t_eats;

	i = table->n_philo;
	while (!table->any_dead)
	{
		if (i == table->n_philo)
		{
			i = 0;
			t_eats = 0;
		}
		if (table->philos[i].times_eat == table->tt_eat)
			t_eats++;
		if (t_eats == table->n_philo || !table->tt_eat)
			break ;
		pthread_mutex_lock(&table->life_check);
		if (get_time() - table->philos[i].last_eat >= table->time.to_die)
			set_dead(&table->philos[i], table);
		pthread_mutex_unlock(&table->life_check);
		i++;
	}
}
