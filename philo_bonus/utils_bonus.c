/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralonso <eralonso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 19:19:01 by eralonso          #+#    #+#             */
/*   Updated: 2023/05/04 15:47:37 by eralonso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<philo_bonus.h>

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

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_free(void *f1, void *f2)
{
	free(f1);
	free(f2);
	return (0);
}

void	ft_putnbr_fd(long int num, int fd)
{
	if (num > 9)
	{
		ft_putnbr_fd(num / 10, fd);
		ft_putnbr_fd(num % 10, fd);
	}
	else
	{
		num += '0';
		write(fd, &num, 1);
	}
}

int	print_error(char *msg, int num, int ret)
{
	int	i;

	i = ft_strlen(msg);
	write(2, msg, i);
	if (num >= 0)
		ft_putnbr_fd(num, 2);
	write(2, "\n", 1);
	return (ret);
}
