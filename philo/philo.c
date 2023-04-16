/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:44:29 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/16 19:27:37 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo.h>

void	*rutine(void *table)
{
	t_table	*cast_table;
	
	cast_table = (t_table *)table;
	while (!cast_table->any_dead)
	{
		printf("a");
	}
	
}

int	main(int ac, char ** av)
{
	t_table	table;
	int		err;
	int		n_philo;
	int		i;

	(void)	ac;
	(void)	av;
	// if (ac <)
	if (ac != 2)
		return (1);
	n_philo = atoi(av[1]);
	if (!n_philo)
		return (1);
	table.any_dead = 0;
	table.philos = (t_philo *)malloc(sizeof(t_philo) * n_philo);
	if (!table.philos)
		return (1);
	table.forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n_philo);
	if (!table.forks)
	{
		free(table.philos);
		return (1);
	}
	if (pthread_mutex_init(&table.print, NULL))
	{
		free(table.philos);
		free(table.forks);
		return (1);
	}
	i = -1;
	while (++i < n_philo)
	{
		if (pthread_mutex_init(&table.forks[i], NULL))
		{
			free(table.philos);
			free(table.forks);
			pthread_mutex_destroy(&table.print);
			return (1);
		}
	}
	i = -1;
	while (++i < n_philo)
	{
		table.philos[i].n = i + 1;
		table.philos[i].alive = 1;
		table.philos[i].r_fork = table.forks[i];
		table.philos[i].l_fork = table.forks[(i + 1) % n_philo];
	}
	err = pthread_create(&table.philos[i].id, NULL, rutine, &table);
	if (err)
	{
		dprintf(2, "Error \'pthread_create()\'.\nThread %i", i);
		return (1);
	}
	return (0);
}