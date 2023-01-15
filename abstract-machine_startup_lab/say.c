#include <klib.h>
// say.c
void putch(char ch);
int putchar(int ch);

void say(const char *s)
{
  for (; *s; s++)
  {
#ifdef __ARCH__
    putch(*s); // AbstractMachine，没有 libc，调用 TRM API 打印字符
#else
    putchar(*s); // 操作系统，调用 libc 打印字符
#endif
  }
}
