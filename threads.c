/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nateshim <nateshim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:57 by nateshim          #+#    #+#             */
/*   Updated: 2025/04/18 15:48:07 by nateshim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philosophers(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->num_philos)
	{
		d->philos[i].id = i + 1;
		d->philos[i].ms_ate = d->ms_start;
		d->philos[i].data = d;
		pthread_mutex_init(&d->philos[i].mtx_ate, NULL);
		if (pthread_create(&d->philos[i].thread, NULL, philo, &d->philos[i]))
		{
			printf("Error: pthread_create failed\n");
			pthread_mutex_destroy(&d->philos[i].mtx_ate);
			set_end(d);
			return (i);
		}
		i++;
	}
	return (i);
}

void	monitor_and_cleanup(t_data *d, int created_philos)
{
	int	i;

	monitor(d);
	i = 0;
	while (i < created_philos)
	{
		pthread_join(d->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < created_philos)
	{
		pthread_mutex_destroy(&d->philos[i].mtx_ate);
		i++;
	}
}

int	init_thread(t_data *d)
{
	int	created_philos;

	created_philos = create_philosophers(d);
	if (created_philos > 0)
		monitor_and_cleanup(d, created_philos);
	return (0);
}

int	increment_and_check_eat_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->mtx_ate);
	philo->num_ate++;
	if (philo->data->num_eat != -1 && philo->num_ate >= philo->data->num_eat)
	{
		pthread_mutex_unlock(&philo->mtx_ate);
		return (0);
	}
	pthread_mutex_unlock(&philo->mtx_ate);
	return (1);
}
