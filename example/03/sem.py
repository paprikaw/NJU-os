# 这段对信号量的建模和普通的P, V信号量API是不一样的，这里的P()会直接take thread的id，然而正常的P()API会take一个信号量。这是因为这里的建模只有一个信号量，不需要考虑input argument. 由于我们需要对thread的行为建模，所以需要pass thread id来模拟block的时候将thread加入等待队列的行为


class Semaphore:
    token, waits = 1, ''

    def P(self, tid):
        if self.token > 0:
            self.token -= 1
            return True
        else:
            self.waits = self.waits + tid # 陷入操作系统, sleep
            return False

    def V(self):
        if self.waits:
            self.waits = self.waits[1:]
        else:
            self.token += 1

    @thread
    def t1(self):
        self.P('1')
        while '1' in self.waits: pass # 陷入操作系统, sleep

        cs = True
        del cs
        self.V()

    @thread
    def t2(self):
        self.P('2')
        while '2' in self.waits: pass # 陷入操作系统, sleep
        cs = True
        del cs
        self.V()

    @marker
    def mark_t1(self, state):
        if localvar(state, 't1', 'cs'): return 'blue'

    @marker
    def mark_t2(self, state):
        if localvar(state, 't2', 'cs'): return 'green'

    @marker
    def mark_both(self, state):
        if localvar(state, 't1', 'cs') and localvar(state, 't2', 'cs'):
            return 'red'
