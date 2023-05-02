/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 19:19:01 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/02 19:21:30 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo.h>

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
		usleep(100);
}

void	*set_dead(t_philo *philo, t_table *table)
{
	table->any_dead = 1;
	pthread_mutex_lock(&table->print);
	printf("%lld %i %s\n", get_time() - table->time_start, philo->n, DEAD);
	pthread_mutex_unlock(&table->print);
	return (NULL);
}

long long	ft_atoll(char *str)
{
	long long	neg;
	long long	res;

	neg = 1;
	res = 0;
	if (!str || !*str)
		return (0);
	while (*str && (*str == '\t' || *str == ' ' || *str == '\n'))
		str++;
	if (*str && *str == '-')
		neg = -1;
	if (*str && (*str == '+' || *str == '-'))
		str++;
	while (*str && (*str >= '0' && *str <= '9'))
	{
		res = (*str - '0') + (res * 10);
		str++;
	}
	return (res * neg);
}
