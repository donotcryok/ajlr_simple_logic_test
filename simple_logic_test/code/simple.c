#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/sched/clock.h>
#include <linux/sched/deadline.h>
#include <linux/sched/idle.h>
#include <linux/sched/sysctl.h>
#include <linux/sched/topology.h>
#include <linux/sched/nohz.h>
#include <linux/sched/stat.h>
#include <linux/sched/rt.h>
#include <linux/sched/fair.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

// 简单调度器的 pick_next_task 函数
static struct task_struct *pick_next_task_simple(struct rq *rq) {
    struct task_struct *p;

    // 遍历就绪队列，优先选择空闲进程
    for_each_process(p) {
        if (p->state == TASK_INTERRUPTIBLE || p->state == TASK_UNINTERRUPTIBLE)
            continue;
        if (p->state == TASK_IDLE) {
            printk(KERN_INFO "Simple Scheduler: Picking idle task %d\n", p->pid);
            return p;
        }
    }

    // 如果没有空闲进程，按默认逻辑调度
    printk(KERN_INFO "Simple Scheduler: Falling back to default scheduler\n");
    return pick_next_task_fair(rq);
}

// 简单调度器的调度类
static const struct sched_class simple_sched_class = {
    .next = &fair_sched_class,
    .pick_next_task = pick_next_task_simple,
};