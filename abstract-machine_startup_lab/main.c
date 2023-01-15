// main.c
#include <klib.h>
void say(const char *s);
int main()
{
  printf("\n");
  printf("hello %d %c %s\n", 1, 'h', "nihao");
  say("hello\n");
}