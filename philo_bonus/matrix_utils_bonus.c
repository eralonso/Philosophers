/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:29:06 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/05 12:56:24 by eralonso         ###   ########.fr       */
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
	sem_wait(philo->table->print);
	if (!philo->table->any_dead)
		printf("%lld %i %s\n", get_time() - \
				philo->table->time_start, philo->n, state);
	sem_post(philo->table->print);
}

void	do_sleep(long long time)
{
	time += get_time();
	while (get_time() <= time)
		usleep(210);
}

void	*set_dead(t_philo *philo, t_table *table)
{
	table->any_dead = 1;
	sem_wait(table->print);
	printf("%lld %i %s\n", get_time() - table->time_start, philo->n, DEAD);
	return (NULL);
}

void	destroy_all(t_table *table)
{
	ft_free(table->philos, NULL);
	sem_close(table->forks);
	sem_close(table->life_check);
	sem_close(table->print);
	sem_unlink("/forks");
	sem_unlink("/life_check");
	sem_unlink("/print");
}
