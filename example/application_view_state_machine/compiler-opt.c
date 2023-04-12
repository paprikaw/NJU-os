void spin_1() {
  int i;
  for (i = 0; i < 100; i++) {
    // Empty loop body
  }
}

void spin_2() {
  volatile int i;
  for (i = 0; i < 100; i++) {
    // Empty loop body
  }
}

int return_1() {
  int x;
  for (int i = 0; i < 100; i++) {
    // Compiler will assign [%0] an assembly operand
    asm("movl $1, %0" : "=g"(x));  // "x = 1;"
  }
  return x;
}

int return_1_volatile() {
  int x;
  for (int i = 0; i < 100; i++) {
    asm volatile ("movl $1, %0" : "=g"(x));
  }
  return x;
}

int foo(int *x) {
    *x = 1; // Must write at least once
    *x = 1; // because *x may be used later in external function
    return *x; // Must be 1
}

void external();
int foo_func_call(int *x) {
  *x = 1;
  external(); // May read or write x
  *x = 1;
  return *x; // Must be 1
}

int foo_volatile(int volatile *x) {
    *x = 1; // *x is volatile
    *x = 1;
    return *x;
}

int foo_volatile2(int * volatile x) {
    *x = 1; // x is volatile
    *x = 1;
    return *x;
}

int foo_barrier(int *x) {
    *x = 1;
    asm volatile("" : : : "memory"); // "Clobbers" memory
    *x = 1;
    return *x;
}
