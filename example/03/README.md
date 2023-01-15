1. thread-sync.h: 一个锁的实现库
2. sum-scalability.c: 使用以上这个库，进行scalability的测试
3. spinlock.py: 自旋锁的python实现，可以用model checker进行检查
4. futex.py: futex锁的model，可以用model checker进行检查
5. pc.c: 互斥锁来实现一个生产者消费者的程序（答应正确数量的括号)
6. pc-check.py: 检查前面pc.c的实现是否是正确的
7. pc-cv.c: 使用条件变量来实现同一个括号算法，但是实现是错的
7. pc-cv.py: 使用条件变量来实现同一个括号算法， 可以被model-checker.py检查的

