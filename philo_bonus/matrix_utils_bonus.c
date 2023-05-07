/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:29:06 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/07 17:21:27 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

long long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_state(t_table *table, char *state)
{
	sem_wait(table->print);
	printf("%lld %i %s\n", get_time() - \
			table->time_start, table->philo.n, state);
	sem_post(table->print);
}

void	do_sleep(t_table *table, t_lli t)
{
	t_lli	init;
	t_lli	tdie;

	init = get_time();
	tdie = (init - table->philo.last_eat) + t;
	if (tdie >= table->time.to_die)
	{
		tdie = table->time.to_die - (init - table->philo.last_eat);
		if (tdie > 0)
			usleep(tdie * 1000);
		set_dead(&table->philo, table);
		sem_post(table->dead);
		sem_wait(table->print);
	}
	usleep(t * 1000);
}

void	*set_dead(t_philo *philo, t_table *table)
{
	sem_wait(table->print);
	printf("%lld %i %s\n", get_time() - table->time_start, philo->n, DEAD);
	return (NULL);
}

void	destroy_all(t_table *table)
{
	ft_free(table->pid, NULL);
	sem_close(table->forks);
	sem_close(table->life_check);
	sem_close(table->print);
	sem_close(table->finish);
	sem_close(table->dead);
	sem_unlink("/forks");
	sem_unlink("/life_check");
	sem_unlink("/print");
	sem_unlink("/finish");
	sem_unlink("/dead");
}
