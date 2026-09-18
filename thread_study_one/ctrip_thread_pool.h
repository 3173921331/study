/**
 *  线程池工具, ctrip_thread_pool.h
 *  zhangyl 2018.03.23
 */

#ifndef __CTRIP_THREAD_POOL_H__
#define __CTRIP_THREAD_POOL_H__

#include <pthread.h>

#ifndef NULL
#define NULL 0
#endif

#define PUBLIC

PUBLIC struct ctrip_task
{
    struct ctrip_task *pNext;
    int value;
};

// 线程池管理结构体，保存线程池全部状态信息
struct ctrip_thread_info
{
    int thread_running;       // 线程池运行标志：1运行，0停止，用来通知工作线程退出
    int thread_num;           // 线程池内工作线程总数量
    int tasknum;              // 当前任务队列中待执行的任务个数
    struct ctrip_task *tasks; // 任务链表的头结点，存放等待执行的任务单向链表
    pthread_t *threadid;      // 动态数组，存放所有工作线程的tid
    pthread_mutex_t mutex;    // 互斥锁，保护任务队列的并发访问（增删任务、计数修改）
    pthread_cond_t cond;      // 条件变量：任务队列为空时，工作线程阻塞等待新任务
};

/* 初始化线程池线程数目
 * @param thread_num 线程数目, 默认为8个
 */
PUBLIC void ctrip_init_thread_pool(int thread_num);

/* 销毁线程池
 */
PUBLIC void ctrip_destroy_thread_pool();

/**向任务池中增加一个任务
 * @param t 需要增加的任务
 */
PUBLIC void ctrip_thread_pool_add_task(struct ctrip_task *t);

/**从任务池中取出一个任务
 * @return 返回得到的任务
 */
struct ctrip_task *ctrip_thread_pool_retrieve_task();

/**执行任务池中的任务
 * @param t 需要执行的任务
 */
PUBLIC void ctrip_thread_pool_do_task(struct ctrip_task *t);

/**线程函数
 * @param thread_param 线程参数
 */
void *ctrip_thread_routine(void *thread_param);

#endif //!__CTRIP_THREAD_POOL_H__