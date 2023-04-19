/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:44:29 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/19 19:47:10 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo.h>

long long int	get_time()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static int	init_table_philos(t_table *table, int n_philo)
{
	int		i;

	table->any_dead = 0;
	table->n_philo = n_philo;
	table->time.to_eat = 1000;
	table->time.to_sleep = 1000;
	table->time.to_die = 1000;
	table->philos = (t_philo *)malloc(sizeof(t_philo) * n_philo);
	if (!table->philos)
		return (0);
	table->forks = (t_mutex *)malloc(sizeof(t_mutex) * n_philo);
	if (!table->forks)
	{
		free(table->philos);
		return (0);
	}
	if (pthread_mutex_init(&table->print, NULL))
	{
		free(table->philos);
		free(table->forks);
		return (0);
	}
	if (pthread_mutex_init(&table->life_check, NULL))
	{
		free(table->philos);
		free(table->forks);
		pthread_mutex_destroy(&table->print);
		return (0);
	}
	i = -1;
	while (++i < n_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
		{
			free(table->philos);
			free(table->forks);
			pthread_mutex_destroy(&table->print);
			pthread_mutex_destroy(&table->life_check);
			return (0);
		}
	}
	i = -1;
	while (++i < n_philo)
	{
		table->philos[i].n = i + 1;
		table->philos[i].alive = 1;
		table->philos[i].r_fork = &table->forks[i];
		table->philos[i].l_fork = &table->forks[((i + 1) % n_philo)];
		table->philos[i].table = table;
		table->philos[i].times_eat = 0;
		table->philos[i].last_eat = get_time();
	}
	return (1);
}

void	print_state(t_philo *philo, char *state)
{
	pthread_mutex_lock(&philo->table->print);
	printf("%lld %i %s\n", get_time() - philo->table->time_start, philo->n, state);
	pthread_mutex_unlock(&philo->table->print);
}

void	do_sleep(long long time)
{

	time += get_time();
	while (get_time() < time)
		usleep(200);
}

void	*set_dead(t_philo *philo, t_table *table)
{
	table->any_dead = 1;
	print_state(philo, DEAD);
	return (NULL);
}

void	*routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->init);
	pthread_mutex_unlock(&philo->table->init);
	if (!(philo->n % 2))
		do_sleep(1000);
	while (!philo->table->any_dead)
	{
		pthread_mutex_lock(philo->r_fork);
		print_state(philo, HTK);
		pthread_mutex_lock(philo->l_fork);
		print_state(philo, HTK);
		pthread_mutex_lock(&philo->table->life_check);
		// if (philo->table->any_dead)
			// break ;
		print_state(philo, ISE);
		do_sleep(philo->table->time.to_eat);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->table->life_check);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		philo->times_eat++;
		print_state(philo, ISS);
		do_sleep(philo->table->time.to_sleep);
		print_state(philo, IST);
	}
	return (NULL);
}

int	main(int ac, char ** av)
{
	t_table	table;
	int		err;
	int		n_philo;
	int		i;

	(void)	ac;
	(void)	av;
	if (ac != 2)
		return (1);
	n_philo = atoi(av[1]);
	if (!n_philo)
		return (1);
	if (!init_table_philos(&table, n_philo))
		return (1);
	pthread_mutex_lock(&table.init);
	i = -1;
	while (++i < n_philo)
	{
		err = pthread_create(&table.philos[i].id, NULL, (void *)routine, &table.philos[i]);
		if (err)
		{
			dprintf(2, "Error \'pthread_create()\'.\nThread %i", i);
			return (1);
		}
	}
	pthread_mutex_unlock(&table.init);
	table.time_start = get_time();
	i = 0;
	while (!table.any_dead)
	{
		pthread_mutex_lock(&table.life_check);
		if (get_time() - table.philos[i].last_eat >= table.time.to_die)
		{
			set_dead(&table.philos[i], &table);
			return (0);
		}
		pthread_mutex_unlock(&table.life_check);
		i++;
		if (i == n_philo)
			i = 0;
	}
	i = -1;
	while (++i < n_philo)
		if (pthread_join(table.philos[i].id, NULL))
			return (1);
	return (0);
}