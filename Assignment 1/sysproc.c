#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern int toggle_flag;

int 
sys_toggle(void)
{
  // toggle();
  // printf(0,"Inside sys_toggle\n" );
  // cprintf("%p,%d lolololo\n",&toggle_flag,toggle_flag);
  toggle_flag=1-toggle_flag;
  return 1;
}

int 
sys_add(void)
{
  int a,b;
  argint(0,&a);
  argint(1,&b);
  return (a+b);
}

// struct {
//   struct spinlock lock;
//   struct proc proc[NPROC];
// } ptable;

extern void ps();

int
sys_ps(void)
{
  ps();
  // struct proc *p;

  // acquire(&ptable.lock);

  // for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  //   if(p->state == RUNNABLE || p->state==RUNNING || p->state==SLEEPING)
  //     cprintf("pid:%d name:%s\n",p->pid,p->name);

  // release(&ptable.lock);

  return 1;
}
