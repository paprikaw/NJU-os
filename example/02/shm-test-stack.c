// 这个程序证明了共享内存是有效的
#include "thread.h"

int *x = 0;

void Thello1(int id)
{
  int private = 2;
  x = &private;
  usleep(200000);
  printf("thread 1 id is equal to %d\n", private);
}

void Thello2(int id)
{
  usleep(100000);
  printf("access thread 1 id from thread 2, id is equal to %d\n", *x);
}

int main()
{
  create(Thello1);
  create(Thello2);
  join();
}
