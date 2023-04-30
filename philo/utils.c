/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 19:19:01 by eralonso          #+#    #+#             */
/*   Updated: 2023/04/30 19:24:44 by eralonso         ###   ########.fr       */
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

int	ft_atoi(char *str)
{
	int	res;
	int	s;
	int	i;

	i = 0;
	s = 1;
	res = 0;
	while (str[i] == '\n' || str[i] == '\t' || str[i] == 32 || \
		str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-')
		s = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		i++;
	}
	return (res * s);
}
