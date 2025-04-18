/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natsumi <natsumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:36 by nateshim          #+#    #+#             */
/*   Updated: 2025/04/18 14:33:10 by natsumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline int	monitor_dead(t_data *d)
{
	int			i;
	long long	ms_ate;

	i = 0;
	while (i < d->num_philos)
	{
		pthread_mutex_lock(&d->philos[i].mtx_ate);
		ms_ate = d->philos[i].ms_ate;
		pthread_mutex_unlock(&d->philos[i].mtx_ate);
		if (gettime_ms() - ms_ate > d->ms_die)
		{
			pthread_mutex_lock(&d->mtx_print);
			printf("%-6lld %d died\n", gettime_ms() - d->ms_start,
				d->philos[i].id);
			pthread_mutex_unlock(&d->mtx_print);
			set_end(d);
			return (1);
		}
		i++;
	}
	return (0);
}

static inline int	monitor_full(t_data *d)
{
	int	i;
	int	cnt_full;

	i = 0;
	cnt_full = 0;
	while (i < d->num_philos)
	{
		pthread_mutex_lock(&d->philos[i].mtx_ate);
		if (d->num_eat != -1 && d->philos[i].num_ate >= d->num_eat)
			cnt_full++;
		pthread_mutex_unlock(&d->philos[i].mtx_ate);
		i++;
	}
	if (d->num_eat != -1 && cnt_full >= d->num_philos)
	{
		set_end(d);
		return (1);
	}
	return (0);
}

void	monitor(t_data *d)
{
	while (check_end(d) == 0)
	{
		// 死亡チェック
		if (monitor_dead(d))
			break ;
		// 満腹チェック
		if (monitor_full(d))
			break ;
		usleep(10);
	}
}
