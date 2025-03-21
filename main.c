/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nateshim <nateshim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:57:10 by nateshim          #+#    #+#             */
/*   Updated: 2025/03/21 17:01:03 by nateshim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *message)
{
	printf("%s\n", message);
	return (1);
}

int	validate_args(int argc, char **argv)
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	num_of_must_eat;

	if (argc < 5 || argc > 6)
		return (ft_error("Error: Invalid number of arguments."));
	num_philos = atoi(argv[1]);
	time_to_die = atoi(argv[2]);
	time_to_eat = atoi(argv[3]);
	time_to_sleep = atoi(argv[4]);
	if (num_philos < 1 || num_philos > 200)
		return (ft_error("Error: num_of_philos must be between 1 and 200."));
	if (time_to_die < 60 || time_to_eat < 60 || time_to_sleep < 60)
		return (ft_error("Error: Invalid number of arguments."));
	if (argc == 6)
	{
		num_of_must_eat = atoi(argv[5]);
		if (num_of_must_eat < 0)
			return (ft_error("Error: Invalid number of arguments."));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (validate_args(argc, argv))
		return (1);
	data.num_philos = atoi(argv[1]);
	data.ms_die = atoi(argv[2]);
	data.ms_eat = atoi(argv[3]);
	data.ms_sleep = atoi(argv[4]);
	if (argc == 6)
		data.num_eat = atoi(argv[5]);
	else
		data.num_eat = -1;
	if (init_data(&data, argc, argv))
	{
		free_data(&data);
		return (1);
	}
	if (init_thread(&data))
	{
		free_data(&data);
		return (1);
	}
	free_data(&data);
	return (0);
}
