// User-defined tasks

void func(void *arg) {
  while (1) {
    lock();
    printf("Thread-%s on CPU #%d\n", arg, cpu_current());
    unlock();
    for (int volatile i = 0; i < 100000; i++) ;
  }
}

Task tasks[] = {
  { .name = "A", .entry = func },
  { .name = "B", .entry = func },
  { .name = "C", .entry = func },
  { .name = "D", .entry = func },
  { .name = "E", .entry = func },
};