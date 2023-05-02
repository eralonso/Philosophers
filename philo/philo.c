/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:44:29 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/02 12:58:56 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo.h>


static int	init_table_philos(t_table *table, int n_philo, char **av, int ac)
{
	int		i;

	table->any_dead = 0;
	table->n_philo = n_philo;
	table->time.to_die = ft_atoi(av[1]);
	table->time.to_eat = ft_atoi(av[2]);
	table->time.to_sleep = ft_atoi(av[3]);
	table->tt_eat = -1;
	if (ac == 5)
		table->tt_eat = ft_atoi(av[4]);
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
	if (pthread_mutex_init(&table->init, NULL))
	{
		free(table->philos);
		free(table->forks);
		pthread_mutex_destroy(&table->print);
		return (0);
	}
	if (pthread_mutex_init(&table->life_check, NULL))
	{
		free(table->philos);
		free(table->forks);
		pthread_mutex_destroy(&table->print);
		pthread_mutex_destroy(&table->init);
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
	}
	return (1);
}

void	*routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->init);
	pthread_mutex_unlock(&philo->table->init);
	if (!(philo->n % 2))
		do_sleep(philo->table->time.to_eat);
	while (!philo->table->any_dead && !(philo->table->tt_eat != -1 \
		&& philo->times_eat == philo->table->tt_eat))
	{
		print_state(philo, IST);
		pthread_mutex_lock(philo->r_fork);
		print_state(philo, HTK);
		if (philo->r_fork == philo->l_fork)
		{
			pthread_mutex_unlock(philo->r_fork);
			break ;
		}
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(&philo->table->life_check);
		print_state(philo, HTK);
		print_state(philo, ISE);
		philo->times_eat++;
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->table->life_check);
		do_sleep(philo->table->time.to_eat);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (philo->table->tt_eat != -1 && \
			philo->times_eat == philo->table->tt_eat)
			break ;
		print_state(philo, ISS);
		do_sleep(philo->table->time.to_sleep);
	}
	return (NULL);
}

int	main(int ac, char ** av)
{
	t_table	table;
	int		err;
	int		n_philo;
	int		i;
	int		t_eats;

	if (ac < 5 || ac > 6)
		return (1);
	n_philo = ft_atoi(av[1]);
	if (!n_philo)
		return (1);
	if (!init_table_philos(&table, n_philo, &av[1], ac - 1))
		return (1);
	pthread_mutex_lock(&table.init);
	i = -1;
	table.time_start = get_time();
	while (++i < n_philo)
	{
		table.philos[i].last_eat = get_time();
		err = pthread_create(&table.philos[i].id, NULL, (void *)routine, &table.philos[i]);
		if (err)
		{
			dprintf(2, "Error \'pthread_create()\'.\nThread %i", i);
			return (1);
		}
	}
	pthread_mutex_unlock(&table.init);
	i = -1;
	while (++i < table.n_philo)
		table.philos[i].last_eat = table.time_start;
	while (!table.any_dead)
	{
		if (i == n_philo)
		{
			i = 0;
			t_eats = 0;
		}
		if (table.philos[i].times_eat == table.tt_eat)
			t_eats++;
		if (t_eats == table.n_philo || !table.tt_eat)
			break ;
		pthread_mutex_lock(&table.life_check);
		if (get_time() - table.philos[i].last_eat >= table.time.to_die)
			set_dead(&table.philos[i], &table);
		pthread_mutex_unlock(&table.life_check);
		i++;
	}
	i = -1;
	while (++i < table.n_philo)
		pthread_mutex_destroy(&table.forks[i]);
	pthread_mutex_destroy(&table.print);
	pthread_mutex_destroy(&table.init);
	pthread_mutex_destroy(&table.life_check);		
	i = -1;
	while (++i < table.n_philo)
		pthread_join(table.philos[i].id, NULL);
	return (0);
}