#include "thread.h"
#include "thread-sync.h"

struct cpu
{
    int ncli;
};

struct spinlock
{
    const char *name;
    int locked;
    struct cpu *cpu;
};

__thread struct cpu lcpu;

struct cpu *mycpu()
{
    return &lcpu;
}

#define panic(...)                                           \
    do                                                       \
    {                                                        \
        fprintf(stderr, "Panic %s:%d ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                        \
        fprintf(stderr, "\n");                               \
        abort();                                             \
    } while (0)

void initlock(struct spinlock *lk, char *name)
{
    lk->name = name;
    lk->locked = 0;
    lk->cpu = 0;
}

// Pushcli/popcli are like cli/sti except that they are matched:
// it takes two popcli to undo two pushcli.  Also, if interrupts
// are off, then pushcli, popcli leaves them off.

void pushcli(void)
{
    // removes CPU-dependent code
    // eflags = readeflags();
    // cli();
    // if(mycpu()->ncli == 0)
    //   mycpu()->intena = eflags & FL_IF;
    mycpu()->ncli += 1;
}

void popcli(void)
{
    // removes CPU-dependent code
    // if(readeflags()&FL_IF)
    //  panic("popcli - interruptible");
    if (--mycpu()->ncli < 0)
        panic("popcli");
    // if(mycpu()->ncli == 0 && mycpu()->intena)
    //   sti();
}

// Check whether this cpu is holding the lock.
int holding(struct spinlock *lock)
{
    int r;
    pushcli();
    r = lock->locked && lock->cpu == mycpu();
    popcli();
    return r;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
// Holding a lock for a long time may cause
// other CPUs to waste time spinning to acquire it.
void acquire(struct spinlock *lk)
{
    pushcli(); // disable interrupts to avoid deadlock.
    if (holding(lk))
        panic("acquire");

    // The xchg is atomic.
    while (atomic_xchg(&lk->locked, 1) != 0)
        ;

    // Tell the C compiler and the processor to not move loads or stores
    // past this point, to ensure that the critical section's memory
    // references happen after the lock is acquired.
    __sync_synchronize();

    // Record info about lock acquisition for debugging.
    lk->cpu = mycpu();
}

// Release the lock.
void release(struct spinlock *lk)
{
    if (!holding(lk))
        panic("release");

    lk->cpu = 0;

    // Tell the C compiler and the processor to not move loads or stores
    // past this point, to ensure that all the stores in the critical
    // section are visible to other cores before the lock is released.
    // Both the C compiler and the hardware may re-order loads and
    // stores; __sync_synchronize() tells them both not to.
    __sync_synchronize();

    // Release the lock, equivalent to lk->locked = 0.
    // This code can't use a C assignment, since it might
    // not be atomic. A real OS would use C atomics here.
    asm volatile("movl $0, %0"
                 : "+m"(lk->locked)
                 :);

    popcli();
}

struct spinlock lk;

#define N 10000000

long sum = 0;

void Tworker(int tid)
{
    lcpu = (struct cpu){.ncli = 0};
    for (int i = 0; i < N; i++)
    {
        acquire(&lk);
        sum++;
        release(&lk);
    }
}

int main()
{
    initlock(&lk, "spinlock");
    for (int i = 0; i < 2; i++)
    {
        create(Tworker);
    }
    join();
    printf("sum = %ld\n", sum);
}