#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define MAX_CPU 8

typedef union task {
  struct {
    const char *name;
    union task *next;
    void      (*entry)(void *);
    Context    *context;
  };
  uint8_t stack[8192];
} Task;  // A "state machine"

Task *currents[MAX_CPU];
#define current currents[cpu_current()]

int locked = 0;  // A spin lock
void lock()   { while (atomic_xchg(&locked, 1)); }
void unlock() { atomic_xchg(&locked, 0); }

#include "tasks.h"

Context *on_interrupt(Event ev, Context *ctx) {
  if (!current) current = &tasks[0];  // First interrupt
  else current->context = ctx;  // Save pointer to stack-saved context
  do {
    current = current->next;
  } while ((current - tasks) % cpu_count() != cpu_current());
  return current->context;  // Restore a new context
}

void mp_entry() {
  yield();  // Self-trap; never returns
}

int main() {
  cte_init(on_interrupt);

  for (int i = 0; i < LENGTH(tasks); i++) {
    Task *task    = &tasks[i];
    Area stack    = (Area) { &task->context + 1, task + 1 };
    task->context = kcontext(stack, task->entry, (void *)task->name);
    task->next    = &tasks[(i + 1) % LENGTH(tasks)];
  }
  mpe_init(mp_entry);
}