/* 
 * $Id$
 */
//les fonctions systèmes accesibles par les process en état U (générant un Trap)


//I.O.
extern int open(char*, char*);
extern int close(int);
extern int read(int,char*,int);
extern int write(int,char*,int);
extern int ioctl(int,int);
//Proces mgmt
extern int kill(int);
extern int create(int (*)(),int,int,char*,int,int*);
extern int resume(int);
extern int suspend(int);
extern int sleep(int);
extern int _getpid(int); //parametre muet
#define getpid() _getpid(0)
extern int chprio(int,int);
extern int getprio(int);
extern int status(int,char*);

//Sémaphores
extern SEMAPHORE* screate(int,char*);
extern int sdelete(SEMAPHORE*);
extern int wait(SEMAPHORE*);
extern int signal(SEMAPHORE*);

//mem mgmt
extern int load(char*);

