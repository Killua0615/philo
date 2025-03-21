#include "philo.h"

long long	gettime_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long long)time.tv_sec * 1000 + (long long)time.tv_usec / 1000);
}

void	msleep(unsigned int ms)
{
	long long	start;

	start = gettime_ms();
	while ((gettime_ms() - start) < ms)
		usleep(10);
}

int	check_end(t_data *d)
{
	int	ret;

	pthread_mutex_lock(&d->mtx_end);
	ret = d->is_end;
	pthread_mutex_unlock(&d->mtx_end);
	return (ret);
}

void	set_end(t_data *d)
{
	pthread_mutex_lock(&d->mtx_end);
	d->is_end = 1;
	pthread_mutex_unlock(&d->mtx_end);
}

void	print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->mtx_print);
	if (check_end(philo->data) == 0)
	{
		printf("%-6lld %d %s\n", gettime_ms() - philo->data->ms_start,
			philo->id, str);
	}
	pthread_mutex_unlock(&philo->data->mtx_print);
}
