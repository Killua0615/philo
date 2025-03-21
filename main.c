#include "philo.h"

void	usage(void)
{
	printf("Usage: ./philo <num_of_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_of_must_eat]\n\n");
	printf("Arguments:\n");
	printf("  <num_of_philos>    Number of philosophers (1 <= num_of_philos <= 200)\n");
	printf("  <time_to_die>      Time in milliseconds after which philosopher dies if they don't eat\n");
	printf("  <time_to_eat>      Time in milliseconds each philosopher spends eating\n");
	printf("  <time_to_sleep>    Time in milliseconds each philosopher spends sleeping\n");
	printf("  [num_of_must_eat]  Optional: Number of times each philosopher must eat before the simulation ends\n");
	printf("                      If not provided,philosophers can eat any number of times\n");
}

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
	{
		usage();
		return (ft_error("Error: Invalid number of arguments."));
	}
	num_philos = atoi(argv[1]);
	time_to_die = atoi(argv[2]);
	time_to_eat = atoi(argv[3]);
	time_to_sleep = atoi(argv[4]);
	if (num_philos < 1 || num_philos > 200)
		return (ft_error("Error: num_of_philos must be between 1 and 200."));
	if (time_to_die < 60 || time_to_eat < 60 || time_to_sleep < 60)
		return (ft_error("Error: time_to_die,time_to_eat,time_to_sleep must be >= 60 ms."));
	if (argc == 6)
	{
		num_of_must_eat = atoi(argv[5]);
		if (num_of_must_eat < 0)
			return (ft_error("Error: num_of_must_eat must be a non-negative number."));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data data;

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