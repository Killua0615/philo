#include "philo.h"

/*
 * 現在時刻(ミリ秒)を取得
 */
long long gettime_ms(void)
{
  struct timeval  time;

  gettimeofday(&time, NULL);
  return ((long long)time.tv_sec * 1000 + (long long)time.tv_usec / 1000);
}

/*
 * ミリ秒単位でスリープ
 * usleep(10) のループで細かく制御
 */
void msleep(unsigned int ms)
{
  long long start;

  start = gettime_ms();
  while ((gettime_ms() - start) < ms)
    usleep(10);
}

/*
 * 終了フラグの読み取り
 */
int check_end(t_data *d)
{
  int ret;

  pthread_mutex_lock(&d->mtx_end);
  ret = d->is_end;
  pthread_mutex_unlock(&d->mtx_end);
  return (ret);
}

/*
 * 終了フラグのセット
 */
void set_end(t_data *d)
{
  pthread_mutex_lock(&d->mtx_end);
  d->is_end = 1;
  pthread_mutex_unlock(&d->mtx_end);
}

/*
 * ステータスメッセージを表示
 * -> 終了フラグが立っていない場合のみ
 * -> 出力競合を防ぐためロック
 */
void print_status(t_philo *philo, char *str)
{
  pthread_mutex_lock(&philo->data->mtx_print);
  if (check_end(philo->data) == 0)
  {
    printf("%-6lld %d %s\n",
      gettime_ms() - philo->data->ms_start,
      philo->id,
      str
    );
  }
  pthread_mutex_unlock(&philo->data->mtx_print);
}
