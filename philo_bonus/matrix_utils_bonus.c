/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:29:06 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/04 18:14:29 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

long long int	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_state(t_philo *philo, char *state)
{
	if (philo->table->any_dead)
		return ;
	pthread_mutex_lock(&philo->table->print);
	if (!philo->table->any_dead)
		printf("%lld %i %s\n", get_time() - \
				philo->table->time_start, philo->n, state);
	pthread_mutex_unlock(&philo->table->print);
}

void	do_sleep(long long time)
{
	time += get_time();
	while (get_time() <= time)
		usleep(200);
}

void	*set_dead(t_philo *philo, t_table *table)
{
	table->any_dead = 1;
	pthread_mutex_lock(&table->print);
	printf("%lld %i %s\n", get_time() - table->time_start, philo->n, DEAD);
	pthread_mutex_unlock(&table->print);
	return (NULL);
}

void	destroy_all(t_table *table)
{
	int	i;

	i = -1;
	ft_free(table->philos, table->forks);
	while (++i < table->n_philo)
		pthread_mutex_destroy(&table->forks[i]);
	pthread_mutex_destroy(&table->print);
	pthread_mutex_destroy(&table->init);
	pthread_mutex_destroy(&table->life_check);
}
