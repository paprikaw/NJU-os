int xchg(int volatile *ptr, int newval) {
  asm volatile(
    "lock xchgl %0, %1"
    : "+m"(*ptr), "+r"(newval)
    : 
    : "memory"
  );
  return newval;
}