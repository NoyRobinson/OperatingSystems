#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Task 1
uint64
sys_map_shared_pages(void)
{
  struct proc* src_proc;
  struct proc* dst_proc;
  int src_proc_id;
  int dst_proc_id;
  uint64 src_va;
  uint64 size;

  argint(0, &src_proc_id);
  argint(1, &dst_proc_id);

  src_proc = find_proc(src_proc_id);
  dst_proc = find_proc(dst_proc_id);

  if(src_proc == 0 || dst_proc == 0)
    return -1;

  argaddr(2, &src_va);
  argaddr(3, &size);

  return map_shared_pages(src_proc, dst_proc, src_va, size);
}

uint64
sys_unmap_shared_pages(void)
{
  return 0;
}
///////////////
