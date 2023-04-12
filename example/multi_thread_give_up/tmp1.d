
tmp1.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <xchg>:
   0:	f3 0f 1e fa          	endbr64 
   4:	89 f0                	mov    %esi,%eax
   6:	f0 87 07             	lock xchg %eax,(%rdi)
   9:	c3                   	retq   
