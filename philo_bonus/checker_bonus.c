/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:58:47 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/04 15:47:14 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

static int	ft_isnum(char *input);
static int	ft_isint(char *input);
static int	ft_nbr_size(t_lli nbr, t_lli len);

int	ft_check_args(int ac, char **args)
{
	int	i;

	if (ac < 4 || ac > 5)
		return (print_error(ERR_ARGS_1, -1, 1));
	i = -1;
	while (++i < ac)
		if (!ft_isint(args[i]) || !ft_isnum(args[i]))
			return (print_error(ERR_ARGS_2, -1, 1));
	return (0);
}

static int	ft_isnum(char *input)
{
	int	i;

	i = 0;
	if (!input || !*input)
		return (0);
	if ((input[0] == '-' || input[0] == '+') && \
		(input[1] == '-' || input[1] == '+'))
		return (0);
	if (input[i] == '+' || input[i] == '-')
		i++;
	if (!input[i])
		return (0);
	while (input[i])
	{
		if (input[i] < '0' || input[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_isint(char *num)
{
	int			i;
	long long	tmp;

	i = 0;
	if (!num || !*num)
		return (0);
	if (num[i] == '-' || num[i] == '+')
		i++;
	while (num[i] && num[i] == '0')
		i++;
	if (!num[i])
		i--;
	if (ft_strlen(&num[i]) > ft_nbr_size(INT_MAX, 10))
		return (0);
	tmp = ft_atoll(num);
	if (tmp < INT_MIN || tmp > INT_MAX)
		return (0);
	return (1);
}

static int	ft_nbr_size(t_lli nbr, t_lli len)
{
	int	nbr_size;

	nbr_size = 0;
	if (nbr < 0)
		nbr = -nbr;
	if (nbr == 0)
		return (1);
	while (nbr > 0)
	{
		nbr_size++;
		nbr /= len;
	}
	return (nbr_size);
}
