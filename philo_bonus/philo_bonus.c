/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 12:44:29 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/08 11:52:08 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

static int	init_sem(t_table *table, int n_philo);
static int	init_table(t_table *table, char **av, int ac);

int	main(int ac, char **av)
{
	t_table	table;

	if (ft_check_args(ac - 1, &av[1]))
		return (1);
	if (init_table(&table, &av[1], ac - 1) == 0)
		return (1);
	if (init_matrix(&table) == 0)
		return (1);
	return (0);
}

static int	init_sem(t_table *table, int n_philo)
{
	sem_unlink("/forks");
	sem_unlink("/life_check");
	sem_unlink("/print");
	sem_unlink("/finish");
	sem_unlink("/dead");
	table->forks = sem_open("/forks", O_CREAT, 600, n_philo);
	if (table->forks == SEM_FAILED)
		return (print_error(ERR_SEM, -1, 0));
	table->life_check = sem_open("/life_check", O_CREAT, 600, 1);
	if (table->life_check == SEM_FAILED)
		return ((sem_unlink("/forks") || print_error(ERR_SEM, -1, 0)) && 0);
	table->print = sem_open("/print", O_CREAT, 600, 1);
	if (table->print == SEM_FAILED)
		return ((sem_unlink("/forks") || sem_unlink("/life_check") \
		|| print_error(ERR_SEM, -1, 0)) && 0);
	table->finish = sem_open("/finish", O_CREAT, 600, 0);
	if (table->finish == SEM_FAILED)
		return ((sem_unlink("/forks") || sem_unlink("/life_check") \
		|| sem_unlink("/print") || print_error(ERR_SEM, -1, 0)) && 0);
	table->dead = sem_open("/dead", O_CREAT, 600, 0);
	if (table->dead == SEM_FAILED)
		return ((sem_unlink("/forks") || sem_unlink("/life_check") \
		|| sem_unlink("/print") || sem_unlink("/finish") || \
		print_error(ERR_SEM, -1, 0)) && 0);
	return (1);
}

static int	init_table(t_table *table, char **av, int ac)
{
	table->n_philo = ft_atoll(av[0]);
	table->time.to_die = ft_atoll(av[1]);
	table->time.to_eat = ft_atoll(av[2]);
	table->time.to_sleep = ft_atoll(av[3]);
	table->tt_eat = -1;
	if (ac == 5)
		table->tt_eat = ft_atoll(av[4]);
	if (!table->n_philo || !table->tt_eat || (ac == 5 && table->tt_eat < 0))
		return (0);
	table->pid = (pid_t *)malloc(sizeof(pid_t) * table->n_philo);
	if (!table->pid)
		return (0);
	if (init_sem(table, table->n_philo) == 0)
		return (ft_free(table->pid, NULL));
	table->end = 0;
	table->philo.times_eat = 0;
	return (1);
}
