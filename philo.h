/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natsumi <natsumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:53 by nateshim          #+#    #+#             */
/*   Updated: 2025/04/16 15:41:57 by natsumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

struct					s_philo
{
	int					id;
	int					num_ate;
	long long			ms_ate;
	pthread_t			thread;
	pthread_mutex_t		mtx_ate;
	t_data				*data;
};

struct					s_data
{
	int					num_philos;
	int					ms_die;
	int					ms_eat;
	int					ms_sleep;
	int					num_eat;
	long long			ms_start;
	int					is_end;
	pthread_mutex_t		*mtx_forks;
	pthread_mutex_t		mtx_print;
	pthread_mutex_t		mtx_end;
	t_philo				*philos;
};

// main.c
void					usage(void);
int						ft_error(char *message);

// init.c
int						init_data(t_data *data, int argc, char **argv);
int						valid_data(int argc, t_data *data);
void					free_data(t_data *data);

// threads.c
int						init_thread(t_data *data);

// monitor.c
void					monitor(t_data *data);

// philo.c
void					*philo(void *arg);

// utils.c
long long				gettime_ms(void);
void					msleep(unsigned int ms);
int						check_end(t_data *d);
void					set_end(t_data *d);
void					print_status(t_philo *philo, char *str);

#endif
