/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 12:36:44 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/05 12:56:52 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

static int	routine_eating(t_philo *philo);
static void	*check_eat(t_philo *philo);
static void	child_proc(t_philo *philo, t_table *table);
static void	main_loop(t_table *table);

int	init_matrix(t_table *table)
{
	int	i;

	i = -1;
	table->time_start = get_time();
	while (++i < table->n_philo)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid < 0)
			return (print_error(ERR_FORK, i, 0));
		if (table->philos[i].pid == 0)
			child_proc(&table->philos[i], table);
		usleep(50);
	}
	main_loop(table);
	destroy_all(table);
	return (1);
}

static int	routine_eating(t_philo *philo)
{
	sem_wait(philo->table->forks);
	print_state(philo, HTK);
	if (philo->table->n_philo == 1)
		return (0);
	sem_wait(philo->table->forks);
	print_state(philo, HTK);
	sem_wait(philo->table->life_check);
	print_state(philo, ISE);
	philo->times_eat++;
	philo->last_eat = get_time();
	sem_post(philo->table->life_check);
	do_sleep(philo->table->time.to_eat);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	if (philo->table->tt_eat != -1 && \
	philo->times_eat == philo->table->tt_eat)
		return (0);
	return (1);
}

static void	*check_eat(t_philo *philo)
{
	while (42)
	{
		if (philo->times_eat == philo->table->tt_eat)
			break ;
		sem_wait(philo->table->life_check);
		if (get_time() - philo->last_eat >= philo->table->time.to_die)
		{
			set_dead(philo, philo->table);
			exit(1);
		}
		sem_post(philo->table->life_check);
		// do_sleep(500);
		// do_sleep(philo->table->time.to_die);
		usleep(1000);
	}
	return (NULL);
}

static void	child_proc(t_philo *philo, t_table *table)
{
	philo->last_eat = table->time_start;
	if (pthread_create(&philo->id, NULL, (void *)check_eat, philo))
		exit (print_error(ERR_PTHREAD, philo->n, 1));
	while (!(table->tt_eat != -1 && philo->times_eat == table->tt_eat))
	{
		print_state(philo, IST);
		if (routine_eating(philo) == 0)
			break ;
		print_state(philo, ISS);
		do_sleep(philo->table->time.to_sleep);
	}
	exit(0);
}

static void	main_loop(t_table *table)
{
	int	i;
	int	status;

	i = -1;
	while (++i < table->n_philo)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			i = -1;
			while (++i < table->n_philo)
				kill(table->philos[i].pid, SIGTERM);
			break ;
		}
	}
}
