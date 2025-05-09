/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nateshim <nateshim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:42 by nateshim          #+#    #+#             */
/*   Updated: 2025/04/18 15:47:07 by nateshim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline pthread_mutex_t	*pick_fork(int id_fork, t_philo *philo)
{
	int				num_philos;
	pthread_mutex_t	*mtx_forks;

	num_philos = philo->data->num_philos;
	mtx_forks = philo->data->mtx_forks;
	if (id_fork == 1)
	{
		if (philo->id % 2 == 0)
			return (&mtx_forks[philo->id % num_philos]);
		else
			return (&mtx_forks[philo->id - 1]);
	}
	else
	{
		if (philo->id % 2 == 0)
			return (&mtx_forks[philo->id - 1]);
		else
			return (&mtx_forks[philo->id % num_philos]);
	}
}

static inline void	philo_one(t_philo *philo)
{
	while (check_end(philo->data) == 0)
	{
		print_status(philo, "is thinking");
		pthread_mutex_lock(pick_fork(1, philo));
		print_status(philo, "has taken a fork");
		while (check_end(philo->data) == 0)
			msleep(1);
		pthread_mutex_unlock(pick_fork(1, philo));
	}
}

static inline void	philosleep(t_philo *philo)
{
	int	eat;
	int	num;
	int	id;

	eat = philo->data->ms_eat;
	num = philo->data->num_philos;
	id = philo->id;
	if (num % 2 == 0)
		msleep(eat * ((id - 1) % 2));
	else
	{
		if (id % 2 == 1)
			msleep(eat * ((2 * num - 1 - id) / (num - 1)));
		else
			msleep(eat * ((num - 1 - id) / (num - 1)));
	}
}

static inline void	philo_some(t_philo *philo)
{
	philosleep(philo);
	while (check_end(philo->data) == 0)
	{
		print_status(philo, "is thinking");
		pthread_mutex_lock(pick_fork(1, philo));
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(pick_fork(2, philo));
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->mtx_ate);
		philo->ms_ate = gettime_ms();
		pthread_mutex_unlock(&philo->mtx_ate);
		print_status(philo, "is eating");
		msleep(philo->data->ms_eat);
		if (!increment_and_check_eat_count(philo))
		{
			pthread_mutex_unlock(pick_fork(2, philo));
			pthread_mutex_unlock(pick_fork(1, philo));
			break ;
		}
		pthread_mutex_unlock(pick_fork(2, philo));
		pthread_mutex_unlock(pick_fork(1, philo));
		print_status(philo, "is sleeping");
		msleep(philo->data->ms_sleep);
		msleep(1);
	}
}

void	*philo(void *arg)
{
	t_philo		*p;
	long long	now;

	p = (t_philo *)arg;
	now = gettime_ms();
	while (now < p->data->ms_start)
	{
		usleep(10);
		now = gettime_ms();
	}
	if (p->data->num_philos == 1)
		philo_one(p);
	else
		philo_some(p);
	return (NULL);
}
