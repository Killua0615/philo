#include "philo.h"

void usage(void)
{
    printf("Usage: ./philo <num_of_philos> <time_to_die> <time_to_eat> "
           "<time_to_sleep> [num_of_must_eat]\n\n");
    printf("num_of_philos       number of philosophers and number of forks\n");
    printf("time_to_die [ms]    time in ms to die\n");
    printf("time_to_eat [ms]    time in ms to eat\n");
    printf("time_to_sleep [ms]  time in ms to sleep\n");
    printf("num_of_must_eat     (optional) number of times each philosopher must eat\n");
}

int main(int argc, char **argv)
{
  t_data  data;

  if (argc < 5 || argc > 6)
  {
    usage();
    return (1);
  }
  // データ初期化
  if (init_data(&data, argc, argv))
  {
    free_data(&data);
    return (1);
  }
  // バリデーション
  if (valid_data(argc, &data))
  {
    usage();
    free_data(&data);
    return (1);
  }
  // スレッド作成＆モニター
  if (init_thread(&data))
  {
    free_data(&data);
    return (1);
  }
  // 後始末
  free_data(&data);
  return (0);
}
