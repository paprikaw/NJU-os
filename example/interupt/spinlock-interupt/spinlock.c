#include <am.h>
#include <klib.h>

int locked = 0;

void lock()
{
    while (atomic_xchg(&locked, 1))
        ;
}

void unlock()
{
    atomic_xchg(&locked, 0);
}

Context *on_interrupt(Event ev, Context *ctx)
{
    switch (ev.event)
    {
    case EVENT_IRQ_TIMER:
        lock();
        printf("Interrupt %d\n", cpu_current());
        unlock();
        break;
    default:
        break;
    }
    assert(!ienabled()); // Interrupt disabled in on_interrupt()
    return ctx;
}

void os_main()
{
    int c = cpu_current();
    iset(true);
    while (1)
    {
        lock();
        printf("Hello from CPU %d\n", c);
        unlock();
    }
}

int main()
{
    cte_init(on_interrupt);
    mpe_init(os_main);
}