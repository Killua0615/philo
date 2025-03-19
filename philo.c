#include "philo.h"

/*
 * フォークのインデックスを計算して取得するヘルパー関数
 */
static inline pthread_mutex_t *pick_fork(int id_fork, t_philo *philo)
{
  int num_philos = philo->data->num_philos;
  pthread_mutex_t *mtx_forks = philo->data->mtx_forks;

  // id_fork が 1 なら「右のフォーク」、2 なら「左のフォーク」(あるいは逆)という実装
  // フォークを取る順番を工夫して、デッドロックを回避している
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

/*
 * 哲学者が1人しかいないケース
 * → フォークが1本しかないため、ずっと食事できない
 */
static inline void philo_one(t_philo *philo)
{
  while (check_end(philo->data) == 0)
  {
    print_status(philo, "is thinking");
    pthread_mutex_lock(pick_fork(1, philo));
    print_status(philo, "has taken a fork");
    // 食べるためのフォークがもう1本ないので、
    // 終了フラグが立つまで待機
    while (check_end(philo->data) == 0)
      msleep(1);
    pthread_mutex_unlock(pick_fork(1, philo));
  }
}

/*
 * スレッド起動直後に少し休むなどの調整
 * (奇数/偶数で時間差をつけることで、ある程度デッドロック回避)
 */
static inline void philosleep(t_philo *philo)
{
  int eat = philo->data->ms_eat;
  int num = philo->data->num_philos;
  int id  = philo->id;

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

/*
 * 哲学者が2人以上いる場合のメインループ
 */
static inline void philo_some(t_philo *philo)
{
  // 開始直後、動作を少しだけずらす
  philosleep(philo);

  while (check_end(philo->data) == 0)
  {
    print_status(philo, "is thinking");
    // フォーク1本目
    pthread_mutex_lock(pick_fork(1, philo));
    print_status(philo, "has taken a fork");
    // フォーク2本目
    pthread_mutex_lock(pick_fork(2, philo));
    print_status(philo, "has taken a fork");
    // 食事開始
    pthread_mutex_lock(&philo->mtx_ate);
    philo->ms_ate = gettime_ms();
    pthread_mutex_unlock(&philo->mtx_ate);
    print_status(philo, "is eating");
    msleep(philo->data->ms_eat);
    // 食べた回数をインクリメント
    pthread_mutex_lock(&philo->mtx_ate);
    philo->num_ate++;
    pthread_mutex_unlock(&philo->mtx_ate);
    // フォークを2本とも解放
    pthread_mutex_unlock(pick_fork(2, philo));
    pthread_mutex_unlock(pick_fork(1, philo));
    // 食後は眠る
    print_status(philo, "is sleeping");
    msleep(philo->data->ms_sleep);
    // フォーク取得の競合をある程度調整するため、少しだけ待機
    msleep(1);
  }
}

/*
 * スレッドが実行する関数
 */
void *philo(void *arg)
{
  t_philo *p = (t_philo *)arg;
  long long now;

  // スタート時刻になるまで待機 (同期用)
  now = gettime_ms();
  while (now < p->data->ms_start)
  {
    usleep(10);
    now = gettime_ms();
  }
  // 哲学者が1人しかいないときの特別処理
  if (p->data->num_philos == 1)
    philo_one(p);
  else
    philo_some(p);
  return (NULL);
}
