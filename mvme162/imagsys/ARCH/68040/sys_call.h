/*
 * $Id$
 *
 * les fonctions systemes, à priori non directement appelables par un process en mode U
 */
extern int sys_getprio(int);
extern int sys_getpid();
extern int sys_chprio(int,int);
extern int sys_suspend(int);
extern int sys_create(int (*)(int),int,int,char*,int,int*);
extern int sys_resume(int);
extern int sys_kill(int);
extern int sys_status(int,char*);

extern int sys_sleep(int);

extern SEMAPHORE* sys_screate(int,char*);
extern int sys_sdelete(SEMAPHORE*);
extern int sys_wait(SEMAPHORE*);
extern int sys_signal(SEMAPHORE*);

extern int sys_open(char*, char*);
extern int sys_close(int);
extern int sys_read(int,char*,int);
extern int sys_write(int,char*,int);
extern int sys_ioctl(int,int);

extern void* Buddy_Alloc(Buddy_Heap*,size_t,int);
extern int Buddy_Dealloc(Buddy_Heap*,void*);
extern int Buddy_GetFreeMemInHeap(Buddy_Heap*,int[],int*);
extern int sys_load(char*);

extern void sys_reschedule();
