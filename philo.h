#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h> // memset などで使用する場合

/*
 * 構造体定義
 */
typedef struct s_data      t_data;
typedef struct s_philo     t_philo;

struct s_philo
{
    int                 id;
    int                 num_ate;
    long long           ms_ate;
    pthread_t           thread;
    pthread_mutex_t     mtx_ate;
    t_data              *data;
};

struct s_data
{
    int                 num_philos;
    int                 ms_die;
    int                 ms_eat;
    int                 ms_sleep;
    int                 num_eat;        // (オプション) 規定の食事回数
    long long           ms_start;       // 計測開始時刻
    int                 is_end;         // 終了フラグ
    pthread_mutex_t     *mtx_forks;     // フォーク用ミューテックスの配列
    pthread_mutex_t     mtx_print;      // 標準出力保護用
    pthread_mutex_t     mtx_end;        // 終了フラグ保護用
    t_philo             *philos;        // 哲学者情報の配列
};

/*
 * 関数プロトタイプ
 */

/* main.c */
void    usage(void);

/* init.c */
int     init_data(t_data *data, int argc, char **argv);
int     valid_data(int argc, t_data *data);
void    free_data(t_data *data);

/* threads.c */
int     init_thread(t_data *data);

/* monitor.c */
void    monitor(t_data *data);

/* philo.c */
void    *philo(void *arg);

/* utils.c */
long long   gettime_ms(void);
void        msleep(unsigned int ms);
int         check_end(t_data *d);
void        set_end(t_data *d);
void        print_status(t_philo *philo, char *str);

#endif
